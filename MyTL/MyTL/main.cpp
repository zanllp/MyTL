#include"Stack.h"
#include<list>
#include<vector>
#include <iostream>
#include<stack>
#include<string>
#include<Windows.h>
#include"Timer.h"
using namespace MyTL;
using namespace std;



int main(int argc, char ** argv)
{
	Timer timer;

	//stack test

	if (1)
	{
		Stack<int> s;
		for (int i = 0; i < 10000000; i++)
		{
			s.Push(i);
		}

		 auto c=s.Where([](int x){return x > 5000000; });
		 
	}
	timer.PrintRuntime();
	
	Stack<Stack<string>> stack;
	if (0)
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
	if (0)
	{
		//vector<string> s;
		Stack<string> s;
		for (int i = 0; i < 10000000; i++)
		{
			s.Push("hello world" + to_string(i));
			//s.push_back("hello world" + to_string(i));
		}

	}
	system("pause");
}