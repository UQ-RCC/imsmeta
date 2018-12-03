/*
Dragonfly Metadata to JSON converter

https://github.com/UQ-RCC/imsmeta

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2018 The University of Queensland.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <vector>
#include <iostream>
#include <regex>
#include "json.hpp"

using json = nlohmann::json;

static std::regex regex_value("^\\s*(\\w+)=(.*)$", std::regex_constants::ECMAScript);
static std::regex regex_section("^\\s*\\[([\\w\\s]+)\\]$", std::regex_constants::ECMAScript);
static std::regex regex_feature("^\\s*\\{DisplayName=([^,]*),\\s*Value=([^\\}]*)\\}$", std::regex_constants::ECMAScript);

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static size_t get_level(const std::string& s) noexcept
{
	size_t level = 0;
	for(size_t i = 0; i < s.size(); ++i)
	{
		if(s[i] == '\t')
			++level;
		else
			break;
	}

	return level;
}

static void pop_and_merge(json& current, std::stack<json>& jstack, std::stack<std::string>& nstack)
{
	json j = std::move(jstack.top());
	j[nstack.top()] = std::move(current);

	jstack.pop();
	nstack.pop();
	current = std::move(j);
}

/* Make some sense of this god-awful format. */
static json parse_ims_metadata(std::istream& is)
{
	/* TODO: Change this to use one stack and std::pair<> */
	std::stack<json> jstack;
	std::stack<std::string> nstack;

	size_t level = 0;
	bool feature_list = false;
	json current = json::object();
	for(std::string s; std::getline(is, s);)
	{
		/* Strip newlines and skip empty. */
		rtrim(s);
		if(s.empty())
			continue;

		/* Get the level, then remove the leading tabs. */
		size_t l = get_level(s);
		ltrim(s);

		if(l > level)
		{
			/* New level without a section header. */
			throw std::exception();
		}
		else if(l < level)
		{
			/* Level end, pop back as much as we need to. */
			for(size_t i = l; i < level; ++i)
				pop_and_merge(current, jstack, nstack);

			/* Can't have a feature-list as a child of a value-list. */
			feature_list = false;
			level = l;
		}

		/* Handle a feature list. */
		if(feature_list)
		{
			std::smatch m;
			if(!std::regex_match(s, m, regex_feature))
				throw std::exception();

			current.push_back({{"DisplayName", m[1]}, {"Value", m[2]}});
			continue;
		}

		/* Handle everything else. These are almost like recursive INI files. */
		std::smatch m;
		bool is_section = std::regex_match(s, m, regex_section);
		if(is_section)
		{
			jstack.push(std::move(current));
			nstack.push(m[1]);

			feature_list = m[1] == "FeatureValueList";
			if(feature_list)
				current = json::array();
			else
				current = json::object();

			++level;
		}
		else
		{
			if(!std::regex_match(s, m, regex_value))
				throw std::exception();

			current[m[1]] = m[2];
		}
	}

	while(!jstack.empty())
		pop_and_merge(current, jstack, nstack);

	return current;
}

int main(int argc, char **argv)
{
	std::cin.exceptions(std::ios::badbit);
	json meta = parse_ims_metadata(std::cin);
	std::cout << meta;
	return 0;
}