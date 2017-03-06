#!/usr/bin/env python

from jinja2 import Environment, PackageLoader, select_autoescape
import sys
import re

def uc(string):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', string)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).upper()

def main() :
    '''

    '''
    if len(sys.argv) <2 :
        print u"no class name"
        return
    if len(sys.argv)==3 :
        _namespace=sys.argv[2]
    else:
        _namespace=""

    env = Environment(
        loader=PackageLoader('class', 'templates')
    )
    env.filters['uc'] = uc
    template = env.get_template('class.j2')
    print template.render(name=sys.argv[1], namespace=_namespace)


if __name__ == '__main__':
    main()
