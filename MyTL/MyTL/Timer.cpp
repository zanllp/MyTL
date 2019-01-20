#include"Timer.h"
#include <time.h>
#include <iostream>
namespace MyTL
{
	string Timer::time_unit[4] = { "����","��","����","Сʱ" };
	Timer::Timer()
	{
		time = clock();
	}
	Timer::~Timer()//����ʲô��ûдҲ��Ҫ�У���Ȼ����
	{
	}
	//���ü�ʱ
	void Timer::Reset()
	{
		time = clock();
	}
	void Timer::PrintRuntime()
	{
		cout << RunTime() << endl;
		Reset();
	}
	//���ش���������ʱ��
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
