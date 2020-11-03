CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror

LIBHEADER=./cgilib.h

test: ./test/*
	for prog in $^; do\
		$(CXX) prog -I.
	done

