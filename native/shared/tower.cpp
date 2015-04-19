#include "tower.hpp"
#include <iostream>
#include <cmath> 

void Range::set_all(int r)
{
	if(r < 0)
	{
		r = 0;
	}

	for(size_t i = 0; i < ranges.size(); i++)
	{
		ranges[i] = r;
	}
}

Range::Range()
{

}

void Range::set_specific(Direction d, int r)
{
	ranges[d] = r;
}

bool Range::in_vertical(int vdiff)
{
	return vdiff > 0 ? ranges[NORTH] >= vdiff : ranges[SOUTH] >= vdiff;
}

bool Range::in_horizontal(int hdiff)
{
	return hdiff > 0 ? ranges[EAST] >= hdiff : ranges[WEST] >= hdiff;
}

bool Range::in_simple(int hdiff, int vdiff)
{
	if(hdiff == 0 && vdiff == 0)
		return true;

	return hdiff == 0 ? in_vertical(vdiff) : in_horizontal(hdiff);
}

bool Range::in_negative(int hdiff, int vdiff)
{
	return (hdiff > 0 && ranges[NORTHWEST] <= hdiff) || (hdiff < 0 && ranges[SOUTHEAST] <= hdiff);
}

bool Range::in_positive(int hdiff, int vdiff)
{
	return (hdiff > 0 && ranges[NORTHEAST] <= hdiff) || (hdiff < 0 && ranges[SOUTHWEST] <= hdiff);
}
bool Range::in_diagonal(int hdiff, int vdiff)
{
	return hdiff == vdiff && (hdiff * vdiff > 0 ? in_positive(hdiff, vdiff) : in_negative(hdiff, vdiff));
}
bool Range::in(Coordinate a, Coordinate b)
{
	int hdiff = b.x - a.x;
	int vdiff = b.y - a.y;
	if(hdiff == 0 || vdiff == 0)
	{
		return in_simple(hdiff, vdiff);
	}
	else
	{
		return in_diagonal(hdiff, vdiff);
	}
}

Tower::Tower(Coordinate location, std::string image_string, Spritesheet * spritesheet, std::vector<Coordinate>& cycles)
	:location(location), image_string(image_string), spritesheet(spritesheet), cycles(cycles)
{
}


Spritesheet * Tower::get_spritesheet()
{
	return spritesheet;
}

//doesnt do any error checking. call with caution
void Tower::set_attacking(Coordinate location)
{
	attacking_tiles.push_back(location);
}

void Tower::remove_attack_tile(Coordinate location)
{
	attacking_tiles.erase(std::remove(attacking_tiles.begin(), attacking_tiles.end(), location), attacking_tiles.end());
}

Coordinate Tower::get_coordinate()
{
	return this->location;
}

std::vector<Coordinate>& Tower::get_attack_tiles()
{
	return attacking_tiles;
}

std::string Tower::get_image_string()
{
	return image_string;
}

std::vector<Coordinate>& Tower::get_sscords()
{
	return cycles;
}
