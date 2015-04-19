#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "coordinate.hpp"
#include <iostream>

class Spritesheet
{
	private:
		std::string file_name;
		Coordinate box_size;
	public:
    	Spritesheet(std::string file_name, Coordinate box_size);   
    	std::string get_file_name(); 
    	Coordinate get_box_size();
};

#include "spritesheet.cpp"
#endif
