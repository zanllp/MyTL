#pragma once
#include<memory>
#include<iostream>
using namespace std;

namespace MyTL
{

	//保存数据的节点 双向链表
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
		T data;//数据
		T GetData()
		{	//这部分的检查只负责尾迭代器的检查，其他的可以直接GetIter()直接获取迭代器
			if (is_empty)//总感觉会降低很多性能
			{
				throw out_of_range("索引越界");
			}
			return data;
		}

		bool is_empty = false;//表示这个结点不包含数据，只有链接
		Node_sptr next;//下个节点位置
		Node_sptr last;//上个节点位置
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
		//给范围for用的
		bool operator!=(Iter_stack<T> diff) noexcept
		{
			//如果一个节点没有保存数据，我们直接认为它是空指针
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
		//隐式转换 感觉可能有歧义，
		//operator T() const
		//{
		//	return node_now->GetData();
		//}
		//解除引用
		T operator *() 
		{
			return node_now->GetData();
		}
		//左自加
		Iter operator++() 
		{
			node_now = node_now->next;
			return *this;
		}
		//右自加
		Iter operator++(int) noexcept
		{
			auto temp = node_now;
			node_now = node_now->next;
			return Iter_stack<T>(temp);
		}
		//左自减
		Iter operator--()
		{
			node_now = node_now->last;
			return *this;
		}
		//右自减
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
	class Stack//管理双向链表各个节点的类
	{
		using Iter = Iter_stack<T>;
		using Node_sptr = shared_ptr<Node_stack<T>>;
	public:
		Stack() noexcept
		{//实例一个节点作为持续的栈底和当前的栈顶，长度初始为1
			Node_sptr node = make_shared<Node_stack<T>>();
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
		//拷贝构造
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
		//压入栈
		void Push(T _data) noexcept
		{
			Node_sptr node = make_shared<Node_stack<T>>(_data, this->top);
			top->next = node;
			top = node;
			len++;
		}
		//弹出最后一个节点
		void Pop() noexcept 
		{
			Node_sptr node = top->last;
			top.reset();
			node->next.reset();
			top = node;
			len--;
		}
		//返回尾索引
		Iter end() noexcept
		{
			Node_sptr end_ = make_shared<Node_stack<T>>();
			end_->last = top;
			end_->is_empty = true;//最后一个数据为空只包含链接
			return Iter(end_);
		}
		//返回头迭代器
		Iter begin() noexcept
		{
			Iter begin_iter(down->next);
			return begin_iter;
		}
		//清空栈
		void clear()
		{
			while (len!=0)
			{
				Pop();
			}
		}
		//是否为空
		bool is_empty() noexcept
		{
			//如果栈底元素的下个位置为空则栈为空
			return  down->next == nullptr;
		}
		Iter GetIter(int index)
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
			return target;
		}
		//根据索引返回迭代器
		T& Index(int index)
		{
			return GetIter(index).node_now->data;
		}
		//下标索引
		T& operator[](int index)
		{
			return GetIter(index).node_now->data;
		}
		//pos插入到哪里，0头部1第二个位置，-1在尾部插入
		void Insert(int pos, Iter& iter) 
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
				Node_sptr node_target = GetIter(pos - 1);
				if (pos == len || pos == -1)//在尾部插入
				{
					this->Push(node->data);
				}
				else//在中间插入
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
		//插入节点 data 插入的数据，pos插入的位置
		void Insert(T data, int pos)
		{
			Node_sptr node = make_shared<Node_stack<T>>();
			node->data = data;
			this->Insert(pos, Iter<T>(node));
		}
		//移除某个节点
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
		//获取节点数量
		int GetLength() noexcept 
		{
			return this->len;
		}
		//复制构造
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
		//获取符合条件的数量   auto c=s.Count([](int x){return x > 50; });
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
		//选取符合条件的元素   auto c=s.Where([](int x){return x > 50; });
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
