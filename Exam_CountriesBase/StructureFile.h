#pragma once
#include <stdio.h> // подключаю библиотеку для работы с файлами
#include"defines.h"
using namespace std;
#define MANIFEST_FILE "manifest.txt"

class StructureFile
{
	string fname; // имя файла с которым работаем
	FILE *file; // создаю указатель файла
	string content; // содержимое файла
public:
	StructureFile(); // конструктор
	StructureFile(string fname); // констуктор с именем файла, если манифест называется по другому
	~StructureFile(); // деструктор закрывает открытый файл
	bool open(); // открываем файл
	bool close(); // закрываем файл
	bool read(); // считываем файл в наш string content
	bool open_and_read(); // решил обьеденить 3 предыдущих метода в один, для удобства
	string& getContent(); // возвращает содержимое файла в string
	size_t get_key_value(string keyword, str_map&m); // keyword - слово перед кавычками в файле манифест, map - ключ=значение, возвращает позицию окончания
	bool insert(size_t pos, string what); // добавление строки в файл по позиции pos
	size_t size(); //возвращает размер поля content
	size_t next_brace_pos(size_t pos); // используется для обновления позиция для записи pos_to_write
};
