#include "Countries.h"


Countries::Countries()
{
}

void Countries::AddTo(MapType m, string key, string value)
{
	if (m == COUNTRIES)
		countries.items.insert(make_pair(key, value));
	else if (m == DISTINCTS)
		distincts.items.insert(make_pair(key, value));
	else if(m == FOLDERS)
		folders.items.insert(make_pair(key, value));
}

str_map& Countries::getContent(MapType m)
{
	if(m == COUNTRIES)
		return countries.items;
	else if(m == DISTINCTS)
		return distincts.items;
//	else if(m == FOLDERS)
	return folders.items;
}

void Countries::set_pos_to_write(MapType m, size_t pos)
{
	if (m == COUNTRIES)
		countries.pos_to_write = pos;
	else if (m == DISTINCTS)
		distincts.pos_to_write = pos;
	else if(m == FOLDERS)
		folders.pos_to_write = pos;
}

size_t Countries::get_pos_to_write(MapType m)
{
	if (m == COUNTRIES)
		return countries.pos_to_write;
	else if (m == DISTINCTS)
		return distincts.pos_to_write;
//	else if (m == FOLDERS)
	return folders.pos_to_write;
}

void Countries::update_pos_to_write(MapType m, string& str)
{
	if (m == COUNTRIES)
		countries.pos_to_write += str.size();
	else if (m == DISTINCTS)
		distincts.pos_to_write += str.size();
	else if (m == FOLDERS)
		folders.pos_to_write += str.size();
}

