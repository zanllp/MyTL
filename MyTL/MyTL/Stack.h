#pragma once
#include<memory>
#include<string>
#include<iostream>
using namespace std;
//�������ݵĽڵ� ˫������
namespace MyTL
{

	
	template<typename T>
	struct Node
	{
		 template <typename T> friend class Iter;
		 template <typename T> friend class Stack;
	public:
		using Node_sptr = shared_ptr<Node<T>>;
		Node() = default;
		Node(T _DATA)
		{
			data = _DATA;
		}
		T data;//����
		T GetData()
		{
			if (is_empty)//�ܸо��ή�ͺܶ�����
			{
				throw exception();
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
		Iter(Node_sptr node)
		{
			node_now = node;
		}
		Iter()
		{
			node_now = nullptr;
		}
		//�����ڸ���Χfor�õ�
		bool operator!=(Iter<T> diff)
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
		//��ʽת��,��stl��ͬ
		//operator T()
		//{
		//	return node_now->GetData();
		//}
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
		Iter<T> operator++(int)
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
		Iter<T> operator--(int)
		{
			auto temp = node_now;
			node_now = node_now->last;
			return Iter<T>(temp);
		}
		~Iter()
		{
			//node_now.reset();
		}
	};

	template<typename T>
	class Stack//����˫����������ڵ����
	{
		using Node_sptr = shared_ptr<Node<T>>;
	public:
		Stack();
		~Stack();
		Stack(const Stack<T> &src);
		void Push(T _data);//��ջ
		void Pop();//��ջ
		Iter<T> end();//ջ��
		Iter<T> begin();//ջ��
		bool Empty();//ջ�Ƿ�Ϊ��
		Iter<T> Index(int index);
		Iter<T> operator[](int index);
		void Insert(int pos, Iter<T>& node);//pos���뵽���0ͷ��1�ڶ���λ�ã�-1��β������
		void Insert(T data, int pos);
		void Delete(int pos);
		int GetLength();
		void print()
		{
		
			for (auto i=begin().node_now;i!=nullptr;i=i->next)
			{
				cout << i.use_count() << endl;
			}
		}
	private:
		int len;//������
		Node_sptr down;//ջ��
		Node_sptr top;//ջ��

	};

	template<typename T>
	inline Stack<T>::Stack()
	{
		//ʵ��һ���ڵ���Ϊ������ջ�׺͵�ǰ��ջ�������ȳ�ʼΪ1
		Node_sptr node = make_shared<Node<T>>();
		this->down = node;
		this->top = node;
		len = 0;
	}

	template<typename T>
	 Stack<T>::~Stack()
	{
		auto temp = this->begin().node_now;
		for (int i = 0; i < len; i++)
		{//3�����ö���Ҫ���
			auto tmp = temp->next;
			temp->next.reset();
			temp->last.reset();
			temp=tmp;
		}
	}
	 
	template<typename T>
	inline Stack<T>::Stack(const Stack<T> &src)
	{
		Node_sptr node = make_shared<Node<T>>();
		this->down = node;
		this->top = node;
		len = 0;
		//for (auto x:const_cast<Stack<T>&> (src))
		//{
		//	this->Push(x);
		//	//cout <<"copy construct  "<< x << endl;
		//}
		for (size_t i = 0; i < 10000; i++)
		{
			Push("hello world" + to_string(i));
		}
		print();
		//cout << top.use_count() << endl;
		//cout << len << endl;
		//this->down = node;
		//len = src.len;
		//auto temp = src.begin().node_now;
		//node->next =temp;
		//for (size_t i = 0; i < len-1; i++)
		//{
		//	temp = temp->next;
		//	auto p_last = node;
		//	node->next = temp;
		//	node = node->next;
		//	node->last = temp;
		//	//cout << node.use_count() << endl;
		//}
	}
	

	//��ջ
	template<typename T>
	void Stack<T>::Push(T _data)
	{
		Node_sptr node = make_shared<Node<T>>(_data);
		Node_sptr node_top = this->top;
		this->top = node;
		node_top->next = node;
		node->last = node_top;
		len++;
	}
	//��ջ
	template<typename T>
	inline void Stack<T>::Pop()
	{
		Node_sptr node = top;
		node->last->next = nullptr;
		top = node->last;
		len--;

	}

	//����ջ��Ԫ��
	template<typename T>
	Iter<T> Stack<T>::end()
	{
		Node_sptr end_ = make_shared<Node<T>>();
		end_->last = top;
		end_->is_empty = true;//���һ������Ϊ��ֻ��������
		return Iter<T>(end_);
	}
	//����ջ��Ԫ��
	template<typename T>
	Iter<T> Stack<T>::begin()
	{
		Iter<T> begin_iter(down->next);
		return begin_iter;
	}
	//�Ƿ�Ϊ��ջ
	template<typename T>
	inline bool Stack<T>::Empty()
	{
		//���ջ��Ԫ�ص��¸�λ��Ϊ����ջΪ��
		return  down->next == nullptr;
	}
	//��������Ա����
	template<typename T>
	Iter<T> Stack<T>::Index(int index)
	{
		if (index < 0 && index != 0)//С��0��ɴӺ��浹���ĵڼ���������-1�������һ��
		{
			index = this->len + index;
		}
		if (index >= len)
		{
			throw exception(); //������Է��ص���ʽ�����߲����ײ���������Ҳ�ϲ����c++��ʹ���쳣
		}
		Node_sptr target;
		if (index > len / 2)//��������
		{
			target=end().node_now->last;
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

	template<typename T>
	Iter<T> Stack<T>::operator[](int index)
	{
		return this->Index(index);
	}

	template<typename T>
	inline void Stack<T>::Insert(int pos, Iter<T>& iter)
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

	template<typename T>
	inline void Stack<T>::Insert(T data, int pos)
	{
		Node_sptr node = make_shared<Node<T>>();
		node->data = data;
		this->Insert(pos, Iter<T>(node));
	}

	template<typename T>
	inline void Stack<T>::Delete(int pos)
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

	template<typename T>
	inline int Stack<T>::GetLength()
	{
		return len;
	}



	//UPDATE:
	//	�±����������,֧����python�ĸ�������
	//	����Խ����
	//	
	//TODO:
	//
}
