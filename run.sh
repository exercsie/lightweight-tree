#!/bin/bash

# Variables
console="[CONSOLE]"
error="[ERROR]"

# Functions
break_line() {
    echo ""
}

build() {
    if [[ ! -f "tree.cpp" ]]; then
        echo "$error Build has no target file. Please re-clone the project!"
        break_line;
        exit 1;
    fi

    echo "$console Building tree..."
    break_line;
    g++ tree.cpp -o tree -std=c++23
}

run() {
    if [[ -f "tree" ]]; then
        if [[ ! -x "tree" ]]; then
            echo "$error File is not executable, type 'chmod +x tree'"
            break_line;
            exit 1;
        fi
    else
        echo "$error Executable does not exist, please build first!"
        break_line;
        exit 1;
    fi

    echo "$console Running tree..."
    break_line;
    ./tree
}

help() {
    echo "$console './run.sh run' to run."
    echo "$console './run.sh build' to build."
    echo "$console './run.sh' to run and build."
    break_line;
}

# Error handling
if [[ "$1" != "run" && "$1" != "build" && "$1" != "help" && "$1" != "" ]]; then
    echo "$error Unknown argument!"
    break_line;
    help;
    exit 1;
fi

# Program
if [[ "$1" == "run" ]]; then
    run;
elif [[ "$1" == "build" ]]; then
    build;
elif [[ "$1" == "help" ]]; then
    help;
else
    build;
    run;
fi
