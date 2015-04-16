import sys
import traceback

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def warning(string):
	return bcolors.WARNING + string + bcolors.ENDC

def fail(string):
	return bcolors.FAIL + string + bcolors.ENDC
	
def ok(string):
	return bcolors.OKBLUE + string + bcolors.ENDC

def print_fatal_error(string):
	print(fail(string))
	exit()

def print_success(string):
	print ok(string)

class TestException(Exception):
	def __init__(self,base_error, message, *args):
		parent = base_error.message
		_, _, tb = sys.exc_info()
		traceback.print_tb(tb) # Fixed format
		if parent == "":
			tb_info = traceback.extract_tb(tb)
			filename, line, func, text = tb_info[-1]
			parent = 'An error occurred on line {} in statement:\n\t{}'.format(line, text)
		if message != "":
			message = "\n" + message
		super(TestException, self).__init__("\n" + fail(parent  + message))

