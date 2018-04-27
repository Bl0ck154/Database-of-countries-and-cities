#include "MenuInterface.h"
#include <io.h>
#include <conio.h>
#include <direct.h>

MenuInterface::MenuInterface() : currPath(BASE_DIR) {}

void MenuInterface::set(Countries &countries, StructureFile &file) // сюда передаются ссылки
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
	if (inCountry.empty()) // если мы не в стране, то переключемся на первую страницу со странами
		set_default_path();
	else
	{
		_temp.items.clear(); // очищаем темповую мапу от прошлых значений
		if (path == ".." || path == "!") // если нажималась кнопка назад, то путь - ".." некорректный, нужно выставить куда возвращаемся (знак ! означает что нужно перегрузить меню после добавления нового элемента)
			path = inRegion.empty() ? inCountry : inRegion; // в страну или в регион
		_temp.pos_to_write = file->get_key_value(path, _temp.items); // запоминаем позицию для записи и читаем из файла в наш темповый мап
		menu_items_count = _temp.items.size(); // запоминаем кол-во найденных items
		temp_map = &_temp.items; // главному указателю присваиваем наш темповый мап
		activeIndex = temp_map->begin(); // запоминаем активный элемент меню - первый элемент по списку
	}
}

template<typename T>T map_find_by_value(map<T,T>map, T value) // функция типа map.find() , только ищет по значению, а возвращает ключ
{
	for (str_map::iterator it = map.begin(); it != map.end(); ++it)
		if (it->second == value)
			return it->first;
	return string();
}

void MenuInterface::ShowMenu(string path)
{
	if (!path.empty()) // если путь изменился, перестраиваем меню
	{
		bool ignore = false;
		if (path == "..") // переход на каталог вверх
		{
			if (!inRegion.empty()) // здесь идут проверки. Если регион не пустой (поле inRegion), то... 
			{
				currPath.resize(currPath.size() - inRegion.size() - 1);; // вырезаем из нашего пути название региона и слэш
				inRegion.clear();// очищаем inRegion, чем показываем что we're not in region
			}
			else if (!inCountry.empty())
			{
				currPath.resize(currPath.size() - inCountry.size() - 1);
				inCountry.clear();
			}
			else // если не попали ни в какой из вышеперечисленных if'ов, значит мы в корневом разделе и нет смысла перезагружать меню
				ignore = true; // запоминаем это
		}
		else if(path != "!")
		{
			currPath += path + '\\'; // обновляем наш путь
			if (inCountry.empty()) // если мы не вошли в страну, значит поле inCountry пусто
				inCountry = path; // записываем нашу страну туда
			else if (inRegion.empty()) // если регион пуст, так само...
				inRegion = path;
		}
		
		if(!ignore)
			ReBuild(path); // подгружаем новое меню из файла
	}

	system("cls"); // очищаем консоль при каждом выводе
	string _message; // сначала сформируем сообщение
	_message += "\n\t\t"; // некоторые отступы от краев консоли
	if (inCountry.empty()) // если мы в главном меню,
		_message += "Countries Data Base v0.1"; // то выводим просто заголовок
	else
		_message += map_find_by_value(countries->getContent(COUNTRIES), inCountry) + ", " + countries->getContent(DISTINCTS).find(inCountry)->second + "s:"; // название страны + название регионов(областей), в множественном числе
	if (!inRegion.empty()) // если мы внутри региона
		_message.pop_back(), _message.pop_back(), _message += " - " + RegionTmp; // удаляем 2 символа "s:", добавляем - и название региона
	
	cout << _message << "\n\n"; // вывод сформированного сообщения

	string tmp; // используется для формирования пути
	for (str_map::iterator it = temp_map->begin(); it != temp_map->end(); it++)
	{
		tmp = currPath + it->second; // на случай если папки находятся не в корне проекта
		if (_access(tmp.c_str(), 04) == -1) // проверяем существует ли папка в нашей файловой системе
			throw exception("Directory not found in your file system");

		cout << "\t" << (it == activeIndex ? "> " : "  ") << it->first << endl; // если выбран этот пункт меню, выводим стрелочку впереди
	}

	cout << "\n\t Press F1 to add a new one\n";
}

void MenuInterface::next()
{
	activeIndex = activeIndex == --(temp_map->end()) ? temp_map->begin() : ++activeIndex; // смещение активного пункта меню вниз, если последний то подымается на первый
}

void MenuInterface::prev()
{
	activeIndex = activeIndex == temp_map->begin() ? --(temp_map->end()) : --activeIndex; // смещение активного пункта меню вверх, если первый то спускается на последний
}

void MenuInterface::enter_key()
{
	if (!(menu_items_count || (!(!inCountry.empty()) && !menu_items_count)))
		return;
	if (activeIndex->second.find('.') != string::npos) // если в значении есть точка, то это файл
	{
		system(string(currPath + activeIndex->second).c_str()); // открываем файл в текстором редакторе виндоус
		ShowMenu();
	}
	else
	{
		// если путь пустой, то находим ключ в контейнере папок по ключу из контейнера стран, если не пустой значит ключ через активный пункт меню
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
			if (menu_items_count || (!_inCountry && !menu_items_count)) // если кол-во пунктов меню 0, то запретить листать, или же разрешить если это главное меню
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
		case KEY_RIGHT: // вперед
			enter_key();
			break;
		case KEY_ENTER:
			enter_key();
			break;
		case KEY_BACKSPACE: // назад
			if(_inCountry)
				ShowMenu("..");
			break;
		case KEY_LEFT: // назад
			if (_inCountry)
				ShowMenu("..");
			break;
		case KEY_F1: // новый файл
			CreateNew();
			ShowMenu("!");// ! для перегрузки меню
			break;
		}
	}
}

void MenuInterface::CreateNew()
{
	cout << "\n\n Enter name: ";
	string nname, nkey;
	getline(cin, nname); // вводим имя нового обьекта 
	cout << "\n Enter key: ";
	getline(cin, nkey); // вводим ключ нового обьекта 
	bool dot_found = (nkey.find('.') != string::npos); // ищем точку которая указывает на то что ключ-папка не имеет расширение
	if (!inRegion.empty() && !dot_found) // попытка создать папку в конечном каталоге регионов
		nkey += ".txt", dot_found = true; // дописываем расширение тхт по дефолту

	while (!map_find_by_value(inCountry.empty() ? countries->getContent(COUNTRIES) : _temp.items, nkey).empty()) // проверяем не занят ли данный ключ
	{
		cout << "Error! This key alreary used\nTry another keyname: "; // просим ввести еще раз
		getline(cin, nkey);
	}

	if (dot_found)
	{
		FILE *newfile = fopen(string(currPath + nkey).c_str(), "w"); // создаем файл
		if (!newfile) // если что-то пошло не так
			throw exception("File creating error");
		fclose(newfile); // закрываем созданый файл
	}
	else
		mkdir(string(currPath + nkey + '\\').c_str()); // создаем папку

	if (inCountry.empty())
	{
		string _district, _folder;
		cout << "\n Enter district name: ";
		getline(cin, _district); // вводим имя района
		cout << "\n Enter folder name: ";
		getline(cin, _folder); // вводим имя папки

		countries->getContent(FOLDERS).insert(make_pair(nkey, _folder)); // добавляем новую запись в мапу стран
		_folder = "\t" + nkey + "=" + _folder; // формируем строку
		file->insert(countries->get_pos_to_write(FOLDERS), _folder); // добавляем эту запись в файл

		countries->getContent(DISTINCTS).insert(make_pair(nkey, _district)); // добавляем новую запись в мапу стран
		_district = "\t" + nkey + "=" + _district;
		file->insert(countries->get_pos_to_write(DISTINCTS), _district); // добавляем эту запись в файл

		countries->getContent(COUNTRIES).insert(make_pair(nname, nkey)); // добавляем новую запись в мапу стран
		nname = "\t" + nname + "=" + nkey;
		file->insert(countries->get_pos_to_write(COUNTRIES), nname); // добавляем эту запись в файл

		countries->set_pos_to_write(FOLDERS, file->next_brace_pos(countries->get_pos_to_write(FOLDERS))); // обновляем позиции для записи
		countries->set_pos_to_write(DISTINCTS, file->next_brace_pos(countries->get_pos_to_write(DISTINCTS)));
		countries->set_pos_to_write(COUNTRIES, file->next_brace_pos(countries->get_pos_to_write(COUNTRIES)));

		nkey += " {\n}"; // добавляем скобки в соответствии со структурой файла манифест
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
		nkey += " {\n}"; // добавляем скобки в соответствии со структурой файла манифест
		file->insert(file->size(), nkey); // добавляем в конец файла
		activeIndex = _temp.items.begin();
	}
	cout << "\nSuccess! ";
	system("pause");
}
