#pragma once
#include <iostream>
#include "Countries.h"
#include "StructureFile.h"
#include <iterator>
#include"defines.h"
using namespace std;

#define KEY_UP 72		// переход вверх
#define KEY_RIGHT 77		// переход вправо
#define KEY_LEFT 75		// переход влево
#define KEY_DOWN 80		// переход вниз
#define KEY_ENTER 13	// перейти в папку
#define KEY_EXIT 27		// выйти
#define KEY_BACKSPACE 8		// назад
#define KEY_F1 59		// новый файл

class MenuInterface
{
	Countries* countries;
	StructureFile* file;
	str_map*temp_map; // контейнер с пунктами меню
	str_map::iterator activeIndex;		// текущий индекс
	string inCountry, inRegion;
	string currPath; // путь к файлам и каталогам
	struct
	{
		str_map items;
		size_t pos_to_write;
	} _temp;
	string RegionTmp; // сюда будем сохранять полное название региона(области)
	void enter_key(); // метод нажатия кнопки выбора (enter) 
	void ShowMenu(string path = "");
	void set_default_path();
	void ReBuild(string path);
	void next(); // смещение активного пункта + 1
	void prev(); // смещение активного пункта - 1
	void KeyAction(); // кнопки меню
	void CreateNew(); // метод создания файла/папки
	size_t menu_items_count;
public:
	MenuInterface();
	void set(Countries &countries, StructureFile &file);
	void Start();

};

