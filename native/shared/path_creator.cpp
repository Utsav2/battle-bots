void Path::add_coords(int x, int y)
{
    this->add_coords(Coordinate(x,y));
}

void Path::add_coords(Coordinate coord)
{
    this->coords.push_back(coord);
}

Path::Path(std::vector<Coordinate> coords)
{
    this->coords = coords;
}

Path::Path(int width, int height)
{
	for(int i = 0; i < width; i++)
	{
		this->add_coords(i, height/2);
	}
}

bool Path::in(int x, int y)
{
    return in(Coordinate(x,y));
}

bool Path::in(Coordinate coord)
{
    for (size_t i=0;i<this->coords.size();++i)
        if (coords[i] == coord)
            return true;
    return false;
}

size_t Path::size()
{
	return coords.size();
}

Coordinate Path::get_coordinate(int index)
{
    return this->coords[index];
}