#pragma once

#include<vector>
#include<queue>
#include<string>
#include<sstream>
#include<iostream>


using std::vector;
using std::queue;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;


template<typename T>
struct BTNode
{
	T		data;
	BTNode* left;
	BTNode* right;

	BTNode() : data(), left(NULL), right(NULL) {}
	BTNode(const T& d, BTNode* l = NULL) : data(d), left(l), right(NULL) {}
};


template<typename T>
struct BTNodeInit
{
	bool	bData;
	T		data;

	BTNodeInit() : bData(false), data() {}
	BTNodeInit(bool b, const T& d = T()) : bData(b), data(d) {}
};

template<typename T>
BTNode<T>* CreateBTreeHier(const vector<BTNodeInit<T>>& arr, int& retCode)
{
	retCode = 0;

	BTNode<T>* nDummy = new BTNode<T>;

	if (arr.empty())
		return nDummy;

	if (!arr.front().bData)
		return nDummy;

	BTNode<T>* nRoot = new BTNode<T>;
	nRoot->data = arr.front().data;

	queue<BTNode<T>*> q;
	q.push(nRoot);

	int errCode = 0;
	for (auto it = arr.begin() + 1; it != arr.end(); ++it)
	{
		if (q.empty())
		{
			errCode = -2;
			break;
		}

		BTNode<T>* node = q.front();
		q.pop();

		BTNodeInit<T> init = *it;
		if (init.bData)
		{
			BTNode<T>* pn = new BTNode<T>;
			node->left = pn;
			pn->data = init.data;

			q.push(pn);
		}

		++it;
		if (arr.end() == it)
		{
			errCode = -3;
			break;
		}

		init = *it;
		if (init.bData)
		{
			BTNode<T>* pn = new BTNode<T>;
			node->right = pn;
			pn->data = init.data;

			q.push(pn);
		}
	}

	retCode = errCode;

	return nRoot;
}

template<typename T>
int TraverseHier(BTNode<T>* root)
{
	if (NULL == root)
		return 0;

	queue<BTNode<T>*> q;
	q.push(root);

	while (!q.empty())
	{
		BTNode<T>* node = q.front();
		q.pop();

		cout << node->data << ',';

		if (NULL != node->left)
			q.push(node->left);
		else
			cout << "-,";

		if (NULL != node->right)
			q.push(node->right);
		else
			cout << "-,";
	}


	cout << endl;

	return 1;
}

inline void TestBT()
{
	cout << "======TestBTree=====" << endl;

	string strBTB = "1,/,2,";
	cout << "input string Hier: " << strBTB << endl;

	stringstream ss(strBTB);

	vector<BTNodeInit<int>> arr;
	while (!ss.eof())
	{
		char c = ss.peek();
		if (!isdigit(c))
		{
			if ('/' == c)
			{
				arr.push_back(BTNodeInit<int>());
			}
			ss.get();
			continue;
		}

		int i = 0;
		ss >> i;

		arr.push_back(BTNodeInit<int>(true, i));
	}

	int retCode = 0;
	BTNode<int>* pBTree = CreateBTreeHier(arr, retCode);

	if (NULL == pBTree)
	{
		cout << "Create BTree Hier failed: null ptr" << endl;
		return;
	}

	if (retCode < 0)
	{
		cout << "Create BTree Hier failed: ret code=" << retCode << endl;
		return;
	}

	cout << "Traverse Hier: ";
	TraverseHier(pBTree);

	cout << "======TestBTree End=====" << endl;
}