#include"Stack.h"
#include<list>
#include<vector>
#include <iostream>
#include<stack>
#include<string>
#include<Windows.h>
using namespace MyTL;
using namespace std;

template<typename T> 
int Count(Stack<T> &a,bool (*c)(T))
{
	int count = 0;
	for (auto x:a)
	{
		if (c(x))
		{
			count++;
		}
	}
	return count;
}

int main(int argc, char ** argv)
{

	//stack test

	if (1)
	{
		Stack<int> s;
		for (int i = 0; i < 100; i++)
		{
			s.Push(i);
		}

		auto c=Count<int>(s, [](int x) {return x > 50; });
		cout << c << endl;
	}


	Stack<Stack<string>> stack;
	if (1)
	{
		for (int i = 0; i < 5; i++)
		{
			Stack<string> stack_a;
			for (size_t i = 0; i < 10; i++)
			{
				stack_a.Push("hello world" + to_string(i));
			}
			stack.Push(stack_a);
		}
		
	}
	for (auto a:stack)
	{
		for (auto x : a)
		{
			cout << x << endl;
		}
	}
	if (1)
	{
		Stack<string> s;
		for (int i = 0; i < 10000000; i++)
		{
			s.Push("hello world" + to_string(i));
		}

	}
	system("pause");
}