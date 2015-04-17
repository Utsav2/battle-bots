#ifndef PATH_H
#define PATH_H

#include <vector>
#include <map>
#include "coordinate.hpp"

class Path
{
	private:
        std::vector<Coordinate> coords;
	public:
        Path(std::vector<Coordinate> coords);
        Path(int, int);
		void add_coords(int x, int y);
		void add_coords(Coordinate coord);
		size_t size();
		bool in(int x, int y);
        bool in(Coordinate coord);
        Coordinate get_coordinate(int index);
};
#include "path_creator.cpp"
#endif
