import libtd
from random import randint as r
from config_tests import ok, warning
a = libtd.Core()
def buy_random():
    if a.money >= 100:
        x = r(0, 9)
        y = r(0, 9)
        a.tower(x,y)
        print "made a tower at (",x,",",y,")"

#buy a random tower
buy_random()
#buy tower at specific coordinates
a.tower(3, 3)
a.tower(6, 3)
#print scores, round numbers
print "score is ", a.score
print "round number is", a.round 
a.loop(5)
print ok("made gui succesfully")
	