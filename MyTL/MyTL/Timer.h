#pragma once
#include <string>
#include <time.h>
#include<iostream>

using namespace std;
namespace MyTL
{
	class Timer//��ʱ��
	{
	public:
		//���캯��
		Timer() 
		{
			time = clock();
		}
		//��������
		~Timer()=default;
		//���ش���������ʱ��
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
				runtime = runtime + to_string(time_h) + "Сʱ";
			}
			if (time_m > 0)
			{
				runtime = runtime + to_string(time_m) + "����";
			}
			if (time_s > 0)
			{
				runtime = runtime + to_string(time_s) + "��";
			}
			runtime = runtime + to_string(time_ms) + "����";
			return runtime;
		}
		//���ü�ʱ
		void Reset()
		{
			time = clock();
		}
		//ֱ���������ʱ�䲢���ü�ʱ
		void PrintRuntime()
		{
			cout << RunTime() << endl;
			Reset();
		}
		double time;//�����˶���ms
	};
}
