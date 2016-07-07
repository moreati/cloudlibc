#!/usr/bin/env python3 

import sys

import jinja2

import list_cloudlibc_functions
import list_glibc_functions


if __name__ == '__main__':
    cloudlibc_funcs = set(list_cloudlibc_functions.functions())
    glibc_funcs = set(list_glibc_functions.functions())
    combined_funcs = list(sorted(cloudlibc_funcs.union(glibc_funcs)))

    env = jinja2.Environment(
        loader=jinja2.FileSystemLoader(['./templates']),
        trim_blocks=True,
        lstrip_blocks=True,
    )
    template = env.get_template('functions.html')
    output = template.render(
        functions=combined_funcs,
        glibc_functions=glibc_funcs,
        cloudlibc_functions=cloudlibc_funcs,
    )
    sys.stdout.write(output)
