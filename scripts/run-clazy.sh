#!/usr/bin/env bash

find . \( -name '*.hpp' \) -exec clazy-standalone -checks=level1 -p ./compile_commands.json {} \;

find . \( -name '*.cpp' \) -exec clazy-standalone -checks=level1 -p ./compile_commands.json {} \;
