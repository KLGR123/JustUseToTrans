#include <iostream>
using namespace std;

template<class T>class BiNode
{
public:
	T data;
	BiNode<T>* lch;
	BiNode<T>* rch;
	BiNode() :lch(NULL), rch(NULL), data(0) {};
};

template<class T>class BST
{
public:
	BST(T r[], int n); //构造函数
	~BST(); //析构函数
	void Release(BiNode<T>* R); //销毁函数
	BiNode<T>* Search(BiNode<T>* R, T key); //查找函数 关键字key
	void InsertBST(BiNode<T>*& R, BiNode<T>* s); //插入节点
	void Delete(BiNode<T>*& R); //删除节点
	bool DeleteBST(BiNode<T>*& R, T key); //删除给定关键字key
	BiNode<T>* GetRoot() { return Root; } //返回根节点
	void DeleteMax(BiNode<T>* R); //删除子树中最大的节点
	void DeleteMin(BiNode<T>* R); //删除子树中最小的节点
	BiNode<T>* SearchFather(BiNode<T>* s, T target); //查找父节点
	void TreePrint(BiNode<T>* parent, BiNode<T>* root, string& prefix);//打印二叉树
	
private:
	BiNode<T>* Root; //根节点
};

template<class T>
BST<T>::BST(T r[], int n) //构造函数
{
	//设置空根节点 迭代列表r
	//创建新节点 data 左右指针 赋值 
	//调用插入函数InsertBST
	Root = NULL;
	for (int i = 0; i < n; i++)
	{
		BiNode<T>* s = new BiNode<T>;
		s->data = r[i];
		s->lch = s->rch = NULL;
		InsertBST(Root, s);
	}
}

template<class T>
BST<T>::~BST() //析构函数
{
	Release(Root);
}

template<class T>
void BST<T>::Release(BiNode<T>* R)  //销毁函数
{
	if (R != NULL)
	{
		Release(R->lch); //销毁左子树
		Release(R->rch); //销毁右子树
		delete R; //销毁根
	}
}

template<class T>
BiNode<T>* BST<T>::Search(BiNode<T>* R, T key) //查找函数 关键字key
{
	//递归算法 地址R记录当前查找地址 为空返回空 不为空判断
	//如果对应数据等于key则返回地址 否则判断大小
	//小于进入左子树 否则进入右子树 Search
	if (R == NULL) return NULL;
	if (key == R->data) return R;
	else if (key < R->data) return Search(R->lch, key);
	else return Search(R->rch, key);
}

template<class T>
void BST<T>::InsertBST(BiNode<T>*& R, BiNode<T>* s)//插入节点
{
	//R指定为根节点 s为待插入的新节点
	//递归 如果R为空 s插入R的位置 
	//否则 判断大小关系 如果s对应数据小于R对应数据 则进入左支InsertBST 反之进右
	if (R == NULL)
		R = s;
	else if (s->data > R->data)
		InsertBST(R->rch, s);
	else
		InsertBST(R->lch, s);
}

template<class T>
void BST<T>::Delete(BiNode<T>*& R) //删除节点
{
	//删除后保证有序
	//先对删除情况分类 即删除叶子节点 删除只有左（右）子树的节点和一般节点
	//q为临时记录节点 s为欲删除的节点的左支的最右边的节点
	BiNode<T>* q, * s;
	if (R->lch == NULL) //注意此时包括了叶子节点和只有右子树的节点
	{
		q = R;
		R = R->rch;
		delete q;
	}
	else if (R->rch == NULL) //只有右子树
	{
		q = R;
		R = R->lch;
		delete q;
	}
	else //一般情况
	{
		s = R->lch; q = R; //s为欲删除的节点的左支的最右边的节点
		while (s->rch != NULL)
		{
			q = s; //q是s的双亲
			s = s->rch; //s迭代更新直到找到左支最右侧节点
		}
		R->data = s->data; //使用前驱数值替换当前节点数值
		if (q != R) //欲删除的节点的左孩子的右子树非空 s此时为q的右孩子
			q->rch = s->lch; //s已经被赋值给欲删除节点 则需将s的左子树与其父节点q连接
		else //欲删除的节点的左孩子的右子树为空 s此时为R的左孩子
			R->lch = s->lch;//R的左孩子将被替换为s的左孩子 即R的左孙子节点
		delete s;
	}
}

template<class T>
bool BST<T>::DeleteBST(BiNode<T>*& R, T key) //删除给定关键字key
{
	//R是二叉排序树的根节点 key是关键字
	//若未找到 即R已为空 返回false
	//找到关键字后 调用Delete删除节点并返回true
	//若key与R对应data不相等 递归
	//若key小于data 则进入左子树继续查找并删除 反之进入右子树
	if (R == NULL) return false;
	else
	{
		if (key == R->data)
		{
			Delete(R);
			return true;
		}
		else if (key < R->data) return DeleteBST(R->lch, key);
		else return DeleteBST(R->rch, key);
	}
}

template<class T> 
void BST<T>::DeleteMax(BiNode<T>* R) //删除子树中最大的节点
{
	//右子树的最右节点一定是最大节点
	//循环更新 找到maximum 删除
	//确保特殊情况成立
	if (R->rch == NULL)
		Root = Root->lch;
	if (R->rch != NULL && R->rch->rch == NULL)
	{
		Root->data = Root->rch->data;
		Root->rch = NULL;
	}
	while (R->rch->rch != NULL)
	{
		R = R->rch;
	}
	R->rch = NULL;
	delete R->rch;
}

template<class T>
void BST<T>::DeleteMin(BiNode<T>* R) //删除子树中最小的节点
{
	if (R->lch == NULL)
		Root = Root->rch;
	if (R->lch != NULL && R->lch->lch == NULL)
	{
		Root->data = Root->lch->data;
		Root->lch = NULL;
	}
	while (R->lch->lch != NULL)
	{
		R = R->lch;
	}
	R->lch = NULL;
	delete R->lch;
}


template<class T>
BiNode<T>* BST<T>::SearchFather(BiNode<T>* s,T target) //查找父节点
{
	//如果s为空 说明没找到 返回空
	//判断若s的孩子节点有R 则返回s 找到R父节点
	//若没有R 比较data大小并对应递归
	if (s == NULL) return NULL;

	if ((s->lch != NULL && s->lch->data == target)||(s->rch != NULL && s->rch->data == target))
		return s;
	if (target < s->data) 
		return SearchFather(s->lch, target);
	if (target > s->data)
		return SearchFather(s->rch, target);
}

template<class T>
void BST<T>::TreePrint(BiNode<T>* parent, BiNode<T>* root, string& prefix) //打印二叉树
{
	prefix += "|"; //与上级树线段相连
	if (root) //有左或右子树
	{
		cout << prefix << "--" << root->data << endl; //打印
		if (root == parent || root == parent->rch) //当转向右支
		{
			prefix.pop_back(); //删去字符串末尾的线段
			prefix += " ";     //改为空格
		}
		TreePrint(root, root->lch, prefix + "  "); //递归 
		TreePrint(root, root->rch, prefix + "  ");
	}
	else //是叶子结点
	{
		if (parent->lch || parent->rch) //只要孩子节点不空 打印
			cout << prefix << "--" << "□" << endl;
	}
}

int main()
{
	int list[12] = { 45, 12, 53, 3, 37, 100, 24, 39, 61, 113, 90, 78 };
	BST<int> tree(list, 12);
	cout<<"二叉排序树："<<endl;
	string s;
	tree.TreePrint(tree.GetRoot(), tree.GetRoot(), s);
	s.clear();

	BiNode<int>* result = tree.Search(tree.GetRoot(), 37);
	cout << "查询37的结果为：" << result->data << endl;
	result = tree.Search(tree.GetRoot(), 90);
	cout << "查询90的结果为：" << result->data << endl;

	cout << endl;
	BiNode<int>* newNode= new BiNode<int>;
	newNode->data = 40;

	BiNode<int>* root = tree.GetRoot();

	cout << "插入新节点40的效果：" << endl;
	tree.InsertBST(root, newNode);
	tree.TreePrint(tree.GetRoot(), tree.GetRoot(), s);
	s.clear();

	cout << "删去刚插入的新节点：" << endl;
	tree.DeleteBST(root, 40);
	tree.TreePrint(tree.GetRoot(), tree.GetRoot(), s);
	s.clear();

	cout << "删去最小节点的效果：" << endl;
	tree.DeleteMin(tree.GetRoot());
	tree.TreePrint(tree.GetRoot(), tree.GetRoot(), s);
	s.clear();

	cout << "删去最大节点的效果：" << endl;
	tree.DeleteMax(tree.GetRoot());
	tree.TreePrint(tree.GetRoot(), tree.GetRoot(), s);
	s.clear();

	BiNode<int>* resul = NULL;
	resul = tree.SearchFather(tree.GetRoot(), 61); 
	cout << "搜索节点61的父亲节点对应数据：" << resul->data << endl;

	tree.~BST();
	return 0;
}
