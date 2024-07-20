#include <vector>
#include <iostream>
#define LIB_IMPPORT
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
    TestList();
    TestPtrMap1();

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

    typedef void (*funT0)(void);
    funT0 ft1 = NULL;
    ft1 = NULL;
    const funT0 ft2 = NULL;
    funT0 const ft3 = NULL;
    cout << typeid(funT0).name() << endl;
    cout << typeid(const funT0).name() << endl;
    cout << typeid(funT0 const).name() << endl;
    //cout << typeid(const funT0 const).name() << endl;










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
