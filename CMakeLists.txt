cmake_minimum_required(VERSION 3.10)
project(imsmeta)

enable_language(C CXX)

add_executable(imsmeta
	imsmeta.cpp
	json.hpp
)

set_property(TARGET imsmeta PROPERTY CXX_STANDARD 17)
set_property(TARGET imsmeta PROPERTY CXX_STANDARD_REQUIRED ON)
