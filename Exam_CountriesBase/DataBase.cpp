#include "DataBase.h"

DataBase::DataBase()
{
	if (!file.open_and_read()) // ������� �������� � ������ ���
		throw exception("File opening or reading error"); // ���� �������� false, �� ������� exception

	// file.get_key_value ��������� �� ����� ����=�������� ������� ������ � �������. ����� �� ���������� ������� countries.set_pos_to_write ���������� �������, ��� ���������� ������
	countries.set_pos_to_write(COUNTRIES, file.get_key_value("countries", countries.getContent(COUNTRIES))); 
	countries.set_pos_to_write(DISTINCTS, file.get_key_value("distincts", countries.getContent(DISTINCTS))); 
	countries.set_pos_to_write(FOLDERS, file.get_key_value("folders", countries.getContent(FOLDERS)));

	mymenu.set(countries, file);

}

void DataBase::ShowMenu()
{
	mymenu.Start();
}
