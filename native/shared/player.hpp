class Player
{
	public:
		int money;
        int score;
        int kills;

    Player(int money=1000,int score=0,int kills=0)
    {
		this->money = money;
		this->score = score;
		this->kills = kills;
    }
};