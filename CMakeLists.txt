cmake_minimum_required(VERSION 3.14)

PROJECT(checker)

ADD_EXECUTABLE(checker Dictionary.cpp main.cpp)
set_property(TARGET checker PROPERTY CXX_STANDARD 17)
