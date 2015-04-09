from  config_tests import TestException, ok, warning as w
import unittest

class libTurnBasedTest(unittest.TestCase):
	def test_normal_game(self):
		import libTurnBased

		gc = libTurnBased.TB_core(2)
		p1, p2 = gc.get_player_list()

		while (gc.get_winner() == -1):
			p1.do_turn(0)
			p2.do_turn(1)	
		
		self.failUnlessEqual(gc.get_winner(), 1,w("The winner value is incorrect. Check your game logic\n"))

	def test_get_player(self):
		import libTurnBased as TB
		gc = TB.TB_core(10)
		self.failUnlessEqual(len(gc.get_player_list()),10,w("Returned an incorrect number of players"))
		
if __name__ == '__main__':
    unittest.main()
