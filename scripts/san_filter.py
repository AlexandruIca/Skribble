#!/usr/bin/env python3

from typing import List

import sys

log: List[List[str]] = []
logging: bool = False

def filter_line(l: List[List[str]], line: str) -> bool:
    avoid = ["fontconfig", "QQml", "QQuick", "qRegister", "<unknown module>"]

    for to_avoid in avoid:
        if to_avoid in line:
            if l:
                l.pop()
            return False

    l[-1].append(line)
    return True

with open('./output.txt') as f:
    for line in f:
        content: str = line[:len(line) - 1]
        if line.startswith('Direct'):
            log.append([content])
            logging = True
        elif ('#' in line) and logging == True:
            logging = filter_line(log, content)
        else:
            logging = False

if not log:
    sys.exit(0)

for lines in log:
    for line in lines:
        print(line)

sys.exit(1)
