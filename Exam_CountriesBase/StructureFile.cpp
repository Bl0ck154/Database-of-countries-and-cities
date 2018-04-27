#include "StructureFile.h"
#include <io.h>


StructureFile::StructureFile()
{
	fname = BASE_DIR; // адрес к файлу (папка в которой он лежил)
	fname += MANIFEST_FILE; //добавляем к адресу имя файла
	file = nullptr; // указатель файла в nullptr, чем показываем что файл сначала закрыт
}

StructureFile::StructureFile(string fname)
{
	this->fname = fname;
	file = nullptr;
}


StructureFile::~StructureFile()
{
	close();
}

bool StructureFile::open()
{
	if(file) // если файл открыт
		return false; // то открывать еще раз мы его не будем и вернем false

	if (_access(fname.c_str(), 04) == -1) // проверяем файл на наличие и возможность прочитать
		throw exception("File not found or access denied");

	file = fopen(fname.c_str(), "r");
	return file ? true : false;
}

bool StructureFile::close()
{
	bool rtrn = false;
	if (file)
	{
		rtrn = fclose(file) ? false : true; // fclose возвращает 0 в случае успешного закрытия, в моем методе будет наоборот - true в случае успеха
		file = nullptr;
	}
	return rtrn;
}

bool StructureFile::read()
{
	if (!file) // если файл не открыт, то прочитать не сможем
		return false;
	if (!content.empty()) //если поле content не пусто, значит там что-то есть, чего быть не должно
		throw exception("string must be empty"); 

	char *tmp = new char[256]; // буфер для чтения строк из файла
	while (!feof(file)) //пока не достигнут конец файла
	{
		if(fgets(tmp, 256, file)) // если прочитали, то 
			content += tmp; // добавляем строку в общее поле content
	}
	delete[]tmp; // очищаем память от нашего char tmp
	return content.empty() ? false : true; // если по окончанию content не пуст, значит всё прошло успешно - true
}

bool StructureFile::open_and_read()
{
	int points = 0; // все 3 процедуры должны быть успешными, подсчитаем баллы
	points += open(); // открыем манифест
	points += read(); // читаем его
	points += close(); // закрываем
	return points == 3 ? true : false; // если все 3, то true
}

string& StructureFile::getContent()
{
	return content;
}

size_t find_by_template(const string &in, const string keyword, const char brace) // функцию находит открытые фигурные скобки на любом расстоянии, т.е. с любым количеством пробелов
{
	size_t pos = -1;
	do {
		pos = in.find(keyword, pos+1); // может быть случай что keyword состоит в другом слове, например, Spain=en, ключ en состоит в слове countries
	} while (in[pos - 1] != '\n' && pos != string::npos); // проверяем чтобы перед нашим ключевым словом обязательно был символ переноса строки

	pos += keyword.size();
	for (; pos - keyword.size() != string::npos; pos++)
	{
		if (in[pos] == ' ' || in[pos] == '\t' || in[pos] == '\n')
			continue;

		if (in[pos] == brace)
		{
			pos++;
			break;
		}
		else
			pos = in.find(keyword, pos + 1), pos += keyword.size();
	}
	return pos; // возвращает позицию после фигурной скобки
}

size_t StructureFile::get_key_value(string keyword, str_map& m)
{
//	string keyword = "countries"; // ключевое слово перед кавычками в файле манифест
	size_t pos = find_by_template(content, keyword,'{'); // ищем слово в нашем стринге файла
	if (pos == string::npos) // если слово не найдено
		throw exception("Not found keyword or structure error");

	bool eq = false;
	string tmp1, tmp2;
	for (size_t i = pos; i < content.size(); i++)
	{
		if (content[i] == '#')//если символ # значить строка - комментарий
			i = content.find("\n", i); // пропускаем всю строку

		if (content[i] == '\t' || content[i] == '\n' || content[i] == '}') // пропускаем пробелы
		{
			if (eq) // если у нас был уже найден знак равно, значит мы читали ключ=ЗНАЧЕНИЕ, а если нашли пробел, то заканчиваем чтение
			{
				m.insert(make_pair(tmp1, tmp2)); // записываем, то что прочитали в мап
				tmp1.clear(), tmp2.clear(); // обнуляем все переменные
				eq = false;
			}
			if (content[i] == '}') { pos = i; break; }
			continue;
		}

		if (content[i] == '=') // проверяем на знак равно
		{
			eq = true;
			continue;
		}
		else if (content[i] == '{') throw exception("manifest structure error");

		if (!eq) // если знака равно еще не было, то это левая сторона
			tmp1 += content[i];
		else // если был, то права
			tmp2 += content[i];
	}
	return pos;
}

bool StructureFile::insert(size_t pos, string what)
{
	FILE *wfile = fopen(fname.c_str(), "w"); // открываем файл для перезаписи
	content.insert(pos, (what + '\n')); // добавляем строку what в главную строку content
	bool rtrn = fputs(content.c_str(), wfile) >= 0 ? true : false; // делаем перезапись файла
	fclose(wfile); // закрываем файл
	return rtrn; // если перезапись прошла успешно, то вернется true
}

size_t StructureFile::size()
{
	return content.size();
}

size_t StructureFile::next_brace_pos(size_t pos)
{
	size_t t = content.find('}', pos);
	return t;
}
