#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "coordinate.hpp"
#include <iostream>

class Spritesheet
{
	private:
		std::string file_name;
		Coordinate box_size;
		size_t sprite_number;
		size_t frame_rate;
	public:
    	Spritesheet(std::string file_name, Coordinate box_size, size_t frame_rate);   
    	std::string get_file_name(); 
    	Coordinate get_box_size();
    	size_t get_frame_rate();
};

#include "spritesheet.cpp"
#endif
