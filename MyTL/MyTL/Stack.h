#pragma once
#include<memory>
#include<string>
#include<iostream>
using namespace std;
//保存数据的节点 双向链表
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
		T data;//数据
		T GetData()
		{
			if (is_empty)//总感觉会降低很多性能
			{
				throw exception();
			}
			return data;
		}

		bool is_empty = false;//表示这个结点不包含数据，只有链接
	private:
		Node_sptr next;//下个节点位置
		Node_sptr last;//上个节点位置
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
		//不等于给范围for用的
		bool operator!=(Iter<T> diff)
		{
			if (this->node_now == diff.node_now)
			{
				return true;
			}
			//如果一个节点没有保存数据，我们直接认为它是空指针
			if ((this->node_now == nullptr || this->node_now->is_empty)&&
				(diff.node_now->is_empty || diff.node_now == nullptr))
			{
				return false;
			}
			return true;
		}
		//隐式转换,与stl不同
		//operator T()
		//{
		//	return node_now->GetData();
		//}
		//解除引用
		T operator *()
		{
			return node_now->GetData();
		}
		//左自加
		Iter<T> operator++() 
		{
			node_now = node_now->next;
			return *this;
		}
		//右自加
		Iter<T> operator++(int)
		{
			auto temp = node_now;
			node_now = node_now->next;
			return Iter<T>(temp);
		}
		//左自减
		Iter<T> operator--()
		{
			node_now = node_now->last;
			return *this;
		}
		//右自减
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
	class Stack//管理双向链表各个节点的类
	{
		using Node_sptr = shared_ptr<Node<T>>;
	public:
		Stack();
		~Stack();
		Stack(const Stack<T> &src);
		void Push(T _data);//入栈
		void Pop();//出栈
		Iter<T> end();//栈顶
		Iter<T> begin();//栈底
		bool Empty();//栈是否为空
		Iter<T> Index(int index);
		Iter<T> operator[](int index);
		void Insert(int pos, Iter<T>& node);//pos插入到哪里，0头部1第二个位置，-1在尾部插入
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
		int len;//链表长度
		Node_sptr down;//栈底
		Node_sptr top;//栈顶

	};

	template<typename T>
	inline Stack<T>::Stack()
	{
		//实例一个节点作为持续的栈底和当前的栈顶，长度初始为1
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
		{//3个引用都需要解除
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
	

	//入栈
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
	//出栈
	template<typename T>
	inline void Stack<T>::Pop()
	{
		Node_sptr node = top;
		node->last->next = nullptr;
		top = node->last;
		len--;

	}

	//返回栈顶元素
	template<typename T>
	Iter<T> Stack<T>::end()
	{
		Node_sptr end_ = make_shared<Node<T>>();
		end_->last = top;
		end_->is_empty = true;//最后一个数据为空只包含链接
		return Iter<T>(end_);
	}
	//返回栈底元素
	template<typename T>
	Iter<T> Stack<T>::begin()
	{
		Iter<T> begin_iter(down->next);
		return begin_iter;
	}
	//是否为空栈
	template<typename T>
	inline bool Stack<T>::Empty()
	{
		//如果栈底元素的下个位置为空则栈为空
		return  down->next == nullptr;
	}
	//索引，成员访问
	template<typename T>
	Iter<T> Stack<T>::Index(int index)
	{
		if (index < 0 && index != 0)//小于0变成从后面倒数的第几个，例如-1就是最后一个
		{
			index = this->len + index;
		}
		if (index >= len)
		{
			throw exception(); //如果是以返回的形式开发者不容易察觉，尽管我不喜欢在c++中使用异常
		}
		Node_sptr target;
		if (index > len / 2)//索引靠后
		{
			target=end().node_now->last;
			for (int i = 0; i < len - index - 1; i++)//迭代
			{
				target = target->last;
			}
		}
		else//索引靠前
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
		if (pos == 0)//在头部插入
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
			if (pos == len || pos == -1)//在尾部插入
			{
				this->Push(node->data);
			}
			else//在中间插入
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
	//	下标运算符索引,支持类python的负向索引
	//	索引越界检查
	//	
	//TODO:
	//
}
