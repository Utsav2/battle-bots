Spritesheet::Spritesheet(std::string file_name, Coordinate box_size, size_t frame_rate) : file_name(file_name), box_size(box_size), frame_rate(frame_rate)
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
size_t Spritesheet::get_frame_rate()
{
	return frame_rate;
}