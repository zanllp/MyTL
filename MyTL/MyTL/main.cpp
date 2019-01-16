#include"Stack.h"
#include<list>
#include<vector>
#include <iostream>
#include<stack>
#include<string>
#include<Windows.h>
using namespace MyTL;
using namespace std;
int main(int argc, char ** argv)
{
	Stack<string> a;
	if (1)
	{
		Stack<string> c;
		for (size_t i = 0; i < 10000; i++)
		{
			c.Push("hello world"+to_string(i));
		}
		cout << c.begin().node_now.use_count() << endl;
		a=Stack<string>(c);
		a.print();
		cout << c.begin().node_now.use_count() << endl;

	}
	for (auto x:a)
	{
		cout << x << endl;
	}

	a.print();
	//cout << a.GetLength()<<*a[20] << endl;
	system("pause");
}