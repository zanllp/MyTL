#pragma once
#include<math.h>
#include<iostream>
namespace MyTL
{


	class String
	{
	public:
		//src的长度

		static int GetLength(const char src[]) //1
		{
			if (src == nullptr)
			{
				return 0;
			}
			int len_0 = 0;
			while (src[len_0] != '\0')
			{
				len_0++;
			}
			return len_0;
		}

		//字面值转指针
		static char * ToCharPtr(const char _data[])  //1
		{
			//有奇怪的bug
			//return const_cast<char*>(_data);
			int len = GetLength(_data);
			char* a = new char[len + 1];//请求需要长度的内存空间
			memcpy(a, _data, len + 1);
			return a;
		}
		//转字符串 len浮点数时保留到小数点后几位
		static String ToString(double num,int len=2)
		{
			String result;
			//负号
			if (num<0)
			{
				num = -num;
				result = "-";
			}
			//大于0的部分
			int pos = 0;
			while (num >= pow(10, pos))//大于等于1
			{
				pos++;
			}
			num < 1 ? pos++ :pos;//小于1
			while (pos)
			{
				result += char((num / pow(10, pos - 1)) + '0');
				num = num - int((num / pow(10, pos - 1)))*pow(10, pos - 1);
				pos--;
			}
			//小数点及余下
			double temp_remain = num - int(num);
			if (temp_remain!=0)
			{
				result += ".";
				pos = -1;
				while (-pos!=len+1)
				{
					int next= temp_remain * pow(10, -pos);
					result+= char(next + '0');
					temp_remain -= next*pow(10,pos);
					pos--;
				}
			}
			return result.GetLength()?result:"0";
		}
		void init()
		{
			delete_this();
			data = new char{ '\0' };
		}
		String()  //1
		{
			init();
		}
		String(const char _data[]) //1
		{
			delete_this();
			data = ToCharPtr(_data);
		}
		String(const String &src)  //1
		{
			delete_this();
			data = ToCharPtr(src.data);
		}
		~String()
		{
			delete_this();
		}
		// 指针偏移型查找 从offset位置开始查找
		int IndexOf(const char dst[], int offset = 0) //1
		{
			return contrast(this->data + offset, ToCharPtr(dst));

		}
		//截取字符串 h开始位置 len截取字符串长度 
		String SubString(int len, int h = 0)
		{
			if (len < 0 || h < 0)
			{
				throw std::exception();
			}
			char* new_str = new char[len + 1];
			new_str[len] = '\0';
			memcpy(new_str, data + h, len);
			return String(new_str);
		}
		//获取自己长度
		int GetLength()  //1
		{
			return GetLength(data);
		}
		//在尾部添加字符串   
		void Append(const char src[]) //1
		{
			int len_right = GetLength(src);
			int len_this = GetLength();
			int len_temp = len_right + len_this;//计算新字符串长度
			char* temp = new char[len_temp + 1];
			temp[len_temp] = '\0';
			memcpy(temp, data, len_this);
			memcpy(temp + len_this, src, len_right);
			delete[] this->data;
			this->data = temp;
		}
		void operator +=(const char src[]) //1
		{
			this->Append(src);
		}
		void operator +=(char src) //1
		{
			auto src_ = new char[2];
			src_[0] = src;
			src_[1] = '\0';
			this->Append(src_);
			delete[] src_;
		}
		//流输入重载 类result << "("<< id << ","<< String::ToString( i->data.occurTimes)<< ") ";
		String& operator<<(const char src[]) //1
		{
			this->Append(src);
			return *this;
		}
		String& operator<<(double src) //1
		{
			this->Append(ToString(src));
			return *this;
		}
		bool operator ==(String obj)  //1
		{
			int pos = 0;
			int t_l = this->GetLength();
			while (true)
			{
				unsigned char t_v = data[pos];//this value
				unsigned char o_v = obj[pos];
				if (t_v != o_v)
				{
					break;
				}
				if (pos == t_l)
				{
					return true;
				}
				pos++;

			}
			return false;
		}
		bool operator <(String obj) //1
		{
			int pos = 0;
			unsigned char t_v;
			unsigned char o_v;
			while (true)
			{
				t_v = data[pos];//this value
				o_v = obj[pos];
				if (t_v == '\0' || t_v < o_v)
				{
					if (t_v == o_v)//字符串相等
					{
						return false;
					}
					else
					{
						return true;
					}
				}
				if (o_v == '\0' || t_v > o_v)
				{
					return false;
				}
				pos++;
			}
		}
		bool operator >(String obj) //1
		{
			int pos = 0;
			unsigned char t_v;
			unsigned char o_v;
			while (true)
			{
				t_v = data[pos];//this value
				o_v = obj[pos];
				if (t_v == '\0' || t_v < o_v)
				{
					return false;
				}
				if (o_v == '\0' || t_v > o_v)
				{
					return true;
				}
				pos++;
			}
		}
		//复制构造 String
		String& operator=(const String &src)  //1
		{
			memcpy(data, src.data, GetLength(src.data) + 1);
			return *this;
		}
		//复制构造 const char[]
		String& operator=(const char _data[]) //1
		{
			int len_right = GetLength(_data);
			int len_this = GetLength();
			int len_temp = len_right + len_this;//计算新字符串长度
			char* temp = new char[len_temp + 1];
			temp[len_temp] = '\0';
			memcpy(temp, data, len_this);
			memcpy(temp + len_this, _data, len_right);
			delete[] this->data;
			this->data = temp;
			return *this;
		}
		//加号重载
		String operator+(const char src[])  //1
		{
			int len_this = GetLength();
			int len_a = GetLength(src);
			int len_temp = len_a + len_this;//计算新字符串长度
			char* temp = new char[len_temp + 1];
			temp[len_temp] = '\0';
			memcpy(temp, data, len_this + 1);
			memcpy(temp + len_this, src, len_a + 1);
			return String(temp);
		}
		//隐式转换成字符指针，方便直接cout
		operator char*() const //1
		{
			return data;
		}
		char operator[](int pos) //1
		{
			return data[pos];
		}
	private:
		void delete_this()
		{
			if (data != nullptr)
			{
				delete[] data;
			}
			data = nullptr;
		}
		void copy(char* dst, const char* _src, int len)
		{
			auto src = ToCharPtr(_src);
			for (int i = 0; i < len; i++)
			{
				dst[i] = src[i];
			}
		}
		char* data = nullptr;//本体
		//索引
		int index = 0;
		//匹配字符串用
		int SearchNext(char * src, char * dst) //1
		{
			//返回1相同
			//0不同
			if (GetLength(dst) == 0)//目标剩余长度为0，查找结束
			{				//直到查询长度为0都是相同，一路返回1直到contrast
				return 1;
			}
			if (*src == *dst)
			{
				if (SearchNext(++src, ++dst))//递归确认下个字符是否相同
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			return 0;
		}
		///匹配字符串用
		int contrast(char * src, char * dst)  //1
		{
			index = 0;
			int len_c = GetLength(src);
			while (len_c - index != 0)
			{
				if (src[index] == dst[0])
				{
					if (SearchNext(src + index, dst))//s返回1则代表字符串相同，contras返回索引位置结束
					{
						return index;
					}
				}
				index++;
			}
			return -1;
		}
	};


}
