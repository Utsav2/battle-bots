#include "path_creator.h"

void Path::add_coords(int x, int y)
{
	assert(x >= 0 && y >= 0);
	xcoords.push_back(x);
	ycoords.push_back(y);
	path_dict[std::make_pair(x, y)] = true;
}
int Path::x(size_t index)
{
	assert(index < xcoords.size());
	return xcoords[index];
}
int Path::y(size_t index)
{
	assert(index < ycoords.size());
	return ycoords[index];
}
bool Path::in(int x, int y)
{
	return path_dict[std::make_pair(x, y)];
}

size_t Path::size()
{
	return xcoords.size();
}

Path * create_path_simple(int width, int height)
{
	Path * path = new Path();
	for(int i = 0; i < width; i++)
	{
		path->add_coords(i, height/2);
	}
	return path;
}

Path * create_path(int width, int height)
{
	return create_path_simple(width, height);
}