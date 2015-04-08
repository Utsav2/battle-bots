#include <boost/python.hpp>
#include <vector>
#include <iostream>

using namespace boost::python;

typedef int Player_id;

class TBT_game_core;

struct TBT_game_player
{
        TBT_game_player(Player_id p_id,TBT_game_core * core)
        {
            this->p_id = p_id;
            this->game_core = core;
        }

        void do_turn(int hand);
        
        Player_id p_id;
        TBT_game_core * game_core;
};

struct TBT_game_player_python
{
	static PyObject* convert(TBT_game_player const& p)
    {
		return boost::python::incref(boost::python::object(p).ptr());
    }
};

class TBT_game_core
{
    public:
        TBT_game_core()
        {
            last_player_id = -1;
            current_turn = new std::vector<int> ();
            current_turn_no = 0;
        }

        PyObject * generate_new_player()
        {
            players.push_back(*(new TBT_game_player(++last_player_id, this)));
            current_turn_done.push_back(false);
            score.push_back(0);
            current_turn->push_back(0);
            return TBT_game_player_python::convert(players[last_player_id]);
        }

        void inform_turn_done(Player_id id,int hand)
        {
            current_turn->at(id) = hand;
            current_turn_done[id] = true;
            check_next_turn();
        }

        Player_id get_winner()
        {
            if (current_turn_no < 10)
                return -1;
            Player_id maxi = 0;
            for (int i = 0; i < num_players(); i++)
                if (score[i] > score[maxi])
                    maxi = i;
            return maxi;
        }

        inline int num_players()
        {
            return last_player_id + 1;
        }
    private:

        void check_next_turn()
        {
            for (Player_id i=0;i<num_players();i++)
                if (!current_turn_done[i])
                    return;

            do_next_turn();

            current_turn_done = std::vector<bool>(num_players(),false);
            old_turns.push_back(*current_turn);
            current_turn = new std::vector<int> (num_players(),0);        
        }

        /*
        All the logic is in here.
        *
        */
        void do_next_turn()
        {
            for (Player_id i = 0;i<num_players();i++)
                for (Player_id j=0;j<num_players();j++)
                    if (current_turn->at(i) == (current_turn->at(j)+1)%3)
                        score[i]++;
            current_turn_no++;
        }

        Player_id last_player_id;
        int current_turn_no;
        std::vector<TBT_game_player> players;
        std::vector<int> score; 
        std::vector<int> * current_turn;
        std::vector<bool> current_turn_done;
        std::vector< std::vector<int> > old_turns;
};

void TBT_game_player::do_turn(int hand)
{
            this->game_core->inform_turn_done(this->p_id, hand);
}

BOOST_PYTHON_MODULE(libtbtg)
{

	boost::python::to_python_converter<
	  TBT_game_player,
	  TBT_game_player_python>();

    class_<TBT_game_core>("TBT_game_core")
        .def("inform_turn_done", &TBT_game_core::inform_turn_done)
        .def("get_winner", &TBT_game_core::get_winner)
        .def("generate_new_player", &TBT_game_core::generate_new_player)
    ;

    class_<TBT_game_player>("TBT_game_player", init<Player_id,TBT_game_core*>())
        .def("do_turn", &TBT_game_player::do_turn)
    ;

}
