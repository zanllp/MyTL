
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
	for (size_t i = 0; i < 10000; i++)
	{
		a.Insert(i, String::ToString(i)<<"#233333");
	}
	Stack<int> c;
	a.ForEach([&](int k, String v) {
		cout << k << " " << v << endl;
		c.Push(k);
	});
	for (auto i : c)
	{
		cout << i << endl;
	}
	system("pause");
}