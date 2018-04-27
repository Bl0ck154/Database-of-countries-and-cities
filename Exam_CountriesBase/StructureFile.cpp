#include "StructureFile.h"
#include <io.h>


StructureFile::StructureFile()
{
	fname = BASE_DIR; // ����� � ����� (����� � ������� �� �����)
	fname += MANIFEST_FILE; //��������� � ������ ��� �����
	file = nullptr; // ��������� ����� � nullptr, ��� ���������� ��� ���� ������� ������
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
	if(file) // ���� ���� ������
		return false; // �� ��������� ��� ��� �� ��� �� ����� � ������ false

	if (_access(fname.c_str(), 04) == -1) // ��������� ���� �� ������� � ����������� ���������
		throw exception("File not found or access denied");

	file = fopen(fname.c_str(), "r");
	return file ? true : false;
}

bool StructureFile::close()
{
	bool rtrn = false;
	if (file)
	{
		rtrn = fclose(file) ? false : true; // fclose ���������� 0 � ������ ��������� ��������, � ���� ������ ����� �������� - true � ������ ������
		file = nullptr;
	}
	return rtrn;
}

bool StructureFile::read()
{
	if (!file) // ���� ���� �� ������, �� ��������� �� ������
		return false;
	if (!content.empty()) //���� ���� content �� �����, ������ ��� ���-�� ����, ���� ���� �� ������
		throw exception("string must be empty"); 

	char *tmp = new char[256]; // ����� ��� ������ ����� �� �����
	while (!feof(file)) //���� �� ��������� ����� �����
	{
		if(fgets(tmp, 256, file)) // ���� ���������, �� 
			content += tmp; // ��������� ������ � ����� ���� content
	}
	delete[]tmp; // ������� ������ �� ������ char tmp
	return content.empty() ? false : true; // ���� �� ��������� content �� ����, ������ �� ������ ������� - true
}

bool StructureFile::open_and_read()
{
	int points = 0; // ��� 3 ��������� ������ ���� ���������, ���������� �����
	points += open(); // ������� ��������
	points += read(); // ������ ���
	points += close(); // ���������
	return points == 3 ? true : false; // ���� ��� 3, �� true
}

string& StructureFile::getContent()
{
	return content;
}

size_t find_by_template(const string &in, const string keyword, const char brace) // ������� ������� �������� �������� ������ �� ����� ����������, �.�. � ����� ����������� ��������
{
	size_t pos = -1;
	do {
		pos = in.find(keyword, pos+1); // ����� ���� ������ ��� keyword ������� � ������ �����, ��������, Spain=en, ���� en ������� � ����� countries
	} while (in[pos - 1] != '\n' && pos != string::npos); // ��������� ����� ����� ����� �������� ������ ����������� ��� ������ �������� ������

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
	return pos; // ���������� ������� ����� �������� ������
}

size_t StructureFile::get_key_value(string keyword, str_map& m)
{
//	string keyword = "countries"; // �������� ����� ����� ��������� � ����� ��������
	size_t pos = find_by_template(content, keyword,'{'); // ���� ����� � ����� ������� �����
	if (pos == string::npos) // ���� ����� �� �������
		throw exception("Not found keyword or structure error");

	bool eq = false;
	string tmp1, tmp2;
	for (size_t i = pos; i < content.size(); i++)
	{
		if (content[i] == '#')//���� ������ # ������� ������ - �����������
			i = content.find("\n", i); // ���������� ��� ������

		if (content[i] == '\t' || content[i] == '\n' || content[i] == '}') // ���������� �������
		{
			if (eq) // ���� � ��� ��� ��� ������ ���� �����, ������ �� ������ ����=��������, � ���� ����� ������, �� ����������� ������
			{
				m.insert(make_pair(tmp1, tmp2)); // ����������, �� ��� ��������� � ���
				tmp1.clear(), tmp2.clear(); // �������� ��� ����������
				eq = false;
			}
			if (content[i] == '}') { pos = i; break; }
			continue;
		}

		if (content[i] == '=') // ��������� �� ���� �����
		{
			eq = true;
			continue;
		}
		else if (content[i] == '{') throw exception("manifest structure error");

		if (!eq) // ���� ����� ����� ��� �� ����, �� ��� ����� �������
			tmp1 += content[i];
		else // ���� ���, �� �����
			tmp2 += content[i];
	}
	return pos;
}

bool StructureFile::insert(size_t pos, string what)
{
	FILE *wfile = fopen(fname.c_str(), "w"); // ��������� ���� ��� ����������
	content.insert(pos, (what + '\n')); // ��������� ������ what � ������� ������ content
	bool rtrn = fputs(content.c_str(), wfile) >= 0 ? true : false; // ������ ���������� �����
	fclose(wfile); // ��������� ����
	return rtrn; // ���� ���������� ������ �������, �� �������� true
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
