#!/usr/bin/env python3

import os
import re

def fpaths(path):
    for base, dnames, fnames in os.walk(path):
        for fname in fnames:
            yield os.path.join(base, fname)

def decls(path):
    with open(path, encoding='utf-8') as f:
        s = f.read()
        decls = re.findall(r'__BEGIN_DECLS\n(.+?)__END_DECLS', s, re.DOTALL)
    return decls

def functions(decl):
    decl = decl.rstrip('\n;')
    logical_lines = [s.strip() for s in decl.split(';')]
    functions = [line for line in logical_lines
                 if not line.startswith(('#define','_MACRO'))
                    and line.endswith(')')]
    return [re.sub(r'[ \t\n]+', ' ', fn) for fn in functions]

def function_name(func):
    l, r = func.split('(', 1)
    rtype, name = l.rsplit(' ', 1)
    return name.lstrip('*')

function_names = [function_name(func) for path in fpaths('src/include')
                                      for decl in decls(path)
                                      for func in functions(decl)]

function_names.sort()

for function_name in function_names:
    print(function_name)
