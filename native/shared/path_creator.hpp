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
		Path();
        Path(std::vector<Coordinate> coords);
        Path(int, int);
		void add_coords(int x, int y);
		void add_coords(Coordinate coord);
		size_t size();
		bool in(int x, int y);
        bool in(Coordinate coord);
        Coordinate get_coordinate(int index);
        void append_path(Path);
        static Path generate_linear_path(Coordinate, Coordinate);
};
#include "path_creator.cpp"
#endif
