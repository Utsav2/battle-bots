from  config_tests import TestException, ok 
from  libworld import World

try:
	p = World()
	p.set("lol")
	assert p.greet() == "lol2"
except Exception as e:
	raise TestException(e, "")

print ok("constructor test set passed")
