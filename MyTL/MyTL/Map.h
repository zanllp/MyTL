#pragma once
#define  RED true
#define  BLACK false
//map使用红黑树作为数据结构
template<typename T0, typename T>
struct Node_map
{
	T0  key;
	T value;
	bool color = BLACK;
	Node_map<T0, T>* left = nullptr;
	Node_map<T0, T>* right = nullptr;
	Node_map<T0, T>* parent = nullptr;
	Node_map<T0, T>* uncle();
	Node_map<T0, T>* grandpar();
};

template<typename T0, typename T>
inline Node_map<T0, T>* Node_map<T0, T>::uncle()
{
	if (this->parent == this->grandpar()->left)
	{
		return this->grandpar()->right;
	}
	else
	{
		return this->grandpar()->left;
	}
	return nullptr;
}

template<typename T0, typename T>
inline Node_map<T0, T>* Node_map<T0, T>::grandpar()
{
	return this->parent->parent;
}

//性质:
//
//节点是红色或黑色。
//根是黑色。
//所有叶子都是黑色（叶子是NIL节点）。
//每个红色节点必须有两个黑色的子节点。（从每个叶子到根的所有路径上不能有两个连续的红色节点。）
//从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。


template<typename T0, typename T>
class Map
{
public:
	Map();
	~Map();
	void Insert(T0  key, T value);
	T Search(T0  key);
	Node_map<T0, T>* root_node;
	Node_map<T0, T>* Move(T0  key);//返回对应的结点指针
	void LeftRotate(Node_map<T0, T>* node);//左旋,待旋转的节点从右边上升为父节点。
	void RightRotate(Node_map<T0, T>* node);//右旋,待旋转的节点从左边上升到父节点。
	int num_s;//查询节点的对比次数

private:
	bool HaveUncleNode(Node_map<T0, T>* node);//时候存在叔叔结点
	bool UncleNodeNull(Node_map<T0, T>* node);//叔叔节点是空或者是黑色
	void Adjust(Node_map<T0, T>* node_target, Node_map<T0, T>* node_new);

};

template<typename T0, typename T>
inline Map<T0, T>::Map()
{
}

template<typename T0, typename T>
inline Map<T0, T>::~Map()
{
}

template<typename T0, typename T>
inline void Map<T0, T>::Insert(T0  key, T value)
{
	num_s = 0;
	Node_map<T0, T>* node_this = Move(key);
	if (node_this == nullptr)//最上一级节点不存在
	{
		Node_map<T0, T>* node_new = new Node_map<T0, T>;
		node_new->value = value;
		node_new->key = key;
		node_new->color = BLACK;//根节点是黑色的
		this->root_node = node_new;
		return;
	}
	auto node_this_key = node_this->key;
	if (key == node_this_key)//命中，修改旧值
	{
		node_this->value = value;
		return;
	}
	Node_map<T0, T>* node_new = new Node_map<T0, T>;
	node_new->value = value;
	node_new->key = key;
	node_new->color = RED;//插入的节点默认红色
	node_new->parent = node_this;
	if (key < node_this_key)
	{
		node_this->left = node_new;
	}
	else
	{
		node_this->right = node_new;
	}
	if (node_this != this->root_node&&node_this->color != BLACK)
	{
		this->Adjust(node_this, node_new);//把这个注释掉就变成BST
	}
	//cout << this->num_s << endl;
}

template<typename T0, typename T>
inline T Map<T0, T>::Search(T0  key)
{
	num_s = 0;
	Node_map<T0, T>* node_now = Move(key);
	auto node_now_key = node_now->key;
	if (key == node_now_key)
	{
		return node_now->value;
	}
	return T();
}

template<typename T0, typename T>
inline Node_map<T0, T>* Map<T0, T>::Move(T0  key)
{
	Node_map<T0, T>* node_now = this->root_node;
	while (node_now != nullptr)
	{
		num_s++;
		auto node_this_key = node_now->key;
		if (key < node_this_key)//小于走左边
		{
			if (node_now->left != nullptr)//这个节点的左边没东西了
			{
				node_now = node_now->left;
			}
			else
			{
				return node_now;//返回这个节点的位置
			}
		}
		if (node_this_key < key)
		{
			if (node_now->right != nullptr)
			{
				node_now = node_now->right;
			}
			else
			{
				return node_now;
			}
		}
		if (node_this_key == key)//等于
		{
			return node_now;
		}
	}
	return node_now;
}


template<typename T0, typename T>
inline void Map<T0, T>::Adjust(Node_map<T0, T>* node_target, Node_map<T0, T>* node_new)
{
	if (this->HaveUncleNode(node_new))//情形3
	{
		auto node_par = node_new->parent->parent;
		bool uncle_pos = node_par->left != node_new->parent;//1表示叔叔在祖父的左边
		bool uncle_color = uncle_pos ? node_par->left->color : node_par->right->color;
		while (uncle_color == RED)
		{
			node_par->left->color = BLACK;			//爸爸和叔叔设为黑色
			node_par->right->color = BLACK;
			node_par->color = RED;					//祖父设为红色
			node_new = node_par;					//祖父节点变为新节点，继续向上变色
			this->root_node->color = BLACK;			      //根节点只能是黑色
			if (!this->HaveUncleNode(node_new))		      //如果新节点没有叔叔退出
			{
				break;
			}
			node_par = node_new->parent->parent;
			uncle_pos = node_par->left
				!= node_new->parent;//1表示叔叔在祖父的左边
			uncle_color = uncle_pos ?
				node_par->left->color :
				node_par->right->color;
		}
	}
	node_target = node_new->parent;
	if (node_target == nullptr || node_target->parent == nullptr)
	{
		return;
	}
	if (node_target->color == RED && this->UncleNodeNull(node_new))
	{
		if (node_target == node_new->grandpar()->left)//情形5
		{
			if (node_new == node_target->right)
			{
				this->LeftRotate(node_target);
				node_new = node_new->left;
			}
			node_target = node_new->parent;
			if (node_new = node_target->left)//情形4
			{
				node_target->color = BLACK;
				node_new->grandpar()->color = RED;
				this->RightRotate(node_new->grandpar());
				this->root_node->color = BLACK;
			}
			return;
		}
		if (node_target == node_new->grandpar()->right)
		{
			if (node_new == node_target->left)
			{
				this->RightRotate(node_target);
				node_new = node_new->right;
			}
			node_target = node_new->parent;
			if (node_new == node_target->right)
			{

				node_target->color = BLACK;
				node_new->grandpar()->color = RED;
				this->LeftRotate(node_new->grandpar());
				this->root_node->color = BLACK;
			}
			return;
		}
	}
}



//
//
//			|									|
//			X			对X结点左旋				Y
//		   / \			——————>		   / \
//		  α  Y								  X   γ
//			 / \		<——————		 / \
//			β  γ		对Y结点右旋			α  β
//
//大写字母表示结点，希腊字母表示叶可能为空也能是即将插入的结点
//对于左右旋的操作，从字面不太好理解，可以想成XY结点之间存在一
//个固定的点，XY是一个杆。那么对X结点的左旋操作即受到一个向左的
//力，那么X节点向下，Y节点向上抬升。
//注意！传参的旋转节点为连接父级的节点。
//
template<typename T0, typename T>
inline void Map<T0, T>::LeftRotate(Node_map<T0, T>* x)
{
	//cout << "左旋" << endl;
	Node_map<T0, T>* parent = x->parent;
	Node_map<T0, T>* y = x->right;
	//y节点与父节点
	if (parent == nullptr)
	{
		this->root_node = y;
		this->root_node->parent = nullptr;
		this->root_node->color = BLACK;
	}
	else
	{
		y->parent = parent;
		if (y->key > parent->key)//操作的父结点改成指向y
		{
			parent->right = y;
		}
		else
		{
			parent->left = y;
		}
	}

	//X节点与β叶
	x->right = y->left;
	if (x->right != nullptr)
	{
		x->right->parent = x;
	}
	//X节点与Y节点
	y->left = x;
	x->parent = y;
}

template<typename T0, typename T>
inline void Map<T0, T>::RightRotate(Node_map<T0, T>* y)
{
	//cout << "右旋" << endl;
	Node_map<T0, T>* parent = y->parent;
	Node_map<T0, T>* x = y->left;
	//X节点与父节点
	if (parent == nullptr)//如果父节点的父节点是空的，说明y是根节点
	{
		this->root_node = x;
		this->root_node->parent = nullptr;
		this->root_node->color = BLACK;
	}
	else
	{
		x->parent = parent;
		if (x->key > parent->key)
		{
			parent->right = x;
		}
		else
		{
			parent->left = x;
		}
	}
	//Y节点与β叶
	y->left = x->right;
	if (y->left != nullptr)
	{
		y->left->parent = y;
	}
	//X节点与Y节点
	x->right = y;
	y->parent = x;
}

template<typename T0, typename T>
inline bool Map<T0, T>::HaveUncleNode(Node_map<T0, T>* node)
{
	Node_map<T0, T>* node_parent = node->parent;
	if (node_parent == nullptr ||
		node == nullptr ||
		node_parent->parent == nullptr)
	{
		return false;
	}
	Node_map<T0, T>* node_grand_parent = node_parent->parent;
	bool parent_pos = node_grand_parent->left == node_parent;//父节点在祖父结点左边返回true
	if (parent_pos)
	{
		if (node_grand_parent->right != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (node_grand_parent->left != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
template<typename T0, typename T>
inline bool Map<T0, T>::UncleNodeNull(Node_map<T0, T>* node)
{
	bool parent_pos = node->parent->parent->left == node->parent;
	if (parent_pos)
	{
		if (node->parent->parent->right == nullptr ||
			node->parent->parent->right->color == BLACK)
		{
			return true;
		}
	}
	else
	{
		if (node->parent->parent->left == nullptr ||
			node->parent->parent->left->color == BLACK)
		{
			return true;
		}
	}
	return false;
}

