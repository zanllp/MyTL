#pragma once
#include<memory>
#include<string>
#include<iostream>
using namespace std;

namespace MyTL
{

	//�������ݵĽڵ� ˫������
	template<typename T>
	struct Node
	{
		 template <typename T> friend class Iter;
		 template <typename T> friend class Stack;
	public:
		using Node_sptr = shared_ptr<Node<T>>;
		Node() = default;
		Node(T _DATA,Node_sptr _LAST)
		{
			data = _DATA;
			last = _LAST;
		}
		T data;//����
		T GetData()
		{
			if (is_empty)//�ܸо��ή�ͺܶ�����
			{
				throw out_of_range("����Խ��");
			}
			return data;
		}

		bool is_empty = false;//��ʾ�����㲻�������ݣ�ֻ������
	private:
		Node_sptr next;//�¸��ڵ�λ��
		Node_sptr last;//�ϸ��ڵ�λ��
	};


	template<typename T>
	class Iter
	{
	public:
		using Node_sptr = shared_ptr<Node<T>>;
		Node_sptr node_now;
		Iter(Node_sptr node) noexcept
		{
			node_now = node;
		}
		Iter()
		{
			node_now = nullptr;
		}
		//����Χfor�õ�
		bool operator!=(Iter<T> diff) noexcept
		{
			if (this->node_now == diff.node_now)
			{
				return true;
			}
			//���һ���ڵ�û�б������ݣ�����ֱ����Ϊ���ǿ�ָ��
			if ((this->node_now == nullptr || this->node_now->is_empty)&&
				(diff.node_now->is_empty || diff.node_now == nullptr))
			{
				return false;
			}
			return true;
		}
		//�������
		T operator *() 
		{
			return node_now->GetData();
		}
		//���Լ�
		Iter<T> operator++() 
		{
			node_now = node_now->next;
			return *this;
		}
		//���Լ�
		Iter<T> operator++(int) noexcept
		{
			auto temp = node_now;
			node_now = node_now->next;
			return Iter<T>(temp);
		}
		//���Լ�
		Iter<T> operator--()
		{
			node_now = node_now->last;
			return *this;
		}
		//���Լ�
		Iter<T> operator--(int) noexcept
		{
			auto temp = node_now;
			node_now = node_now->last;
			return Iter<T>(temp);
		}
		~Iter()
		{
			node_now.reset();
		}
	};

	template<typename T>
	class Stack//����˫����������ڵ����
	{
		using Node_sptr = shared_ptr<Node<T>>;
	public:
		Stack() noexcept
		{//ʵ��һ���ڵ���Ϊ������ջ�׺͵�ǰ��ջ�������ȳ�ʼΪ1
			Node_sptr node = make_shared<Node<T>>();
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
		Stack(const Stack<T> &src) noexcept
		{
			Node_sptr node = make_shared<Node<T>>();
			down = node;
			top = node;
			len = 0;
			for (auto x : const_cast<Stack<T>&> (src))
			{
				this->Push(x);
			}
		}
		void Push(T _data) noexcept
		{
			Node_sptr node = make_shared<Node<T>>(_data, this->top);
			top->next = node;
			top = node;
			len++;
		}
		void Pop() noexcept 
		{
			Node_sptr node = top->last;
			top.reset();
			node->next.reset();
			top = node;
			len--;
		}
		Iter<T> end() noexcept
		{
			Node_sptr end_ = make_shared<Node<T>>();
			end_->last = top;
			end_->is_empty = true;//���һ������Ϊ��ֻ��������
			return Iter<T>(end_);
		}
		Iter<T> begin() noexcept
		{
			Iter<T> begin_iter(down->next);
			return begin_iter;
		}
		bool Empty() noexcept
		{
			//���ջ��Ԫ�ص��¸�λ��Ϊ����ջΪ��
			return  down->next == nullptr;
		}
		Iter<T> Index(int index)
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
			return Iter<T>(target);
		}
		Iter<T> operator[](int index)
		{
			return this->Index(index);
		}
		//pos���뵽���0ͷ��1�ڶ���λ�ã�-1��β������
		void Insert(int pos, Iter<T>& iter) 
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
				Node_sptr node_target = this->Index(pos - 1);
				if (pos == len || pos == -1)//��β������
				{
					this->Push(node->data);
				}
				else//���м����
				{
					Node_sptr node_target_next = this->Index(pos);
					node_target->next = node;
					node->last = node_target;
					node_target_next->last = node;
					node->next = node_target_next;
					this->len++;
				}
			}
		}
		void Insert(T data, int pos)
		{
			Node_sptr node = make_shared<Node<T>>();
			node->data = data;
			this->Insert(pos, Iter<T>(node));
		}
		void earse(int pos)
		{
			if (pos == -1 || pos == len - 1)
			{
				this->pop();
				return;
			}
			Node_sptr node = this->Index(pos);
			node->last->next = node->next;
			node->next->last = node->last;
			this->len--;
		}
		int GetLength() noexcept 
		{
			return this->len;
		}
		Stack<T>& operator=(const Stack<T> &src) noexcept 
		{
			Node_sptr node = make_shared<Node<T>>();
			down = node;
			top = node;
			len = 0;
			for (auto x : const_cast<Stack<T>&>(src))
			{
				this->Push(x);
			}
			return *this;
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
