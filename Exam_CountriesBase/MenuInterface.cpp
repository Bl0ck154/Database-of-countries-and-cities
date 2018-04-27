#include "MenuInterface.h"
#include <io.h>
#include <conio.h>
#include <direct.h>

MenuInterface::MenuInterface() : currPath(BASE_DIR) {}

void MenuInterface::set(Countries &countries, StructureFile &file) // ���� ���������� ������
{
	this->countries = &countries; 
	this->file = &file;
	set_default_path();
//	Start();//debug
}

void MenuInterface::Start()
{
	ShowMenu(); 
	KeyAction(); 
}

void MenuInterface::set_default_path()
{
	temp_map = &countries->getContent(COUNTRIES);
	activeIndex = temp_map->begin();
}

void MenuInterface::ReBuild(string path) // path = ukr
{
	if (inCountry.empty()) // ���� �� �� � ������, �� ������������ �� ������ �������� �� ��������
		set_default_path();
	else
	{
		_temp.items.clear(); // ������� �������� ���� �� ������� ��������
		if (path == ".." || path == "!") // ���� ���������� ������ �����, �� ���� - ".." ������������, ����� ��������� ���� ������������ (���� ! �������� ��� ����� ����������� ���� ����� ���������� ������ ��������)
			path = inRegion.empty() ? inCountry : inRegion; // � ������ ��� � ������
		_temp.pos_to_write = file->get_key_value(path, _temp.items); // ���������� ������� ��� ������ � ������ �� ����� � ��� �������� ���
		menu_items_count = _temp.items.size(); // ���������� ���-�� ��������� items
		temp_map = &_temp.items; // �������� ��������� ����������� ��� �������� ���
		activeIndex = temp_map->begin(); // ���������� �������� ������� ���� - ������ ������� �� ������
	}
}

template<typename T>T map_find_by_value(map<T,T>map, T value) // ������� ���� map.find() , ������ ���� �� ��������, � ���������� ����
{
	for (str_map::iterator it = map.begin(); it != map.end(); ++it)
		if (it->second == value)
			return it->first;
	return string();
}

void MenuInterface::ShowMenu(string path)
{
	if (!path.empty()) // ���� ���� ���������, ������������� ����
	{
		bool ignore = false;
		if (path == "..") // ������� �� ������� �����
		{
			if (!inRegion.empty()) // ����� ���� ��������. ���� ������ �� ������ (���� inRegion), ��... 
			{
				currPath.resize(currPath.size() - inRegion.size() - 1);; // �������� �� ������ ���� �������� ������� � ����
				inRegion.clear();// ������� inRegion, ��� ���������� ��� we're not in region
			}
			else if (!inCountry.empty())
			{
				currPath.resize(currPath.size() - inCountry.size() - 1);
				inCountry.clear();
			}
			else // ���� �� ������ �� � ����� �� ����������������� if'��, ������ �� � �������� ������� � ��� ������ ������������� ����
				ignore = true; // ���������� ���
		}
		else if(path != "!")
		{
			currPath += path + '\\'; // ��������� ��� ����
			if (inCountry.empty()) // ���� �� �� ����� � ������, ������ ���� inCountry �����
				inCountry = path; // ���������� ���� ������ ����
			else if (inRegion.empty()) // ���� ������ ����, ��� ����...
				inRegion = path;
		}
		
		if(!ignore)
			ReBuild(path); // ���������� ����� ���� �� �����
	}

	system("cls"); // ������� ������� ��� ������ ������
	string _message; // ������� ���������� ���������
	_message += "\n\t\t"; // ��������� ������� �� ����� �������
	if (inCountry.empty()) // ���� �� � ������� ����,
		_message += "Countries Data Base v0.1"; // �� ������� ������ ���������
	else
		_message += map_find_by_value(countries->getContent(COUNTRIES), inCountry) + ", " + countries->getContent(DISTINCTS).find(inCountry)->second + "s:"; // �������� ������ + �������� ��������(��������), � ������������� �����
	if (!inRegion.empty()) // ���� �� ������ �������
		_message.pop_back(), _message.pop_back(), _message += " - " + RegionTmp; // ������� 2 ������� "s:", ��������� - � �������� �������
	
	cout << _message << "\n\n"; // ����� ��������������� ���������

	string tmp; // ������������ ��� ������������ ����
	for (str_map::iterator it = temp_map->begin(); it != temp_map->end(); it++)
	{
		tmp = currPath + it->second; // �� ������ ���� ����� ��������� �� � ����� �������
		if (_access(tmp.c_str(), 04) == -1) // ��������� ���������� �� ����� � ����� �������� �������
			throw exception("Directory not found in your file system");

		cout << "\t" << (it == activeIndex ? "> " : "  ") << it->first << endl; // ���� ������ ���� ����� ����, ������� ��������� �������
	}

	cout << "\n\t Press F1 to add a new one\n";
}

void MenuInterface::next()
{
	activeIndex = activeIndex == --(temp_map->end()) ? temp_map->begin() : ++activeIndex; // �������� ��������� ������ ���� ����, ���� ��������� �� ���������� �� ������
}

void MenuInterface::prev()
{
	activeIndex = activeIndex == temp_map->begin() ? --(temp_map->end()) : --activeIndex; // �������� ��������� ������ ���� �����, ���� ������ �� ���������� �� ���������
}

void MenuInterface::enter_key()
{
	if (!(menu_items_count || (!(!inCountry.empty()) && !menu_items_count)))
		return;
	if (activeIndex->second.find('.') != string::npos) // ���� � �������� ���� �����, �� ��� ����
	{
		system(string(currPath + activeIndex->second).c_str()); // ��������� ���� � ��������� ��������� �������
		ShowMenu();
	}
	else
	{
		// ���� ���� ������, �� ������� ���� � ���������� ����� �� ����� �� ���������� �����, ���� �� ������ ������ ���� ����� �������� ����� ����
		RegionTmp = activeIndex->first;
		ShowMenu(inCountry.empty() ? countries->getContent(FOLDERS).find(activeIndex->second)->second : activeIndex->second);
	}
}

void MenuInterface::KeyAction()
{
	bool isWorking = true;
	while (isWorking)
	{
		bool _inCountry = (!inCountry.empty());
		switch (_getch())
		{
		case KEY_UP:
			if (menu_items_count || (!_inCountry && !menu_items_count)) // ���� ���-�� ������� ���� 0, �� ��������� �������, ��� �� ��������� ���� ��� ������� ����
				prev();
			ShowMenu();
			break;
		case KEY_DOWN:
			if (menu_items_count || (!_inCountry && !menu_items_count))
				next();
			ShowMenu();
			break;
		case KEY_EXIT:
			isWorking = false;
			break;
		case KEY_RIGHT: // ������
			enter_key();
			break;
		case KEY_ENTER:
			enter_key();
			break;
		case KEY_BACKSPACE: // �����
			if(_inCountry)
				ShowMenu("..");
			break;
		case KEY_LEFT: // �����
			if (_inCountry)
				ShowMenu("..");
			break;
		case KEY_F1: // ����� ����
			CreateNew();
			ShowMenu("!");// ! ��� ���������� ����
			break;
		}
	}
}

void MenuInterface::CreateNew()
{
	cout << "\n\n Enter name: ";
	string nname, nkey;
	getline(cin, nname); // ������ ��� ������ ������� 
	cout << "\n Enter key: ";
	getline(cin, nkey); // ������ ���� ������ ������� 
	bool dot_found = (nkey.find('.') != string::npos); // ���� ����� ������� ��������� �� �� ��� ����-����� �� ����� ����������
	if (!inRegion.empty() && !dot_found) // ������� ������� ����� � �������� �������� ��������
		nkey += ".txt", dot_found = true; // ���������� ���������� ��� �� �������

	while (!map_find_by_value(inCountry.empty() ? countries->getContent(COUNTRIES) : _temp.items, nkey).empty()) // ��������� �� ����� �� ������ ����
	{
		cout << "Error! This key alreary used\nTry another keyname: "; // ������ ������ ��� ���
		getline(cin, nkey);
	}

	if (dot_found)
	{
		FILE *newfile = fopen(string(currPath + nkey).c_str(), "w"); // ������� ����
		if (!newfile) // ���� ���-�� ����� �� ���
			throw exception("File creating error");
		fclose(newfile); // ��������� �������� ����
	}
	else
		mkdir(string(currPath + nkey + '\\').c_str()); // ������� �����

	if (inCountry.empty())
	{
		string _district, _folder;
		cout << "\n Enter district name: ";
		getline(cin, _district); // ������ ��� ������
		cout << "\n Enter folder name: ";
		getline(cin, _folder); // ������ ��� �����

		countries->getContent(FOLDERS).insert(make_pair(nkey, _folder)); // ��������� ����� ������ � ���� �����
		_folder = "\t" + nkey + "=" + _folder; // ��������� ������
		file->insert(countries->get_pos_to_write(FOLDERS), _folder); // ��������� ��� ������ � ����

		countries->getContent(DISTINCTS).insert(make_pair(nkey, _district)); // ��������� ����� ������ � ���� �����
		_district = "\t" + nkey + "=" + _district;
		file->insert(countries->get_pos_to_write(DISTINCTS), _district); // ��������� ��� ������ � ����

		countries->getContent(COUNTRIES).insert(make_pair(nname, nkey)); // ��������� ����� ������ � ���� �����
		nname = "\t" + nname + "=" + nkey;
		file->insert(countries->get_pos_to_write(COUNTRIES), nname); // ��������� ��� ������ � ����

		countries->set_pos_to_write(FOLDERS, file->next_brace_pos(countries->get_pos_to_write(FOLDERS))); // ��������� ������� ��� ������
		countries->set_pos_to_write(DISTINCTS, file->next_brace_pos(countries->get_pos_to_write(DISTINCTS)));
		countries->set_pos_to_write(COUNTRIES, file->next_brace_pos(countries->get_pos_to_write(COUNTRIES)));

		nkey += " {\n}"; // ��������� ������ � ������������ �� ���������� ����� ��������
		file->insert(file->size(), nkey);
	}
	else if (!inRegion.empty() || dot_found)
	{
		_temp.items.insert(make_pair(nname, nkey));
		file->insert(_temp.pos_to_write, ("\t" + nname + "=" + nkey));
		activeIndex = _temp.items.begin();
	}
	else/* if (!inCountry.empty())*/
	{
		_temp.items.insert(make_pair(nname, nkey));
		file->insert(_temp.pos_to_write, ("\t" + nname + "=" + nkey));
		nkey += " {\n}"; // ��������� ������ � ������������ �� ���������� ����� ��������
		file->insert(file->size(), nkey); // ��������� � ����� �����
		activeIndex = _temp.items.begin();
	}
	cout << "\nSuccess! ";
	system("pause");
}
