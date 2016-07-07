#!/usr/bin/env python3

import urllib.request
import re


def functions():
    with urllib.request.urlopen('https://www.gnu.org/software/libc/manual/html_node/Function-Index.html') as f:
        s = f.read().decode('utf-8')

    function_names = [name.lstrip('*') for name in re.findall('<code>(.+?)</code>', s)]
    function_names.sort()
    return function_names

if __name__ == '__main__':
    function_names = functions()
    for name in function_names:
        print(name)
