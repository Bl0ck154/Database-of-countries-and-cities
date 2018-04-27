#include "Header.h"

void main()
{

	try
	{
		DataBase base;
		base.ShowMenu();
	}
	catch (const exception&e)
	{
		cout << typeid(e).name() << " : " << e.what() << endl;
	}
	
}