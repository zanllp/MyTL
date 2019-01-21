#pragma once
#include <string>
#include <time.h>
#include<iostream>

using namespace std;
namespace MyTL
{
	class Timer//计时器
	{
	public:
		//构造函数
		Timer() 
		{
			time = clock();
		}
		//析构函数
		~Timer()=default;
		//返回代码块的运行时间
		string RunTime()
		{
			time = clock() - time;
			int time_h = int(time) / 3600000;
			int time_m = int(time - time_h * 3600000) / 60000;
			int time_s = int(time - time_h * 3600000 - time_m * 60000) / 1000;
			int time_ms = int(time) % 1000;
			string runtime;
			if (time_h > 0)
			{
				runtime = runtime + to_string(time_h) + "小时";
			}
			if (time_m > 0)
			{
				runtime = runtime + to_string(time_m) + "分钟";
			}
			if (time_s > 0)
			{
				runtime = runtime + to_string(time_s) + "秒";
			}
			runtime = runtime + to_string(time_ms) + "毫秒";
			return runtime;
		}
		//重置计时
		void Reset()
		{
			time = clock();
		}
		//直接输出运行时间并重置计时
		void PrintRuntime()
		{
			cout << RunTime() << endl;
			Reset();
		}
		double time;//运行了多少ms
	};
}
