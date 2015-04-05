from  config_tests import *
from  libworld import World

try:
	p = World()
	p.set("lol")
	assert p.greet() == "lol"
except:
	print fail("something went wrong here :-| ")
print ok("constructor test set passed")
