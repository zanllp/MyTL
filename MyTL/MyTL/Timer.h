#pragma once
#include <string>
namespace MyTL
{
	using namespace std;
	class Timer//��ʱ��
	{
	public:
		//���캯��
		Timer();
		//��������
		~Timer();
		//���ش���������ʱ��
		string RunTime();
		//���ü�ʱ
		void Reset();
		//ֱ���������ʱ�䲢���ü�ʱ
		void PrintRuntime();
	private:
		double time;//�����˶���ms
		static string time_unit[4];
	};
}
