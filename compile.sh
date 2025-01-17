#!/usr/bin/env bash
mkdir -p bin
if [[ ! -f bin/make_random || make_random.c -nt bin/make_random ]]; then
    echo "Compiling make_random.c..."
    cc -O3 make_random.c -o bin/make_random
else
    echo "make_random.c is up to date."
fi
if [[ ! -f bin/solve || make_csvs.cpp -nt bin/make_csvs ]]; then
    echo "Compiling make_csvs.cpp..."
    c++ -O3 -std=c++11 make_csvs.cpp -o bin/make_csvs
else
    echo "make_csvs.cpp is up to date."
fi
if [[ ! -f bin/solve || main.cpp -nt bin/solve ]]; then
    echo "Compiling main.cpp..."
    c++ -O3 -std=c++11 main.cpp -o bin/solve
else
    echo "main.cpp is up to date."
fi
echo "Compiled"
