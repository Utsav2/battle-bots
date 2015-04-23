import libtd
from config_tests import ok
a = libtd.Core()
for i in range(10):
	a.loop()
print ok("made gui succesfully")
