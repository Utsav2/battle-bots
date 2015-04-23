import libtd
from random import randint as r
from config_tests import ok
a = libtd.Core()
def buy_random():
	while a.money >= 100:
		a.tower(r(0, 9), r(0, 9))

a.tower(3, 3)
a.tower(6, 7)

a.loop(20)
print ok("made gui succesfully")
