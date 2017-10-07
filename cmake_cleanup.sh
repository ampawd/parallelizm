#!/bin/bash

if [ -d ./CMakeFiles ];
then
	rm -rf CMakeFiles
fi

if [ -f ./cmake_install.cmake ] 
then
	rm cmake_install.cmake
fi

if [ -f ./CMakeCache.txt ];then
	rm CMakeCache.txt
fi

if [ -f ./Makefile ];then
	rm Makefile
fi

if [ -f ./parallelizm ];then
	rm parallelizm
fi 
