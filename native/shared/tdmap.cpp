Player::Player(int money = 1000)
{
	this->money = money;
}


bool Player::still_playing()
{
	return is_playing;
}


/*
   Just a constructor.
*/
TDMap::TDMap(int width, int height, std::vector<Path *> paths)
	:dimensions(Coordinate(width,height)),paths(paths)
{
}

/*
   Just another constructor.
*/
TDMap::TDMap(Coordinate dimensions, std::vector<Path *> paths)
	:dimensions(dimensions)
{
	assert(dimensions.x > 0 && dimensions.y > 0 && !(paths.empty()));
	this->paths = paths;
}

/*
   returns all the sprites at coordinate c.
*/
std::vector<Sprite *> TDMap::get_sprites_at(Coordinate c)
{
	std::vector<Sprite *> to_return(0);
	BOOST_FOREACH(Sprite * s, this->sprites)
	{
		if (s->get_coordinate() == c)
		{
			to_return.push_back(s);
		}
	}
	return to_return;
}

/*
   returns the Tower at coordinate c.
*/
Tower * TDMap::get_tower_at(Coordinate c)
{
	BOOST_FOREACH(Tower * t, this->towers)
	{
		if (t->get_coordinate() == c)
		{
			return t;
		}
	}
	return nullptr;	
}

/*
   returns the sprites at coordinate (row,column).
*/
std::vector<Sprite *> TDMap::get_sprites_at(int row, int column)
{
	return get_sprites_at(Coordinate(row,column));
}

/*
   returns the Tower at coordinate (row,column).
*/
Tower * TDMap::get_tower_at(int row, int column)
{
	return get_tower_at(Coordinate(row,column));
}

/*
   Adds a tower.
   Returns true if tower successfully added.
*/
bool TDMap::add_tower(Tower * tower)
{
    Coordinate c = tower->get_coordinate();
    Coordinate d = this->dimensions;
    if ( ! (c.x < d.x && c.y < d.y ))
        return false;

	if (get_tower_at(c) != nullptr)
		return false;

	BOOST_FOREACH(Path * path, paths)
	{
		if(path->in(c))
			return false;
	}

	this->towers.push_back(tower);
	return true;
}

/*
   Adds a tower.
   Returns true if sprite successfully added.
*/
bool TDMap::add_sprite(Sprite * sprite)
{

    Coordinate c = sprite->get_coordinate();
    Coordinate d = this->dimensions;
    if ( ! (c.x < d.x && c.y < d.y ))
        return false;

	this->sprites.push_back(sprite);
	return true;
}

void TDMap::remove_sprite(Sprite * sprite)
{
	this->sprites.erase(std::remove(this->sprites.begin(), this->sprites.end(), sprite), this->sprites.end());
}

void TDMap::update_towers()
{
	BOOST_FOREACH(Tower * tower, this->towers)
	{
		tower->Update();
	}
}