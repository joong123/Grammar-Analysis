// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <vector>
#include <iostream>
#include "grammar.h"
#include "tree0.h"

using namespace std;

 // Definition for a binary tree node.
  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(NULL), right(NULL) {}
      TreeNode(int x) : val(x), left(NULL), right(NULL) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
  };

class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) { 
        vector<TreeNode> vec;
        
    }
};

typedef int* list0int;
typedef int* it0int;

int main()
{
    // "in", "out", "xx", "xxx"
    const char* const Col1Item1 = "in";
    const char* const Col1Item2 = "out";
    const char* const Col1Item3 = "xx";
    const char* const Col1Item4 = "xxx";
    const char* arr[4] = { Col1Item1, Col1Item2, Col1Item3, Col1Item4 };

    int index = 3;
    int ret = scanf_s("%d", &index);

    const char* selected = arr[index];

    printf("selected: %s\n\n", selected);






    int ia = 3;




    // 三个指针
    int* pa         = &ia;
    const int* cpa  = &ia;
    int* const pca  = &ia;

    /*int** ppa;
        int const** cpa = &cpa;
        int* const* pca=&pca;*/


    // 同学们我们继续看，这里三个指针的指针
    // 问题来了，请回答：这三个指针的指针分别是什么类型
    auto ppa        = &pa;
    auto pcpa       = &cpa;
    auto ppca       = &pca;




    // 

    typedef int* pint;
    const pint* ppi = NULL;
    
    const int** cpi = NULL;
    // int* const cpi;

    cout << typeid(ppi).name() << endl;
    cout << typeid(ppca).name() << endl;
    cout << typeid(cpi).name() << endl;










    TestBT();

    int a = 0;
    int b = 1;
    int c = 9;
    //c = a = getchar(), b = getchar();
    //std::cout << "Hello World!\n";

    list0int ls = LIST0MAKEBY(int, 5);
    LIST0ADD(ls, c);
    LIST0ADD(ls, b);

    LIST0AT(ls, 4) = 234;
    for (it0int it = LIST0BEGIN(ls); it != LIST0END(ls); ++it)
    {
        cout << *it << ',';
    }
    cout << endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
