#!/usr/bin/env python3

from typing import List

import json

def filter_file_name(name: str) -> bool:
    not_wanted: List[str] = [
        "mocs_compilation.cpp",
        "qrc_qml.cpp",
        "qrc_icons.cpp"
    ]

    for pattern in not_wanted:
        if name.endswith(pattern) == True:
            return True

    return False

array = []

with open("./compile_commands.json") as f:
    data = json.load(f)

    for obj in data:
        if filter_file_name(obj["file"]) == False:
            string = '{\n'
            string += f'  "directory": "{obj["directory"]}",\n'
            string += f'  "command": "{obj["command"]}",\n'
            string += f'  "file": "{obj["file"]}"\n'
            string += '}'
            array.append(string)

    print('[')
    print(',\n'.join(array))
    print('\n]')
