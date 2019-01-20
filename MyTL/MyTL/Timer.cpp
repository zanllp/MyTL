#include"Timer.h"
#include <time.h>
#include <iostream>
namespace MyTL
{
	string Timer::time_unit[4] = { "毫秒","秒","分钟","小时" };
	Timer::Timer()
	{
		time = clock();
	}
	Timer::~Timer()//就算什么都没写也有要有，不然出错
	{
	}
	//重置计时
	void Timer::Reset()
	{
		time = clock();
	}
	void Timer::PrintRuntime()
	{
		cout << RunTime() << endl;
		Reset();
	}
	//返回代码块的运行时间
	string Timer::RunTime()
	{

		time = clock() - time;
		int time_h = int(time) / 3600000;
		int time_m = int(time - time_h * 3600000) / 60000;
		int time_s = int(time - time_h * 3600000 - time_m * 60000) / 1000;
		int time_ms = int(time) % 1000;
		string runtime;
		if (time_h > 0)
		{
			runtime = runtime + to_string(time_h) + time_unit[3];
		}
		if (time_m > 0)
		{
			runtime = runtime + to_string(time_m) + time_unit[2];
		}
		if (time_s > 0)
		{
			runtime = runtime + to_string(time_s) + time_unit[1];
		}
		runtime = runtime + to_string(time_ms) + time_unit[0];
		return runtime;
	}



}
