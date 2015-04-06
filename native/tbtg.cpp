#include <boost/python.hpp>
#include <vector>

using namespace boost::python;

typedef int player_id;

class TBT_game_core;

struct TBT_game_player
{
        TBT_game_player(player_id p_id,TBT_game_core * core)
        {
            this->p_id = p_id;
            this->game_core = core;
        }

        void do_turn(int hand);
        
        player_id p_id;
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
            initial_stage = true;
            last_player_id = -1;
        }

        PyObject * generate_new_player()
        { 
            players.push_back(*(new TBT_game_player(++last_player_id, this)));
            return TBT_game_player_python::convert(players[last_player_id]);
        } 

        void end_initial_stage()
        {
            initial_stage = false;
            current_turn_no = 0;
            score = std::vector<int> (last_player_id + 1, 0);
            current_turn = new std::vector<int>(last_player_id + 1, 0);
            current_turn_done = std::vector<bool>(last_player_id + 1,false);
        }

        void inform_turn_done(player_id id,int hand)
        {
            current_turn->at(id) = hand;
            current_turn_done[id] = true;
            check_next_turn();
        }

        player_id get_winner()
        {
            if (current_turn_no < 10)
                return -1;
            player_id maxi = 0;
            for (int i = 0; i < last_player_id; i++)
                if (score[i] > score[maxi])
                    maxi = i;
            return maxi;
        }
    private:

        void check_next_turn()
        {
            player_id i;
            for (i=0;i<last_player_id;i++)
                if (!current_turn_done[i])
                    return;

            do_next_turn();

            current_turn_done = std::vector<bool>(last_player_id,false);
            old_turns.push_back(*current_turn);
            current_turn = new std::vector<int> (last_player_id,0);        
        }

        /*
        All the logic is in here.
        *
        */
        void do_next_turn()
        {
            for (player_id i = 0;i<last_player_id;i++)
                for (player_id j=0;i<last_player_id;j++)
                    if (current_turn->at(i) == (current_turn->at(j)+1)%3)
                        score[i]++;
        }

        bool initial_stage;
        player_id last_player_id;
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
        .def("end_initial_stage", &TBT_game_core::end_initial_stage)
        .def("inform_turn_done", &TBT_game_core::inform_turn_done)
        .def("get_winner", &TBT_game_core::get_winner)
        .def("generate_new_player", &TBT_game_core::generate_new_player)
    ;

    class_<TBT_game_player>("TBT_game_player", init<player_id,TBT_game_core*>())
        .def("do_turn", &TBT_game_player::do_turn)
    ;

}
