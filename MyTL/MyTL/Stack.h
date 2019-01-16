#pragma once
#include<memory>
#include<string>
#include<iostream>
using namespace std;

namespace MyTL
{

	//保存数据的节点 双向链表
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
		T data;//数据
		T GetData()
		{
			if (is_empty)//总感觉会降低很多性能
			{
				throw out_of_range("索引越界");
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
		Iter(Node_sptr node) noexcept
		{
			node_now = node;
		}
		Iter()
		{
			node_now = nullptr;
		}
		//给范围for用的
		bool operator!=(Iter<T> diff) noexcept
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
		Iter<T> operator++(int) noexcept
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
	class Stack//管理双向链表各个节点的类
	{
		using Node_sptr = shared_ptr<Node<T>>;
	public:
		Stack() noexcept
		{//实例一个节点作为持续的栈底和当前的栈顶，长度初始为1
			Node_sptr node = make_shared<Node<T>>();
			this->down = node;
			this->top = node;
			len = 0;
		}
		~Stack()
		{
			auto temp = this->begin().node_now;
			for (int i = 0; i < len; i++)
			{//3个引用都需要解除
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
			end_->is_empty = true;//最后一个数据为空只包含链接
			return Iter<T>(end_);
		}
		Iter<T> begin() noexcept
		{
			Iter<T> begin_iter(down->next);
			return begin_iter;
		}
		bool Empty() noexcept
		{
			//如果栈底元素的下个位置为空则栈为空
			return  down->next == nullptr;
		}
		Iter<T> Index(int index)
		{
			if (index < 0 && index != 0)//小于0变成从后面倒数的第几个，例如-1就是最后一个
			{
				index = this->len + index;
			}
			if (index >= len)
			{
				throw out_of_range("索引越界");; //如果是以返回的形式开发者不容易察觉，尽管我不喜欢在c++中使用异常
			}
			Node_sptr target;
			if (index > len / 2)//索引靠后
			{
				target = end().node_now->last;
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
		Iter<T> operator[](int index)
		{
			return this->Index(index);
		}
		//pos插入到哪里，0头部1第二个位置，-1在尾部插入
		void Insert(int pos, Iter<T>& iter) 
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
		int len;//链表长度
		Node_sptr down;//栈底
		Node_sptr top;//栈顶

	};






	//UPDATE:
	//	raw指针改为智能指针
	//	支持范围for
	//	增加迭代器
	//  以及其它一堆
	//TODO:
	//
}
