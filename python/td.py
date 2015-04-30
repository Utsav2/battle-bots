import libtd as lib
from save_eval import safe_eval
import random
from sys import argv

#needs a file to exec
if(len(argv) < 2):
    print "please enter a filename for your code"
    exit(1)

#checks if the file is valid
try:
    code_file = open(argv[1])
except IOError: 
    print "please enter a valid filename"
    exit(1)

#reads the code from the file
code = code_file.read()
#spawns the gamecore
core = lib.Core()
#tries to safely execute it
safe_eval(code, {'core' : core})
