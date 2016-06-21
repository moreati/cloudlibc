#!/usr/bin/env python3

import urllib.request
import re


with urllib.request.urlopen('https://www.gnu.org/software/libc/manual/html_node/Function-Index.html') as f:
    s = f.read().decode('utf-8')

function_names = [name.lstrip('*') for name in re.findall('<code>(.+?)</code>', s)]
function_names.sort()

for name in function_names:
    print(name)
