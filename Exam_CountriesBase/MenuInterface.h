#pragma once
#include <iostream>
#include "Countries.h"
#include "StructureFile.h"
#include <iterator>
#include"defines.h"
using namespace std;

#define KEY_UP 72		// ������� �����
#define KEY_RIGHT 77		// ������� ������
#define KEY_LEFT 75		// ������� �����
#define KEY_DOWN 80		// ������� ����
#define KEY_ENTER 13	// ������� � �����
#define KEY_EXIT 27		// �����
#define KEY_BACKSPACE 8		// �����
#define KEY_F1 59		// ����� ����

class MenuInterface
{
	Countries* countries;
	StructureFile* file;
	str_map*temp_map; // ��������� � �������� ����
	str_map::iterator activeIndex;		// ������� ������
	string inCountry, inRegion;
	string currPath; // ���� � ������ � ���������
	struct
	{
		str_map items;
		size_t pos_to_write;
	} _temp;
	string RegionTmp; // ���� ����� ��������� ������ �������� �������(�������)
	void enter_key(); // ����� ������� ������ ������ (enter) 
	void ShowMenu(string path = "");
	void set_default_path();
	void ReBuild(string path);
	void next(); // �������� ��������� ������ + 1
	void prev(); // �������� ��������� ������ - 1
	void KeyAction(); // ������ ����
	void CreateNew(); // ����� �������� �����/�����
	size_t menu_items_count;
public:
	MenuInterface();
	void set(Countries &countries, StructureFile &file);
	void Start();

};

