/*
Dragonfly Metadata to JSON converter
https://github.com/UQ-RCC/imsmeta

SPDX-License-Identifier: Apache-2.0
Copyright (c) 2019 The University of Queensland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <vector>
#include <iostream>
#include <regex>
#include "json.hpp"

using json = nlohmann::json;

static std::regex regex_value("^\\s*([\\w\\s]+)=(.*)$", std::regex_constants::ECMAScript);
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