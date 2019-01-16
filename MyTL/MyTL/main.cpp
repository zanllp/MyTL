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

	//stack test
	Stack<Stack<string>> stack;
	if (1)
	{
		for (int i = 0; i < 20; i++)
		{
			Stack<string> stack_a;
			for (size_t i = 0; i < 100; i++)
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
	if (true)
	{
		Stack<string> s;
		for (int i = 0; i < 10000000; i++)
		{
			s.Push("hello world" + to_string(i));
		}
		int len = s.GetLength();
		for (int i = 0; i < len; i++)
		{
			s.Pop();
		}
		
	}
	system("pause");
}