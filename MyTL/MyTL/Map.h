#pragma once
#define  RED true
#define  BLACK false
//mapʹ�ú������Ϊ���ݽṹ
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

//����:
//
//�ڵ��Ǻ�ɫ���ɫ��
//���Ǻ�ɫ��
//����Ҷ�Ӷ��Ǻ�ɫ��Ҷ����NIL�ڵ㣩��
//ÿ����ɫ�ڵ������������ɫ���ӽڵ㡣����ÿ��Ҷ�ӵ���������·���ϲ��������������ĺ�ɫ�ڵ㡣��
//����һ�ڵ㵽��ÿ��Ҷ�ӵ����м�·����������ͬ��Ŀ�ĺ�ɫ�ڵ㡣


template<typename T0, typename T>
class Map
{
public:
	Map();
	~Map();
	void Insert(T0  key, T value);
	T Search(T0  key);
	Node_map<T0, T>* root_node;
	Node_map<T0, T>* Move(T0  key);//���ض�Ӧ�Ľ��ָ��
	void LeftRotate(Node_map<T0, T>* node);//����,����ת�Ľڵ���ұ�����Ϊ���ڵ㡣
	void RightRotate(Node_map<T0, T>* node);//����,����ת�Ľڵ��������������ڵ㡣
	int num_s;//��ѯ�ڵ�ĶԱȴ���

private:
	bool HaveUncleNode(Node_map<T0, T>* node);//ʱ�����������
	bool UncleNodeNull(Node_map<T0, T>* node);//����ڵ��ǿջ����Ǻ�ɫ
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
	if (node_this == nullptr)//����һ���ڵ㲻����
	{
		Node_map<T0, T>* node_new = new Node_map<T0, T>;
		node_new->value = value;
		node_new->key = key;
		node_new->color = BLACK;//���ڵ��Ǻ�ɫ��
		this->root_node = node_new;
		return;
	}
	auto node_this_key = node_this->key;
	if (key == node_this_key)//���У��޸ľ�ֵ
	{
		node_this->value = value;
		return;
	}
	Node_map<T0, T>* node_new = new Node_map<T0, T>;
	node_new->value = value;
	node_new->key = key;
	node_new->color = RED;//����Ľڵ�Ĭ�Ϻ�ɫ
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
		this->Adjust(node_this, node_new);//�����ע�͵��ͱ��BST
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
		if (key < node_this_key)//С�������
		{
			if (node_now->left != nullptr)//����ڵ�����û������
			{
				node_now = node_now->left;
			}
			else
			{
				return node_now;//��������ڵ��λ��
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
		if (node_this_key == key)//����
		{
			return node_now;
		}
	}
	return node_now;
}


template<typename T0, typename T>
inline void Map<T0, T>::Adjust(Node_map<T0, T>* node_target, Node_map<T0, T>* node_new)
{
	if (this->HaveUncleNode(node_new))//����3
	{
		auto node_par = node_new->parent->parent;
		bool uncle_pos = node_par->left != node_new->parent;//1��ʾ�������游�����
		bool uncle_color = uncle_pos ? node_par->left->color : node_par->right->color;
		while (uncle_color == RED)
		{
			node_par->left->color = BLACK;			//�ְֺ�������Ϊ��ɫ
			node_par->right->color = BLACK;
			node_par->color = RED;					//�游��Ϊ��ɫ
			node_new = node_par;					//�游�ڵ��Ϊ�½ڵ㣬�������ϱ�ɫ
			this->root_node->color = BLACK;			      //���ڵ�ֻ���Ǻ�ɫ
			if (!this->HaveUncleNode(node_new))		      //����½ڵ�û�������˳�
			{
				break;
			}
			node_par = node_new->parent->parent;
			uncle_pos = node_par->left
				!= node_new->parent;//1��ʾ�������游�����
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
		if (node_target == node_new->grandpar()->left)//����5
		{
			if (node_new == node_target->right)
			{
				this->LeftRotate(node_target);
				node_new = node_new->left;
			}
			node_target = node_new->parent;
			if (node_new = node_target->left)//����4
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
//			X			��X�������				Y
//		   / \			������������>		   / \
//		  ��  Y								  X   ��
//			 / \		<������������		 / \
//			��  ��		��Y�������			��  ��
//
//��д��ĸ��ʾ��㣬ϣ����ĸ��ʾҶ����Ϊ��Ҳ���Ǽ�������Ľ��
//�����������Ĳ����������治̫����⣬�������XY���֮�����һ
//���̶��ĵ㣬XY��һ���ˡ���ô��X���������������ܵ�һ�������
//������ôX�ڵ����£�Y�ڵ�����̧����
//ע�⣡���ε���ת�ڵ�Ϊ���Ӹ����Ľڵ㡣
//
template<typename T0, typename T>
inline void Map<T0, T>::LeftRotate(Node_map<T0, T>* x)
{
	//cout << "����" << endl;
	Node_map<T0, T>* parent = x->parent;
	Node_map<T0, T>* y = x->right;
	//y�ڵ��븸�ڵ�
	if (parent == nullptr)
	{
		this->root_node = y;
		this->root_node->parent = nullptr;
		this->root_node->color = BLACK;
	}
	else
	{
		y->parent = parent;
		if (y->key > parent->key)//�����ĸ����ĳ�ָ��y
		{
			parent->right = y;
		}
		else
		{
			parent->left = y;
		}
	}

	//X�ڵ����Ҷ
	x->right = y->left;
	if (x->right != nullptr)
	{
		x->right->parent = x;
	}
	//X�ڵ���Y�ڵ�
	y->left = x;
	x->parent = y;
}

template<typename T0, typename T>
inline void Map<T0, T>::RightRotate(Node_map<T0, T>* y)
{
	//cout << "����" << endl;
	Node_map<T0, T>* parent = y->parent;
	Node_map<T0, T>* x = y->left;
	//X�ڵ��븸�ڵ�
	if (parent == nullptr)//������ڵ�ĸ��ڵ��ǿյģ�˵��y�Ǹ��ڵ�
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
	//Y�ڵ����Ҷ
	y->left = x->right;
	if (y->left != nullptr)
	{
		y->left->parent = y;
	}
	//X�ڵ���Y�ڵ�
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
	bool parent_pos = node_grand_parent->left == node_parent;//���ڵ����游�����߷���true
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

