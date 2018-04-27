#pragma once
#include <stdio.h> // ��������� ���������� ��� ������ � �������
#include"defines.h"
using namespace std;
#define MANIFEST_FILE "manifest.txt"

class StructureFile
{
	string fname; // ��� ����� � ������� ��������
	FILE *file; // ������ ��������� �����
	string content; // ���������� �����
public:
	StructureFile(); // �����������
	StructureFile(string fname); // ���������� � ������ �����, ���� �������� ���������� �� �������
	~StructureFile(); // ���������� ��������� �������� ����
	bool open(); // ��������� ����
	bool close(); // ��������� ����
	bool read(); // ��������� ���� � ��� string content
	bool open_and_read(); // ����� ���������� 3 ���������� ������ � ����, ��� ��������
	string& getContent(); // ���������� ���������� ����� � string
	size_t get_key_value(string keyword, str_map&m); // keyword - ����� ����� ��������� � ����� ��������, map - ����=��������, ���������� ������� ���������
	bool insert(size_t pos, string what); // ���������� ������ � ���� �� ������� pos
	size_t size(); //���������� ������ ���� content
	size_t next_brace_pos(size_t pos); // ������������ ��� ���������� ������� ��� ������ pos_to_write
};
