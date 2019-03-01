//
//  main.cpp
//  Treap
//
//  Created by Семен Пьянков on 08/10/2018.
//  Copyright © 2018 sepy. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <utility>

using namespace std;
struct node
{
    node *left, *right;
    int key, priority;
    node () : key (0), priority (0), left (nullptr), right (nullptr) { }
    node (int key, int priority) : key (key), priority (priority), left (nullptr), right (nullptr) { }
    
};
typedef node* treap;

void dumpTreap (treap out, int spacingCounter = 0)
{
    if (out)
    {
        dumpTreap (out->right, spacingCounter + 1);
        for (int i = 0; i < spacingCounter; i++) printf ("_________");
        printf ("(%d.%d)\n", out->key, out->priority);
        dumpTreap (out->left, spacingCounter + 1);
    }
}

pair<treap, treap> split (treap root, int key, treap* dupl) //операция split разделяет дерево на два по ключу, если есть совпадение со значением ключа
                                                                                                            // - кидает эту вершину в dupl
{
    if (root == nullptr) return make_pair (nullptr, nullptr);
    
    if (root->key < key)
    {
        (*dupl) = nullptr;
        pair<treap, treap> splitted = split (root->right, key, dupl);
        root->right = splitted.first;
        return make_pair (root, splitted.second);
    }
    else if (root->key > key)
    {
        (*dupl) = nullptr;
        pair<treap, treap> splitted = split (root->left, key, dupl);
        root->left = splitted.second;
        return make_pair (splitted.first, root);
    }
    else
    {
        (*dupl) = root;
        return make_pair (root->left, root->right);
    }
}

treap merge(treap left, treap right) //операция merge - сливает два дерева
{
    if (left == nullptr || right == nullptr) return right == nullptr ? left : right;
    
    if (left->key > right->key)
    {
        treap tmp = right;
        right = left;
        left = tmp;
    }
    
    if (left->priority > right->priority)
    {
        left->right = merge (left->right, right);
        //dump (left);
        return left;
    }
    else
    {
        right->left = merge (left, right->left);
        //dump (right);
        return right;
    }
}

treap unite (treap left, treap right) //множественная операция объединения
{
    if (!left || !right)  return left ? left : right;
    if (left->priority < right->priority)  swap (left, right); //гарантируем, что высший приоритет у левого дерева
    
    treap lt, rt, duplicate;
    auto tmp = split (right, left->key, &duplicate);
    lt = tmp.first; rt = tmp.second;
	
	auto root = new node (left->key, left->priority);
    root->left = unite (left->left, lt);
    root->right = unite (left->right, rt);
    return root;
}

treap intersect (treap left, treap right) //множественная операция пересечения
{
    /*dumpTreap (left);
    printf ("*****************************\n");
    dumpTreap (right);
    printf ("*****************************\n");*/
    
    if (!left || !right)  return nullptr;
    if (left->priority < right->priority)  swap (left, right); //гарантируем, что высший приоритет у левого дерева
    
    treap lt, rt, duplicate, tmpleft, tmpright;
    auto tmp = split (right, left->key, &duplicate);
    lt = tmp.first; rt = tmp.second;
    
    tmpleft  = intersect (left->left, lt);
    tmpright = intersect (left->right, rt);
    
    if (duplicate)
    {
	    auto root = new node (left->key, left->priority);
        root->left = tmpleft;
        root->right = tmpright;
        return root;
    }
    else
    {
        return merge (tmpleft, tmpright);
    }
}

treap differ (treap left, treap right, bool right_is_subtr) //множественная операция разности, subtr=true когда right - это "вычитаемое"
{
    /*dumpTreap (left);
    printf ("*****************************\n");
    dumpTreap (right);
    printf ("*****************************\n");*/
    
    if (!left || !right)  return right_is_subtr ? left : right;
    if (left->priority < right->priority)
    {
        right_is_subtr = !right_is_subtr;
        swap (left, right); //гарантируем, что высший приоритет у левого дерева
    }
    
    treap lt, rt, duplicate, tmpleft, tmpright;
    auto tmp = split (right, left->key, &duplicate);
    lt = tmp.first; rt = tmp.second;
    
    tmpleft  = differ (left->left,  lt, right_is_subtr);
    tmpright = differ (left->right, rt, right_is_subtr);
    
    if (duplicate == nullptr && right_is_subtr)
    {
	    auto root = new node (left->key, left->priority);
        root->left = tmpleft;
        root->right = tmpright;
        return root;
    }
    else
    {
        return merge (tmpleft, tmpright);
    }
}

int main ()
{
	auto t1 = new node (13, 3);
	auto t2 = new node (9, 7);
	auto t3 = new node (14, 4);
	auto t4 = new node (13, 8);
    
    t2 = merge (t1, t2);
    t4 = merge (t3, t4);
    
    dumpTreap (t4);
    printf ("*****************************\n");
    
    dumpTreap (t2);
    printf ("*****************************\n");
    /*
    treap un = unite (t2, t4);
    dumpTreap (un);
    printf ("*****************************\n");
    printf ("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    
    treap inter = intersect (t2, t4);
    dumpTreap (inter);
    printf ("*****************************\n");
    */printf ("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    
    treap diff = differ (t2, t4, false);
    dumpTreap (diff);
    printf ("*****************************\n");
    
    /* if (0)
     {
     t2 = merge (t1, t2);
     t4 = merge (t3, t4);
     t4 = merge (t4, t2);
     }
     else
     {
     t2 = unite (t1, t2);
     t4 = unite (t3, t4);
     t4 = unite (t4, t2);
     }
     
    treap duplicate;
    auto t5 = split (t4, 12, &duplicate);
    
    dumpTreap (t5.first);
    printf ("*****************************\n");
    dumpTreap (t5.second);
    printf ("*****************************\n");
    if (duplicate)
    {
        dumpTreap (duplicate);
        printf ("*****************************\n");
    }*/
    return 0;
}
