#include"Stack.h"
#include<list>
#include<vector>
#include <iostream>
#include<stack>
#include<string>
#include<Windows.h>
#include"Timer.h"
#include"String.h"
using namespace MyTL;
using namespace std;



int main(int argc, char ** argv)
{

	
	cout << String::ToString(-99.99) << endl;


	Timer timer;
	for (double i =-100; i < 100; i+=0.01)
	{
		cout << String::ToString(i) << endl;
	}



	for (size_t i = 0; i < 10000000; i++)
	{
		String str = "hello world";
		str<<"    "<<to_string(i).c_str();
		cout << str <<"  "<<str.GetLength()<< endl;
	}
	timer.PrintRuntime();
	
//	cout << str<<"              1" << endl;

	Stack<int> c;
	c.Push(1);
	c[1] = 2;
	cout << typeid(c[0]).name()<<c[0] << "最后" << endl;
	vector<int> a;
	a.push_back(1);
	cout << typeid(a[0]).name() << endl;



	

	//stack test

	if (1)
	{
		Stack<int> s;
		for (int i = 0; i < 10000000; i++)
		{
			s.Push(i);
		}

		 auto c=s.Where([](int x){return x > 5000000; });
		 //c[0] = 1;
		 cout << &c[0]<<"最后" << endl;
		 
		 
		 for (auto x:c)
		 {
			// cout << x << endl;
		 }
		
		 
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