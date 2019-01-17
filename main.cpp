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
struct treap
{
    treap *left, *right;
    int key, priority;
    treap (int new_key)
    {
        key      = new_key;
        priority = rand();
        left     = right = NULL;
    }
    
};

void dump (treap *out)
{
    if (out != NULL)
    {
        printf ("(%d.%d) \n", out->key, out->priority);
        
        dump (out->left);
        dump (out->right);
    }
}

treap split (treap* less, treap* gtr, treap* root, int key)
{
    if (root == NULL)
    {
        *less = *gtr = NULL;
        return NULL;
    }
    
    treap* newroot  = new treap(root->key);
    newroot->priority = root->priority;
    
    if (root->key < key)
    {
        *less = newroot;
        return split (&(newroot->right), gtr, root->right, key);
    }
    else if (root->key > key)
    {
        *gtr = newroot;
        return split (less, &(newroot->left), root->left, key);
    }
    else
    {
        less = root->left;
        gtr  = root->right;
        return newroot;
    }
    
    //return make_pair(less, gtr);
}

treap* merge(treap *left, treap *right)
{
    if (left == nullptr || right == nullptr) return right == nullptr ? left : right;
    if (left->key > right->key)
    {
        treap* tmp = right;
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

void unite (treap* treap_left, treap* treap_right, treap* root)
{
    if (!treap_left || !treap_right)
    {
        root = treap_left;
        if (treap_left == NULL) root = treap_right;
    }
    else if (treap_left->priority > treap_right->priority)
    {
        unite (treap_left->right, treap_right, treap_left->right);
        root = treap_left;
    }
    else
    {
        unite (treap_right->left, treap_left, treap_right->left);
        root = treap_right;
    }
}

treap unionTreap (treap* left, treap* right)
{
    treap root(0), less(0), gtr(0), duplicate(0);
    
    if (left  == NULL) return *right;
    if (right == NULL) return *left;
    
    if (left->priority < right->priority)
    {
        treap* tmp = right;
        right = left;
        left = tmp;
    }
    
    duplicate = split (&less, &gtr, right, left->key);
    
    
    root = *new treap(left->key);
    root.priority = left->priority;
    *(root.left) = unionTreap(left->left, &less);
    *(root.right) = unionTreap(left->right, &gtr);
    
    return root;
}

void dumpTreap (treap* out, int spacingCounter = 0)
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
    treap t1 (11); t1.priority = 3;
    treap t2 (9);  t2.priority = 7;
    treap t3 (14); t3.priority = 4;
    treap t4 (13); t4.priority = 8;
    
    treap t11(0);
    
    //unite (&t1, &t2, &t11);
    t11 = unionTreap(&t1, &t2);
    dump (&t11);
    printf ("*****************************\n");
    
    t2 = *merge (&t1, &t2);
    t4 = *merge (&t3, &t4);
    t4 = *merge (&t4, &t2);
    
    dumpTreap (&t4);
    printf ("*****************************\n");
    
    treap t5 (0), t6 (0);
    auto tmp = split (&t5, &t6, &t4, 13);
    
    dumpTreap (&t5);
    printf ("*****************************\n");
    dumpTreap (&t6);
    printf ("*****************************\n");
    return 0;
}


