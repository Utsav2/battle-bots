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

Path::Path()
{
    
}

Path::Path(int width, int height)
{
	 for(int i = -1; i <= width ; i++)
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

void Path::append_path(Path latter)
{
    for (size_t i=0; i < latter.coords.size(); i++)
    {
        if (this->coords.back() != latter.coords[i])
        {
            this->coords.push_back(latter.coords[i]);
        }
    }
}

bool are_adjacent(Coordinate c1, Coordinate c2)
{
    Coordinate diff = c1 - c2;
    return abs(diff.x) + abs(diff.y) == 1;
}

bool are_diagonal_adjacent(Coordinate c1, Coordinate c2)
{
    Coordinate diff = c1 - c2;
    return abs(diff.x) == 1 && abs(diff.y) == 1;
}

static Path generate_linear_path(Coordinate start, Coordinate end)
{

    if (start == end)
    {
        std::vector<Coordinate> coords;
        coords.push_back(start);
        return Path(coords);
    }
    if (are_adjacent(start,end))
    {
        std::vector<Coordinate> coords;
        coords.push_back(start);
        coords.push_back(end);
        return Path(coords);
    }
    if (are_diagonal_adjacent(start,end))
    {
        std::vector<Coordinate> coords;
        coords.push_back(start);
        coords.push_back(Coordinate(start.x,end.y));
        coords.push_back(end);
        return Path(coords);
    }

    Coordinate mid_left = (end-start)/2 + start;
    Coordinate mid_right = (start-end)/2 + end;

    if (are_diagonal_adjacent(mid_left,mid_right))
    {
        mid_left.x = mid_right.x;
    }

    Path left = generate_linear_path(start,mid_left);
    Path right = generate_linear_path(mid_right,end);
    left.append_path(right);
    return left;
}
