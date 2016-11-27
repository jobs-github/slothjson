#!/usr/bin/python
# email: yao050421103@gmail.com
import sys
import os.path
import string
import shutil

def make():
    if os.path.exists('release'):
        shutil.rmtree('release')
    dirs = [
        'release', 
        'release/res'
        ]
    for dir in dirs:
        os.mkdir(dir)
    cmds = [
        'cp -r doc release',
        'cp res/design.png release/res',
        'cp res/logo.png release/res',
        'cp README.md release/',
        'cp README_ZH.md release/'
        ]
    for cmd in cmds:
        os.system(cmd)
    src = 'release/'
    des = 'html/'
    if os.path.exists(des):
        shutil.rmtree(des)
    os.system('generate-md --layout github --input %s --output %s' % (src, des))
    shutil.rmtree('release')
    print 'make doc success!'

if __name__ == "__main__":
    make()