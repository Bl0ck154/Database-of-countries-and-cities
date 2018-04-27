#pragma once
#include "Header.h"
#include"defines.h"
class DataBase
{
	StructureFile file;
	Countries countries;
	MenuInterface mymenu;
public:
	DataBase();
	void ShowMenu();
};

