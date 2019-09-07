
#include <iostream>
#include"Timer.h"
#include "String.h"
#include "Stack.h"
#include "Map.h"
using namespace MyTL;
using namespace std;


int main(int argc, char ** argv)
{

	Map<int, String> a;
	for (size_t i = 0; i < 1000000; i++)
	{
		a.Insert(i, String::ToString(i));
	}
	cout << a.Search(2333) << endl;
	system("pause");
}