#ifndef TOWER_H
#define TOWER_H
#include <iostream>
#include <vector>

typedef std::pair<int, int> coordinate;

enum Direction { NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST };

class Range
{
	private:
		std::vector<int> ranges;
		bool in_vertical(int);
		bool in_horizontal(int);
		bool in_simple(int, int);
		bool in_negative(int, int);
		bool in_positive(int, int);
		bool in_diagonal(int, int);
	public:
		Range();
		Range(int);
		void set_all(int);
		void set_specific(Direction, int);
		bool in(coordinate, coordinate);
};

class Tower
{

	private:
		std::string _image_string;
		std::string _attack_image_string;
		Range * range;
		std::vector<coordinate> attacking_tiles;

	public:
		Tower();
		void set_image_string(std::string);
		std::string get_image_string();
		void set_attack_image_string(std::string);
		std::string get_attack_image_string();
		void set_attacking(coordinate);
		void remove_attack_tile(coordinate);
		std::vector<coordinate>& get_attack_tiles();
};


#endif