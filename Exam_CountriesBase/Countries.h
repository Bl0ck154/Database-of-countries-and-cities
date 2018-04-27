#pragma once
#include"defines.h"
using namespace std;

enum MapType // �������� ����, ������� �������� � ������ �����
{
	COUNTRIES,
	DISTINCTS,
	FOLDERS
};

class Countries
{
	Countries(); // ����������� ���������, ������ ������� ������ ����� � main'e
	friend class DataBase; // ������ � ������������� ������ DataBase
	struct // ������ � �� �����
	{
		str_map items; // ���� ����=�������� // countries { Ukraine=ukr; United States=us }
		size_t pos_to_write; // ������� �������� ������ � �����, ������������ ��� ���������� ������� � ����
	} countries, distincts, folders;// ����� ����� � �������� ��������// ����� ����� � �������� �����

public:
	void AddTo(MapType m, string key, string value); // ���������e ������ � ��������

	str_map& getContent(MapType); // ���������� ������ �� ���� ���������������� ����, ����� ������������ ��� ���������� ���� ����
	void set_pos_to_write(MapType, size_t); // ���������� ������� ��� ������
	size_t get_pos_to_write(MapType); // �������� ������� ��� ������
	void update_pos_to_write(MapType, string&); // ��������(��������) ������� ��� ������
};

