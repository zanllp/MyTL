#pragma once
#include<memory>
#include<iostream>
using namespace std;

namespace MyTL
{

	//�������ݵĽڵ� ˫������
	template<typename T>
	struct Node_stack
	{
	public:
		using Node_sptr = shared_ptr<Node_stack<T>>;
		Node_stack() = default;
		Node_stack(T _DATA,Node_sptr _LAST)
		{
			data = _DATA;
			last = _LAST;
		}
		T data;//����
		T GetData()
		{	//�ⲿ�ֵļ��ֻ����β�������ļ�飬�����Ŀ���ֱ��GetIter()ֱ�ӻ�ȡ������
			if (is_empty)//�ܸо��ή�ͺܶ�����
			{
				throw out_of_range("����Խ��");
			}
			return data;
		}

		bool is_empty = false;//��ʾ�����㲻�������ݣ�ֻ������
		Node_sptr next;//�¸��ڵ�λ��
		Node_sptr last;//�ϸ��ڵ�λ��
	};


	template<typename T>
	class Iter_stack
	{
	public:
		using Node_sptr = shared_ptr<Node_stack<T>>;
		using Iter = Iter_stack<T>;
		Node_sptr node_now;
		Iter_stack(Node_sptr node) noexcept
		{
			node_now = node;
		}
		Iter_stack()
		{
			node_now = nullptr;
		}
		//����Χfor�õ�
		bool operator!=(Iter_stack<T> diff) noexcept
		{
			//���һ���ڵ�û�б������ݣ�����ֱ����Ϊ���ǿ�ָ��
			if ((this->node_now == nullptr || this->node_now->is_empty)&&
				(diff.node_now->is_empty || diff.node_now == nullptr))
			{
				return false;
			}
			if (this->node_now != diff.node_now)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//��ʽת�� �о����������壬
		//operator T() const
		//{
		//	return node_now->GetData();
		//}
		//�������
		T operator *() 
		{
			return node_now->GetData();
		}
		//���Լ�
		Iter operator++() 
		{
			node_now = node_now->next;
			return *this;
		}
		//���Լ�
		Iter operator++(int) noexcept
		{
			auto temp = node_now;
			node_now = node_now->next;
			return Iter_stack<T>(temp);
		}
		//���Լ�
		Iter operator--()
		{
			node_now = node_now->last;
			return *this;
		}
		//���Լ�
		Iter operator--(int) noexcept
		{
			auto temp = node_now;
			node_now = node_now->last;
			return Iter_stack<T>(temp);
		}
		//
		Iter& operator = (T src) noexcept
		{
			node_now->data = src;
			return *this;
		}
		//
		~Iter_stack()
		{
			//node_now.reset();
		}
	};


	template<typename T, typename ...args>
	class Stack//����˫����������ڵ����
	{
		using Iter = Iter_stack<T>;
		using Node_sptr = shared_ptr<Node_stack<T>>;
	public:
		Stack() noexcept
		{//ʵ��һ���ڵ���Ϊ������ջ�׺͵�ǰ��ջ�������ȳ�ʼΪ1
			Node_sptr node = make_shared<Node_stack<T>>();
			this->down = node;
			this->top = node;
			len = 0;
		}
		~Stack()
		{
			auto temp = this->begin().node_now;
			for (int i = 0; i < len; i++)
			{//3�����ö���Ҫ���
				auto tmp = temp->next;
				temp->next.reset();
				temp->last.reset();
				temp = tmp;
			}
		}
		//��������
		Stack(const Stack<T> &src) noexcept
		{
			Node_sptr node = make_shared<Node_stack<T>>();
			down = node;
			top = node;
			len = 0;
			for (auto x : const_cast<Stack<T>&> (src))
			{
				this->Push(x);
			}
		}
		//ѹ��ջ
		void Push(T _data) noexcept
		{
			Node_sptr node = make_shared<Node_stack<T>>(_data, this->top);
			top->next = node;
			top = node;
			len++;
		}
		//�������һ���ڵ�
		void Pop() noexcept 
		{
			Node_sptr node = top->last;
			top.reset();
			node->next.reset();
			top = node;
			len--;
		}
		//����β����
		Iter end() noexcept
		{
			Node_sptr end_ = make_shared<Node_stack<T>>();
			end_->last = top;
			end_->is_empty = true;//���һ������Ϊ��ֻ��������
			return Iter(end_);
		}
		//����ͷ������
		Iter begin() noexcept
		{
			Iter begin_iter(down->next);
			return begin_iter;
		}
		//���ջ
		void clear()
		{
			while (len!=0)
			{
				Pop();
			}
		}
		//�Ƿ�Ϊ��
		bool is_empty() noexcept
		{
			//���ջ��Ԫ�ص��¸�λ��Ϊ����ջΪ��
			return  down->next == nullptr;
		}
		Iter GetIter(int index)
		{
			if (index < 0 && index != 0)//С��0��ɴӺ��浹���ĵڼ���������-1�������һ��
			{
				index = this->len + index;
			}
			if (index >= len)
			{
				throw out_of_range("����Խ��");; //������Է��ص���ʽ�����߲����ײ���������Ҳ�ϲ����c++��ʹ���쳣
			}
			Node_sptr target;
			if (index > len / 2)//��������
			{
				target = end().node_now->last;
				for (int i = 0; i < len - index - 1; i++)//����
				{
					target = target->last;
				}
			}
			else//������ǰ
			{
				target = begin().node_now;
				for (int i = 0; i < index; i++)
				{
					target = target->next;
				}
			}
			return target;
		}
		//�����������ص�����
		T& Index(int index)
		{
			return GetIter(index).node_now->data;
		}
		//�±�����
		T& operator[](int index)
		{
			return GetIter(index).node_now->data;
		}
		//pos���뵽���0ͷ��1�ڶ���λ�ã�-1��β������
		void Insert(int pos, Iter& iter) 
		{
			Node_sptr node = iter.node_now;
			if (pos == 0)//��ͷ������
			{
				Node_sptr node_target = this->down;
				if (len == 0)
				{
					this->Push(node->data);
				}
				else
				{
					Node_sptr node_target_next = this->begin();
					node->last = node_target;
					node_target->next = node;
					node->next = node_target_next;
					node_target_next->last = node;
					this->len++;
				}
			}
			else
			{
				Node_sptr node_target = GetIter(pos - 1);
				if (pos == len || pos == -1)//��β������
				{
					this->Push(node->data);
				}
				else//���м����
				{
					Node_sptr node_target_next = GetIter(pos);
					node_target->next = node;
					node->last = node_target;
					node_target_next->last = node;
					node->next = node_target_next;
					this->len++;
				}
			}
		}
		//����ڵ� data ��������ݣ�pos�����λ��
		void Insert(T data, int pos)
		{
			Node_sptr node = make_shared<Node_stack<T>>();
			node->data = data;
			this->Insert(pos, Iter<T>(node));
		}
		//�Ƴ�ĳ���ڵ�
		void earse(int pos)
		{
			if (pos == -1 || pos == len - 1)
			{
				Pop();
				return;
			}
			Node_sptr node = GetIter(0).node_now;
			node->last->next = node->next;
			node->next->last = node->last;
			node.reset();
			this->len--;
		}
		//��ȡ�ڵ�����
		int GetLength() noexcept 
		{
			return this->len;
		}
		//���ƹ���
		Stack<T>& operator=(const Stack<T> &src) noexcept 
		{
			Node_sptr node = make_shared<Node_stack<T>>();
			down = node;
			top = node;
			len = 0;
			for (auto x : const_cast<Stack<T>&>(src))
			{
				this->Push(x);
			}
			return *this;
		}
		//��ȡ��������������   auto c=s.Count([](int x){return x > 50; });
		int Count( bool(*c)(T)) noexcept
		{
			int count = 0;
			for (auto x : *this)
			{
				if (c(x))
				{
					count++;
				}
			}
			return count;
		}
		//ѡȡ����������Ԫ��   auto c=s.Where([](int x){return x > 50; });
		Stack<T> Where(bool(*c)(T)) noexcept
		{
			Stack<T> select;
			for (auto x : *this)
			{
				if (c(x))
				{
					select.Push(x);
				}
			}
			return select;
		}

		

	private:
		int len;//������
		Node_sptr down;//ջ��
		Node_sptr top;//ջ��

	};






	//UPDATE:
	//	rawָ���Ϊ����ָ��
	//	֧�ַ�Χfor
	//	���ӵ�����
	//  �Լ�����һ��
	//TODO:
	//
}
