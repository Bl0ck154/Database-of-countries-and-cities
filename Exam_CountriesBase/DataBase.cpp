#include "DataBase.h"

DataBase::DataBase()
{
	if (!file.open_and_read()) // открыем манифест и читаем его
		throw exception("File opening or reading error"); // если вернулся false, то бросаем exception

	// file.get_key_value считываем из файла ключ=значение которые входят в кавычки. Сразу же записываем позицию countries.set_pos_to_write последнего символа, для дальнейшей записи
	countries.set_pos_to_write(COUNTRIES, file.get_key_value("countries", countries.getContent(COUNTRIES))); 
	countries.set_pos_to_write(DISTINCTS, file.get_key_value("distincts", countries.getContent(DISTINCTS))); 
	countries.set_pos_to_write(FOLDERS, file.get_key_value("folders", countries.getContent(FOLDERS)));

	mymenu.set(countries, file);

}

void DataBase::ShowMenu()
{
	mymenu.Start();
}
