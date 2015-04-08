#include <boost/python.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>

typedef int Player_id;

class TB_core;

struct TB_player
{
        TB_player(Player_id p_id, TB_core * core)
        {
            this->p_id = p_id;
            this->core = core;
        }

        void do_turn(int hand);
        
        Player_id p_id;
        TB_core * core;
};


class TB_core
{
    public:
        TB_core(int number_of_players)
        {
            last_player_id = -1;
            current_turn = new std::vector<int> ();
            current_turn_no = 0;
            while(number_of_players-- > 0)
            	generate_new_player();
        }

        boost::python::list get_player_list()
        {
		
			boost::python::list ret;
			for(size_t i = 0 ; i < players.size() ; i++)
			{
				ret.append(players[i]);
			}
			return ret;
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

        bool is_finished()
        {
        	return get_winner() != -1;
        }

        inline int num_players()
        {
            return last_player_id + 1;
        }

    private:

    	void generate_new_player()
        {
            players.push_back(*(new TB_player(++last_player_id, this)));
            current_turn_done.push_back(false);
            score.push_back(0);
            current_turn->push_back(0);
        }


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
        std::vector<TB_player> players;
        std::vector<int> score; 
        std::vector<int> * current_turn;
        std::vector<bool> current_turn_done;
        std::vector< std::vector<int> > old_turns;
};

void TB_player::do_turn(int hand)
{
            this->core->inform_turn_done(this->p_id, hand);
}

using namespace boost::python;

BOOST_PYTHON_MODULE(libTurnBased)
{

    class_<TB_core>("TB_core", init<int>())
        .def("get_winner", &TB_core::get_winner)
        .def("get_player_list", &TB_core::get_player_list)
        .def("is_finished", &TB_core::is_finished)
    ;

    class_<TB_player>("TB_player", init<Player_id,TB_core*>())
        .def("do_turn", &TB_player::do_turn)
    ;

}
