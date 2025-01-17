//设计一个类redBlackTree派生于抽象类bSTree。编写所有函数代码
//find，insert，delete的时间复杂度必须是O（logn）
//ascend的时间复杂度应该是O（n）
#include<iostream>
#include<fstream>
#include<iomanip>
#include<limits>
using namespace std;
template<class T>
class RedBlackNode
{
public:
	T data;//存储的值
	int color;//黑1红0
	RedBlackNode<T>* LeftChild;
	RedBlackNode<T>* RightCild;
	RedBlackNode<T>* parent;
	RedBlackNode(int col, T DATA, RedBlackNode<T>* left = nullptr, RedBlackNode<T>* right = nullptr, RedBlackNode<T>* pa = nullptr)
	{
		color = col;
		data = DATA;
		LeftChild = left;
		RightCild = right;
		parent = pa;
	}
};
template<class T>
class RedBlackTree
{
private:
	RedBlackNode<T>* root;//根节点

public:
	//默认构造函数
	RedBlackTree()
	{
		root = nullptr;
	}
	RedBlackNode<T>* getRoot()
	{
		return root;
	}
	//查找关键字为key的数对并且返回
	//找到了就返回这个数对，否则返回nullptr
	//操作与BST相同
	RedBlackNode<T>* find(T key)
	{
		RedBlackNode<T>* p = this->root;
		while (p)
		{
			if (key < p->data)
			{
				//所要查找的值小于目前值，则去左孩子
				p = p->LeftChild;
			}
			else if (key > p->data)
			{
				//大于则去右孩子
				p = p->RightCild;
			}
			else
			{
				//相等则找到了，返回其即可
				return p;
			}
		}
		//如果运行到这里说明没找到
		return nullptr;
	}
	//插入数对
	void Insert(T key)
	{
		RedBlackNode<T>* leaf = new RedBlackNode<T>((T)INT_MAX, 1);
		//先判断这个值是否已经存在
		RedBlackNode<T>* p = root;
		RedBlackNode<T>* parent_of_p = nullptr;
		while (p)
		{
			parent_of_p = p;//记录当前位置
			if (key < p->data)
			{
				p = p->LeftChild;
			}
			else
			{
				p = p->RightCild;
			}

		}
		//此时p的值即为插入处
		//新节点设置为红色
		RedBlackNode<T>* r = new RedBlackNode<T>(0, key);
		if (root != nullptr)
		{
			//如果根不为空
			if (key < parent_of_p->data)
			{
				//小于则插入左子树
				parent_of_p->LeftChild = r;
				r->parent = parent_of_p;
			}
			else
			{
				//大于则插入右子树
				parent_of_p->RightCild = r;
				r->parent = parent_of_p;
			}
			//以上为节点的插入，接下来应该判断是否出现连续红边的情况
			RedBlackNode<T>* pu, * gu, * uncle, * curr;
			curr = r;//指向现在的节点
			//接下来不断改变temp以实现向上遍历
			while ((pu = curr->parent) && pu->color == 0)
			{
				//父节点存在且为红色
				//即出现连续红边
				//让uncle指向祖父节点的另一个孩子
				gu = pu->parent;
				if (gu->LeftChild == pu)
				{
					uncle = gu->RightCild;
				}
				else
				{
					uncle = gu->LeftChild;
				}
				//看祖父另一个孩子的颜色
				if (uncle && uncle->color == 0)
				{
					//祖父另一个孩子也是红色
					//直接变色
					gu->color = 0;
					pu->color = 1;
					uncle->color = 1;
					curr = gu;
				}
				else
				{
					//祖父另一个孩子是黑色或不存在
					if (curr == pu->LeftChild && pu == gu->LeftChild)
					{
						//LLr
						curr = LLr(gu);
						curr = gu;
					}
					else if (curr == pu->RightCild && pu == gu->LeftChild)
					{
						//LRr
						LRr(gu);
						curr = gu;
					}
					else if (curr == pu->LeftChild && pu == gu->RightCild)
					{
						//RLr
						RLr(gu);
						curr = gu;
					}
					else if (curr == pu->RightCild && pu == gu->RightCild)
					{
						//RRr
						RRr(gu);
						curr = gu;
					}
					//temp = gu;
				}
			}
			root->color = 1;
		}
		else
		{
			//根为空，设当前节点为根
			root = r;
			root->color = 1;//根节点必须黑色
			//无需判断会否出现旋转的情况
		}
	}
	RedBlackNode<T>* LLr(RedBlackNode<T>* gu)
	{
		RedBlackNode<T>* pu = gu->LeftChild;
		RedBlackNode<T>* u = pu->LeftChild;
		gu->LeftChild = pu->RightCild;
		if (pu->RightCild != nullptr)
		{
			pu->RightCild->parent = gu;
		}
		pu->parent = gu->parent;
		if (gu->parent == nullptr)
		{
			root = pu;
		}
		else
		{
			if (gu == gu->parent->LeftChild)
				gu->parent->LeftChild = pu;
			else
				gu->parent->RightCild = pu;
		}
		pu->RightCild = gu;
		gu->parent = pu;
		//修改颜色
		gu->color = 0;
		pu->color = 1;
		return pu;
	}
	RedBlackNode<T>* LRr(RedBlackNode<T>* gu)
	{
		RedBlackNode<T>* pu = gu->LeftChild;
		RedBlackNode<T>* u = pu->RightCild;
		u->parent = gu->parent;
		if (gu->parent == nullptr)
		{
			//即gu是根节点时
			root = u;
		}
		else
		{
			if (gu == gu->parent->LeftChild)
				gu->parent->LeftChild = u;
			else
				gu->parent->RightCild = u;
		}
		//pu->RightCild = nullptr;
		pu->RightCild = u->LeftChild;
		if (u->LeftChild)
			u->LeftChild->parent = pu;
		//gu->LeftChild = nullptr;
		gu->LeftChild = u->RightCild;
		if (u->RightCild)
			u->RightCild->parent = gu;
		u->LeftChild = pu;
		pu->parent = u;
		u->RightCild = gu;
		gu->parent = u;
		//修改颜色
		u->color = 1;
		gu->color = 0;
		return u;
	}
	RedBlackNode<T>* RLr(RedBlackNode<T>* gu)
	{
		RedBlackNode<T>* pu = gu->RightCild;
		RedBlackNode<T>* u = pu->LeftChild;
		u->parent = gu->parent;
		if (gu->parent == nullptr)
		{
			//说明gu为根
			root = u;
		}
		else
		{
			if (gu == gu->parent->LeftChild)
				gu->parent->LeftChild = u;
			else
				gu->parent->RightCild = u;
		}
		gu->RightCild = u->LeftChild;
		if (u->LeftChild)
			u->LeftChild->parent = gu;
		pu->LeftChild = u->RightCild;
		if (u->RightCild)
			u->RightCild->parent = pu;
		u->LeftChild = gu;
		gu->parent = u;
		u->RightCild = pu;
		pu->parent = u;
		//改颜色
		u->color = 1;
		gu->color = 0;
		return u;
	}
	RedBlackNode<T>* RRr(RedBlackNode<T>* gu)
	{
		RedBlackNode<T>* pu = gu->RightCild;
		RedBlackNode<T>* u = pu->RightCild;
		pu->parent = gu->parent;
		if (gu->parent == nullptr)
		{
			root = pu;
		}
		else
		{
			if (gu == gu->parent->LeftChild)
				gu->parent->LeftChild = pu;
			else
				gu->parent->RightCild = pu;
		}
		gu->RightCild = pu->LeftChild;
		if (pu->LeftChild)
		{
			pu->LeftChild->parent = gu;
		}
		pu->LeftChild = gu;
		gu->parent = pu;
		//改颜色
		gu->color = 0;
		pu->color = 1;
		return pu;
	}
	//删除关键字为key的数
	void Delete(T key)
	{
		//利用find函数定位到该节点
		RedBlackNode<T>* node = find(key);
		int color;
		if (node == nullptr)
		{
			cout << "不存在此节点！" << endl;
			return;
		}
		//判断该节点情况
		//删除节点有两个子树
		if (node->LeftChild && node->RightCild)
		{
			//寻找该节点的后继节点
			RedBlackNode<T>* replace = node;
			replace = node->LeftChild;
			while (replace->RightCild)
				replace = replace->RightCild;
			//再将后继节点值赋给需要删除的值，转化为2
			if (node->parent == nullptr)//删除节点是根节点
			{
				root = replace;
			}
			else//删除节点不是根节点
			{
				//建立父子联系
				if (node == node->parent->LeftChild)
				{
					node->parent->LeftChild = replace;
				}
				else
				{
					node->parent->RightCild = replace;
				}
			}
			//给replace的左孩子改关系
			//后继节点没有右孩子
			RedBlackNode<T>* child = replace->LeftChild;
			RedBlackNode<T>* parent = replace->parent;
			//保存replace的颜色之后做判断
			//因为是转化为删除replace
			color = replace->color;
			if (parent == node)
			{
				//待删除节点是后继节点的父节点
				//则parent仍然指向自己
				parent = replace;
			}
			else
			{
				//将replace的孩子转移给replace的父节点
				//parent->LeftChild = child;
				parent->RightCild = child;
				if (child)
					child->parent = parent;
				replace->LeftChild = node->LeftChild;
				replace->LeftChild->parent = replace;
			}
			//将node的关系转给replace
			replace->parent = node->parent;
			replace->color = node->color;
			replace->RightCild = node->RightCild;
			node->RightCild->parent = replace;

			//以上为删除操作，接下来判断是否需要重置
			if (color == 1)
			{
				//删除节点是黑色的
				DeleteChange(child, parent);
			}
			delete node;
			return;
		}
		//待删除节点只有一个子树时
		else if (node->LeftChild != nullptr || node->RightCild != nullptr)
		{
			//将node的孩子节点取代它自己即可
			RedBlackNode<T>* child, * parent;
			if (node->LeftChild)
				child = node->LeftChild;
			else
				child = node->RightCild;
			parent = node->parent;
			color = node->color;

			if (child)
				child->parent = parent;
			if (parent)
			{
				//说明不是根节点
				if (node == parent->LeftChild)
					parent->LeftChild = child;
				else
					parent->RightCild = child;

			}
			else
			{
				//是根节点
				root = child;
			}
			if (color == 1)
				DeleteChange(child, parent);
			delete node;
		}
		//它没有孩子
		else
		{
			RedBlackNode<T>* parent = node->parent;
			color = node->color;
			if (parent)
			{
				//不是根节点
				if (node == parent->LeftChild)
					parent->LeftChild = nullptr;
				else
					parent->RightCild = nullptr;
			}
			else
			{
				//是根节点
				root = nullptr;
			}
			if (color == 1)
				DeleteChange(nullptr, parent);
			delete node;
		}

	}
	//类比AVL的删除操作
	void DeleteChange(RedBlackNode<T>* node, RedBlackNode<T>* parent)
	{
		RedBlackNode<T>* other;
		while ((!node || node->color == 1) && node != root)
		{
			//分为node是左孩子和右孩子两种情况
			if (parent->LeftChild == node)
			{
				//node是parent的左孩子
				other = parent->RightCild;
				//另一个孩子红色
				//则other一定有两个内部的黑色节点孩子
				if (other->color == 0)
				{
					//Lr型
					//Lr0型
					if (other->LeftChild->color == 1 && other->RightCild->color == 1)
					{
						other->color = 1;
						other->LeftChild->color = 0;
						leftRotate(parent);
						//转完之后整棵树平衡

					}
					//Lr1.1
					else if (other->LeftChild->RightCild->color == 0)
					{
						other->LeftChild->RightCild->color = 1;
						rightRotate(other);
						leftRotate(parent);
						//平衡

					}
					//Lr1.2
					else if (other->LeftChild->LeftChild->color == 0)
					{
						other->LeftChild->LeftChild->color = 1;
						rightRotate(other->LeftChild);
						rightRotate(other);
						leftRotate(parent);
					}
					//LR2
					else
					{
						//和LR1.2旋转一样
						other->LeftChild->LeftChild->color = 1;
						rightRotate(other->LeftChild);
						rightRotate(other);
						leftRotate(parent);
					}
					//这三种情况都可以通过一次旋转获得平衡
					break;
				}
				else
				{
					//Lb型
					//另一个孩子黑色，根据它的孩子颜色状况 分为三种情况
					//Lb0
					if ((!other->LeftChild || other->LeftChild->color == 1) && (!other->RightCild || other->RightCild->color == 1))
					{
						//记录一下parent原来的颜色以判断是否结束
						int color = parent->color;
						//直接变色
						parent->color = 1;
						other->color = 0;
						//原来是红色则完成平衡
						if (color == 0)
							break;
						else
							node = parent;
					}
					//Lb1.1
					else if (other->LeftChild && other->LeftChild->color == 0)
					{
						other->LeftChild->color = parent->color;
						parent->color = 1;
						rightRotate(other);
						leftRotate(parent);
						break;
					}
					//Lb1.2
					else if (other->RightCild && other->RightCild->color == 0)
					{
						other->color = parent->color;
						parent->color = 1;
						other->RightCild->color = 1;
						leftRotate(parent);
						break;
					}
					//Lb2
					else
					{
						other->LeftChild->color = parent->color;
						parent->color = 1;
						rightRotate(other);
						leftRotate(parent);
						break;
					}
				}
			}
			else
			{
				//node是parent的右孩子
				other = parent->LeftChild;
				//另一孩子红色
				if (other && other->color == 0)
				{
					//Rr0
					//other一定有两个黑色内部节点孩子
					if (other->LeftChild->color == 1 && other->RightCild->color == 1)
					{
						other->color = 1;
						other->RightCild->color = 0;
						rightRotate(parent);
					}
					//Rr1.1
					else if (other->RightCild->LeftChild->color == 0)
					{
						other->RightCild->LeftChild->color = 1;
						leftRotate(other);
						rightRotate(parent);
					}
					//Rr1.2
					else if (other->RightCild->RightCild->color == 0)
					{
						other->RightCild->RightCild->color = 1;
						leftRotate(other->RightCild);
						leftRotate(other);
						rightRotate(parent);

					}
					//Rr2操作和Rr1.2一样
					else
					{
						other->RightCild->RightCild->color = 1;
						leftRotate(other->RightCild);
						leftRotate(other);
						rightRotate(parent);
					}
					//整个Rr型做完必定平衡
					break;
				}
				else
				{
					//Rb型
					//Rb0;
					if ((!other->LeftChild || other->LeftChild->color == 1) && (!other->RightCild || other->RightCild->color == 1))
					{
						int color = parent->color;
						parent->color = 1;
						other->color = 0;
						if (color == 0)
							break;
						else
						{
							node = parent;
						}
					}
					//Rb1.1
					else if (other->LeftChild && other->LeftChild->color == 0)
					{
						other->color = parent->color;
						parent->color = 1;
						other->LeftChild->color = 1;
						rightRotate(parent);
						break;
					}
					//Rb1.2
					else if (other->RightCild && other->RightCild->color == 0)
					{
						other->RightCild->color = parent->color;
						parent->color = 1;
						leftRotate(other);
						rightRotate(parent);
						break;
					}
					//Rb2
					else
					{
						other->RightCild->color = parent->color;
						parent->color = 1;
						leftRotate(other);
						rightRotate(parent);
						break;
					}
				}
			}
		}
		if (node)
			node->color = 1;
	}
	void leftRotate(RedBlackNode<T>* x)
	{
		//令x的右孩子是y
		RedBlackNode<T>* y = x->RightCild;
		//将y的左孩子作为x的右孩子
		x->RightCild = y->LeftChild;
		if (y->LeftChild)
			y->LeftChild->parent = x;
		//将x的父亲设置为y的父亲
		y->parent = x->parent;
		if (x->parent == nullptr)
		{
			//是根
			root = y;
		}
		else
		{
			if (x == x->parent->LeftChild)
				x->parent->LeftChild = y;
			else
				x->parent->RightCild = y;
		}
		//再将x作为y的左孩子
		y->LeftChild = x;
		x->parent = y;
	}
	void rightRotate(RedBlackNode<T>* y)
	{
		//设x是y的左孩子
		RedBlackNode<T>* x = y->LeftChild;
		//x的右孩子设置为y的左孩子
		y->LeftChild = x->RightCild;
		if (x->RightCild)
			x->RightCild->parent = y;
		//y父亲为x的父亲
		x->parent = y->parent;
		if (y->parent == nullptr)
			root = x;
		else
		{
			if (y == y->parent->LeftChild)
				y->parent->LeftChild = x;
			else
				y->parent->RightCild = x;

		}
		//将y变成x的右孩子
		x->RightCild = y;
		y->parent = x;
	}
	//按关键字升序输出
	void ascend(RedBlackNode<T>* temp)
	{
		//升序输出即LVR，中序输出
		if (temp->LeftChild)
			ascend(temp->LeftChild);
		if (temp)
			cout << setw(5) << temp->data;
		if (temp->RightCild)
			ascend(temp->RightCild);
	}
	void print(RedBlackNode<T>* temp, int height)
	{
		if (temp != nullptr)
		{
			print(temp->RightCild, height + 1);
			cout << setw((long int)8 * height) << " ";

			string color;
			if (temp->color == 0)
				color = "red";
			else
			{
				color = "black";
			}
			cout << temp->data << color;
			cout << endl;
			print(temp->LeftChild, height + 1);
		}
	}
};
int main()
{
	RedBlackTree<int> RBT;
	RedBlackNode<int>* _root = RBT.getRoot();
	cout << "插入操作：" << endl;
	for (int i = 0; i < 10; i++)
	{
		int temp;
		cin >> temp;
		RBT.Insert(temp);
		_root = RBT.getRoot();
	}
	cout << "插入操作结果为：" << endl;
	RBT.print(_root, 0);
	cout << "删除操作：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << "删除：";
		int tem;
		cin >> tem;
		RBT.Delete(tem);
		_root = RBT.getRoot();
		cout << "删除后操作结果为：" << endl;
		RBT.print(_root, 0);
	}
	cout << endl << "ascend操作为:" << endl;
	RBT.ascend(_root);
}
/*Insert: 50 60 65 80 5 10 62 70 23 10
Delete: 5 10 50
*/