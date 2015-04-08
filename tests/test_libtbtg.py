from  config_tests import TestException, ok, warning
try:
	import libTurnBased

	gc = libTurnBased.TB_core()
	p1 = gc.generate_new_player()
	p2 = gc.generate_new_player()

	while (gc.get_winner() == -1):
		p1.do_turn(0)
		p2.do_turn(1)
except Exception as e:
	raise TestException(e, "")

if (gc.get_winner() != 1):
	print warning("The winner value is incorrect. Check your game logic\n")
else:
	print ok("Turn By Turn Game library tests passed.\n") 
