TDMap::TDMap(int width, int height, std::vector<Path *> paths)
	:dimensions(Coordinate(width,height)),paths(paths)
{
}

TDMap::TDMap(Coordinate dimensions, std::vector<Path *> paths)
	:dimensions(dimensions)
{
	assert(dimensions.x > 0 && dimensions.y > 0 && !(paths.empty()));
	this->paths = paths;
}

/*
boost does range checking, so we dont need to do it
we can think about it later.
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

std::vector<Sprite *> TDMap::get_sprites_at(int row, int column)
{
	return get_sprites_at(Coordinate(row,column));
}

Tower * TDMap::get_tower_at(int row, int column)
{
	return get_tower_at(Coordinate(row,column));
}

bool TDMap::add_tower(Tower * tower)
{
	if (get_tower_at(tower->get_coordinate()) != nullptr)
		return false;

	this->towers.push_back(tower);
	return true;
}

bool TDMap::add_sprite(Sprite * sprite)
{
	if (!get_sprites_at(sprite->get_coordinate()).empty())
		return false;

	this->sprites.push_back(sprite);
	return true;
}