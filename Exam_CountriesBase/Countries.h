#pragma once
#include"defines.h"
using namespace std;

enum MapType // основные типы, которые хранятся в классе стран
{
	COUNTRIES,
	DISTINCTS,
	FOLDERS
};

class Countries
{
	Countries(); // конструктор приватный, нельзя создать данный класс в main'e
	friend class DataBase; // только в дружественном классе DataBase
	struct // страны и их ключи
	{
		str_map items; // мапа ключ=значение // countries { Ukraine=ukr; United States=us }
		size_t pos_to_write; // позиция закрытой скобки в файле, используется для добавления записей в файл
	} countries, distincts, folders;// ключи стран и названия регионов// ключи стран и названия папок

public:
	void AddTo(MapType m, string key, string value); // добавлениe страны в манифест

	str_map& getContent(MapType); // возвращает ссылку на мапу соответствующего типа, метод используется для заполнения этой мапы
	void set_pos_to_write(MapType, size_t); // установить позицию для записи
	size_t get_pos_to_write(MapType); // получить позицию для записи
	void update_pos_to_write(MapType, string&); // обновить(сдвинуть) позицию для записи
};

