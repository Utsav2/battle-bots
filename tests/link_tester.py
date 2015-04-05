from config_tests import fail,ok
try:
	import libyay
except ImportError as e:
	addendum = fail("Did you remember to run ./configure? \nOr you might have not added build to PYTHONPATH")
	raise ImportError(e.message + "\n" + addendum) 

print(ok("link test set passed"))
