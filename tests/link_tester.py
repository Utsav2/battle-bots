from config_tests import TestException, ok
try:
	import libyay
except ImportError as e:
	addendum = "Did you remember to run ./configure? \nOr you might have not added build to PYTHONPATH"
	raise TestException(e,  addendum) 

print(ok("link test set passed"))
