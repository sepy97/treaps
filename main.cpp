//
//  main.cpp
//  Treap
//
//  Created by Семен Пьянков on 08/10/2018.
//  Copyright © 2018 sepy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <utility>

using namespace std;
struct node
{
    node *left, *right;
    int key, priority;
    node () { }
    node (int key, int priority) : key (key), priority (priority), left (NULL), right (NULL) { }
    
};
typedef node* treap;

pair<treap, treap> split (treap root, int key)
{
    if (root == nullptr) return make_pair (nullptr, nullptr);
    
    if (root->key < key)
    {
        pair<treap, treap> splitted = split (root->right, key);
        root->right = splitted.first;
        return make_pair (root, splitted.second);
    }
    else
    {
        pair<treap, treap> splitted = split (root->left, key);
        root->left = splitted.second;
        return make_pair (splitted.first, root);
    }
    
}

treap merge(treap left, treap right)
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

treap unite (treap left, treap right)
{
    if (!left || !right)  return left ? left : right;
    if (left->priority < right->priority)  swap (left, right);
    
    treap lt, rt;
    auto tmp = split (right, left->key);
    lt = tmp.first; rt = tmp.second;
    
    left->left = unite (left->left, lt);
    left->right = unite (left->right, rt);
    return left;
}

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

int main ()
{
    treap t1 = new node (11, 3);
    treap t2 = new node (9, 7);
    treap t3 = new node (14, 4);
    treap t4 = new node (13, 8);
    
    t2 = unite/*merge*/ (t1, t2);
    t4 = unite/*merge*/ (t3, t4);
    t4 = unite/*merge*/ (t4, t2);
    
    dumpTreap (t4);
    printf ("*****************************\n");
    
    auto t5 = split (t4, 13);
    
    dumpTreap (t5.first);
    printf ("*****************************\n");
    dumpTreap (t5.second);
    printf ("*****************************\n");
    return 0;
}
