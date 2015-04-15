#ifndef PATH_H
#define PATH_H

#include <vector>
#include <map>

class Path
{
	private:
		std::vector<int> xcoords;
		std::vector<int> ycoords;
		std::map<std::pair<int,int>, bool> path_dict;
	public:
		void add_coords(int x, int y);
		int x(size_t index);
		int y(size_t index);
		size_t size();
		bool in(int x, int y);
};

Path * create_path(int width, int height);

#endif
