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
