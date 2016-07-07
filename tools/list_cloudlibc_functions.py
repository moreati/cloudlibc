#!/usr/bin/env python3

import os

def functions():
    symbols = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                           '../src/libc/symbols'))
    with open(symbols) as f:
        function_names = [line.rstrip() for line in f]
    function_names.sort()
    return function_names

if __name__ == '__main__':
    function_names = functions()
    for function_name in function_names:
        print(function_name)
