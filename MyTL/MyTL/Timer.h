#pragma once
#include <string>
namespace MyTL
{
	using namespace std;
	class Timer//计时器
	{
	public:
		//构造函数
		Timer();
		//析构函数
		~Timer();
		//返回代码块的运行时间
		string RunTime();
		//重置计时
		void Reset();
		//直接输出运行时间并重置计时
		void PrintRuntime();
	private:
		double time;//运行了多少ms
		static string time_unit[4];
	};
}
