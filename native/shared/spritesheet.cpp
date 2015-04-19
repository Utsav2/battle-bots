Spritesheet::Spritesheet(std::string file_name, Coordinate box_size) : file_name(file_name), box_size(box_size)
{
	
}

std::string Spritesheet::get_file_name()
{
	return this->file_name;
} 

Coordinate Spritesheet::get_box_size()
{
	return box_size;
}