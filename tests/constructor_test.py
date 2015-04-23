import libtd
from random import randint as r
from config_tests import ok, warning
a = libtd.Core()
def buy_random():
	while a.money >= 100:
		a.tower(r(0, 9), r(0, 9))

buy_random()
a.loop(10)

if a.money < 0:
	print warning("negative money")

else:
	print ok("made gui succesfully")