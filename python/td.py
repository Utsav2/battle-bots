from flask import Flask, render_template, request, abort
import libtd as lib
from save_eval import safe_eval
import random
from sys import argv

if(len(argv) < 2):
    print "please enter a filename for your code"
    exit(1)
try:
    code_file = open(argv[1])
except IOError: 
    print "please enter a valid filename"
    exit(1)

code = code_file.read()
core = lib.Core()
safe_eval(code, {'core' : core})
