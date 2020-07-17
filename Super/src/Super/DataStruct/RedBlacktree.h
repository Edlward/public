/**
 * C++ ����: �����
 *
 * @author skywang
 * @date 2013/11/07
 */

#ifndef _RED_BLACK_TREE_HPP_
#define _RED_BLACK_TREE_HPP_ 

#include <iomanip>
#include <iostream>
using namespace std;

enum RBTColor
{
    RED, 
    BLACK
};

template <class T>
class RedBlackTreeNode
{
    public:
        T key;                                   // �ؼ���(��ֵ)
        RBTColor color;                    // ��ɫ
        RedBlackTreeNode *left;      // ����
        RedBlackTreeNode *right;    // �Һ���
        RedBlackTreeNode *parent; // �����
        RedBlackTreeNode(T value, RBTColor c, RedBlackTreeNode *p, RedBlackTreeNode *l, RedBlackTreeNode *r):
            key(value),color(c),parent(),left(l),right(r) 
        {
        }
};

template <class T>
class RedBlackTree 
{
    private:
        RedBlackTreeNode<T> *mRoot;    // �����
    public:
        RedBlackTree();
        ~RedBlackTree();
        // ǰ�����"�����"
        void preOrder();
        // �������"�����"
        void inOrder();
        // �������"�����"
        void postOrder();

        // (�ݹ�ʵ��)����"�����"�м�ֵΪkey�Ľڵ�
        RedBlackTreeNode<T>* search(T key);
        // (�ǵݹ�ʵ��)����"�����"�м�ֵΪkey�Ľڵ�
        RedBlackTreeNode<T>* iterativeSearch(T key);

        // ������С��㣺������С���ļ�ֵ��
        T minimum();
        // ��������㣺���������ļ�ֵ��
        T maximum();

        // �ҽ��(x)�ĺ�̽�㡣��������"�����������ֵ���ڸý��"��"��С���"��
        RedBlackTreeNode<T>* successor(RedBlackTreeNode<T> *x);
        // �ҽ��(x)��ǰ����㡣��������"�����������ֵС�ڸý��"��"�����"��
        RedBlackTreeNode<T>* predecessor(RedBlackTreeNode<T> *x);

        // �����(keyΪ�ڵ��ֵ)���뵽�������
        void insert(T key);

        // ɾ�����(keyΪ�ڵ��ֵ)
        void remove(T key);

        // ���ٺ����
        void destroy();

        // ��ӡ�����
        void print();
    private:
        // ǰ�����"�����"
        void preOrder(RedBlackTreeNode<T>* pNode) const;
        // �������"�����"
        void inOrder(RedBlackTreeNode<T>* pNode) const;
        // �������"�����"
        void postOrder(RedBlackTreeNode<T>* pNode) const;

        // (�ݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�
        RedBlackTreeNode<T>* search(RedBlackTreeNode<T>* x, T key) const;
        // (�ǵݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�
        RedBlackTreeNode<T>* iterativeSearch(RedBlackTreeNode<T>* x, T key) const;

        // ������С��㣺����pNodeΪ�����ĺ��������С��㡣
        RedBlackTreeNode<T>* minimum(RedBlackTreeNode<T>* pNode);
        // ��������㣺����pNodeΪ�����ĺ����������㡣
        RedBlackTreeNode<T>* maximum(RedBlackTreeNode<T>* pNode);

        // ����
        void leftRotate(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* x);
        // ����
        void rightRotate(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* y);
        // ���뺯��
        void insert(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* node);
        // ������������
        void insertFixUp(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* node);
        // ɾ������
        void remove(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T> *node);
        // ɾ����������
        void removeFixUp(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T> *node, RedBlackTreeNode<T> *parent);

        // ���ٺ����
        void destroy(RedBlackTreeNode<T>* &pNode);

        // ��ӡ�����
        void print(RedBlackTreeNode<T>* pNode, T key, int direction);

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)
};

template <class T>
RedBlackTree<T>::RedBlackTree():mRoot(NULL)
{
    mRoot = NULL;
}

template <class T>
RedBlackTree<T>::~RedBlackTree() 
{
    destroy();
}


//ǰ�����"�����"
template <class T>
void RedBlackTree<T>::preOrder(RedBlackTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        cout<< pNode->key << " " ;
        preOrder(pNode->left);
        preOrder(pNode->right);
    }
}

template <class T>
void RedBlackTree<T>::preOrder() 
{
    preOrder(mRoot);
}


//�������"�����"

template <class T>
void RedBlackTree<T>::inOrder(RedBlackTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        inOrder(pNode->left);
        cout<< pNode->key << " " ;
        inOrder(pNode->right);
    }
}

template <class T>
void RedBlackTree<T>::inOrder() 
{
    inOrder(mRoot);
}


//�������"�����"

template <class T>
void RedBlackTree<T>::postOrder(RedBlackTreeNode<T>* pNode) const
{
    if(pNode != NULL)
    {
        postOrder(pNode->left);
        postOrder(pNode->right);
        cout<< pNode->key << " " ;
    }
}

template <class T>
void RedBlackTree<T>::postOrder() 
{
    postOrder(mRoot);
}


//(�ݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::search(RedBlackTreeNode<T>* x, T key) const
{
    if (x==NULL || x->key==key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::search(T key) 
{
    search(mRoot, key);
}


// (�ǵݹ�ʵ��)����"�����x"�м�ֵΪkey�Ľڵ�

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::iterativeSearch(RedBlackTreeNode<T>* x, T key) const
{
    while ((x!=NULL) && (x->key!=key))
    {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::iterativeSearch(T key)
{
    iterativeSearch(mRoot, key);
}

 
// ������С��㣺����pNodeΪ�����ĺ��������С��㡣

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::minimum(RedBlackTreeNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->left != NULL)
        pNode = pNode->left;
    return pNode;
}

template <class T>
T RedBlackTree<T>::minimum()
{
    RedBlackTreeNode<T> *p = minimum(mRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}
 

// ��������㣺����pNodeΪ�����ĺ����������㡣

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::maximum(RedBlackTreeNode<T>* pNode)
{
    if (pNode == NULL)
        return NULL;

    while(pNode->right != NULL)
        pNode = pNode->right;
    return pNode;
}

template <class T>
T RedBlackTree<T>::maximum()
{
    RedBlackTreeNode<T> *p = maximum(mRoot);
    if (p != NULL)
        return p->key;

    return (T)NULL;
}


//�ҽ��(x)�ĺ�̽�㡣��������"�����������ֵ���ڸý��"��"��С���"��

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::successor(RedBlackTreeNode<T> *x)
{
    // ���x�����Һ��ӣ���"x�ĺ�̽��"Ϊ "�����Һ���Ϊ������������С���"��
    if (x->right != NULL)
        return minimum(x->right);

    // ���xû���Һ��ӡ���x���������ֿ��ܣ�
    // (01) x��"һ������"����"x�ĺ�̽��"Ϊ "���ĸ����"��
    // (02) x��"һ���Һ���"�������"x����͵ĸ���㣬���Ҹø����Ҫ��������"���ҵ������"��͵ĸ����"����"x�ĺ�̽��"��
    RedBlackTreeNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->right))
    {
        x = y;
        y = y->parent;
    }

    return y;
}
 

//�ҽ��(x)��ǰ����㡣��������"�����������ֵС�ڸý��"��"�����"��

template <class T>
RedBlackTreeNode<T>* RedBlackTree<T>::predecessor(RedBlackTreeNode<T> *x)
{
    // ���x�������ӣ���"x��ǰ�����"Ϊ "��������Ϊ���������������"��
    if (x->left != NULL)
        return maximum(x->left);

    // ���xû�����ӡ���x���������ֿ��ܣ�
    // (01) x��"һ���Һ���"����"x��ǰ�����"Ϊ "���ĸ����"��
    // (01) x��"һ������"�������"x����͵ĸ���㣬���Ҹø����Ҫ�����Һ���"���ҵ������"��͵ĸ����"����"x��ǰ�����"��
    RedBlackTreeNode<T>* y = x->parent;
    while ((y!=NULL) && (x==y->left))
    {
        x = y;
        y = y->parent;
    }

    return y;
}

/* 
 * �Ժ�����Ľڵ�(x)��������ת
 *
 * ����ʾ��ͼ(�Խڵ�x��������)��
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \      --(����)-->           / \                #
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 *
 */
template <class T>
void RedBlackTree<T>::leftRotate(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* x)
{
    // ����x���Һ���Ϊy
    RedBlackTreeNode<T> *y = x->right;

    // �� ��y�����ӡ� ��Ϊ ��x���Һ��ӡ���
    // ���y�����ӷǿգ��� ��x�� ��Ϊ ��y�����ӵĸ��ס�
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // �� ��x�ĸ��ס� ��Ϊ ��y�ĸ��ס�
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        root = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
        else
            x->parent->right = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
    }
    
    // �� ��x�� ��Ϊ ��y�����ӡ�
    y->left = x;
    // �� ��x�ĸ��ڵ㡱 ��Ϊ ��y��
    x->parent = y;
}

/* 
 * �Ժ�����Ľڵ�(y)��������ת
 *
 * ����ʾ��ͼ(�Խڵ�y��������)��
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(����)-->            /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 * 
 */
template <class T>
void RedBlackTree<T>::rightRotate(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* y)
{
    // ����x�ǵ�ǰ�ڵ�����ӡ�
    RedBlackTreeNode<T> *x = y->left;

    // �� ��x���Һ��ӡ� ��Ϊ ��y�����ӡ���
    // ���"x���Һ���"��Ϊ�յĻ����� ��y�� ��Ϊ ��x���Һ��ӵĸ��ס�
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // �� ��y�ĸ��ס� ��Ϊ ��x�ĸ��ס�
    x->parent = y->parent;

    if (y->parent == NULL) 
    {
        root = x;            // ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // ��� y�������ڵ���Һ��ӣ���x��Ϊ��y�ĸ��ڵ���Һ��ӡ�
        else
            y->parent->left = x;    // (y�������ڵ������) ��x��Ϊ��x�ĸ��ڵ�����ӡ�
    }

    // �� ��y�� ��Ϊ ��x���Һ��ӡ�
    x->right = y;

    // �� ��y�ĸ��ڵ㡱 ��Ϊ ��x��
    y->parent = x;
}

/*
 * �����������������
 *
 * ���������в���ڵ�֮��(ʧȥƽ��)���ٵ��øú�����
 * Ŀ���ǽ������������һ�ź������
 *
 * ����˵����
 *     root ������ĸ�
 *     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�z
 */
template <class T>
void RedBlackTree<T>::insertFixUp(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* node)
{
    RedBlackTreeNode<T> *parent, *gparent;

    // �������ڵ���ڣ����Ҹ��ڵ����ɫ�Ǻ�ɫ��
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //�������ڵ㡱�ǡ��游�ڵ�����ӡ�
        if (parent == gparent->left)
        {
            // Case 1����������ڵ��Ǻ�ɫ
            {
                RedBlackTreeNode<T> *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
            if (parent->right == node)
            {
                RedBlackTreeNode<T> *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } 
        else//����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�
        {
            // Case 1����������ڵ��Ǻ�ɫ
            {
                RedBlackTreeNode<T> *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
            if (parent->left == node)
            {
                RedBlackTreeNode<T> *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // �����ڵ���Ϊ��ɫ
    rb_set_black(root);
}

/* 
 * �������뵽�������
 *
 * ����˵����
 *     root ������ĸ����
 *     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�node
 */
template <class T>
void RedBlackTree<T>::insert(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T>* node)
{
    RedBlackTreeNode<T> *y = NULL;
    RedBlackTreeNode<T> *x = root;

    // 1. �����������һ�Ŷ�������������ڵ���ӵ�����������С�
    while (x != NULL)
    {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y!=NULL)
    {
        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
    else
        root = node;

    // 2. ���ýڵ����ɫΪ��ɫ
    node->color = RED;

    // 3. ������������Ϊһ�Ŷ��������
    insertFixUp(root, node);
}

/* 
 * �����(keyΪ�ڵ��ֵ)���뵽�������
 *
 * ����˵����
 *     pNode ������ĸ����
 *     key ������ļ�ֵ
 */
template <class T>
void RedBlackTree<T>::insert(T key)
{
    RedBlackTreeNode<T> *z=NULL;

    // ����½����ʧ�ܣ��򷵻ء�
    if ((z=new RedBlackTreeNode<T>(key,BLACK,NULL,NULL,NULL)) == NULL)
        return ;

    insert(mRoot, z);
}

/*
 * �����ɾ����������
 *
 * �ڴӺ������ɾ������ڵ�֮��(�����ʧȥƽ��)���ٵ��øú�����
 * Ŀ���ǽ������������һ�ź������
 *
 * ����˵����
 *     root ������ĸ�
 *     node �������Ľڵ�
 */
template <class T>
void RedBlackTree<T>::removeFixUp(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T> *node, RedBlackTreeNode<T> *parent)
{
    RedBlackTreeNode<T> *other;

    while ((!node || rb_is_black(node)) && node != root)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x���ֵ�w�Ǻ�ɫ��  
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x���ֵ�w�Ǻ�ɫ��  
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��  
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��  
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/* 
 * ɾ�����(node)�������ر�ɾ���Ľ��
 * ����˵����
 *     root ������ĸ����
 *     node ɾ���Ľ��
 */
template <class T>
void RedBlackTree<T>::remove(RedBlackTreeNode<T>* &root, RedBlackTreeNode<T> *node)
{
    RedBlackTreeNode<T> *child, *parent;
    RBTColor color;

    // ��ɾ���ڵ��"���Һ��Ӷ���Ϊ��"�������
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        // ��ɾ�ڵ�ĺ�̽ڵ㡣(��Ϊ"ȡ���ڵ�")
        // ������ȡ��"��ɾ�ڵ�"��λ�ã�Ȼ���ٽ�"��ɾ�ڵ�"ȥ����
        RedBlackTreeNode<T> *replace = node;

        // ��ȡ��̽ڵ�
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node�ڵ�"���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } 
        else 
            // "node�ڵ�"�Ǹ��ڵ㣬���¸��ڵ㡣
            root = replace;

        // child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ"�����Ľڵ�"��
        // "ȡ���ڵ�"�϶����������ӣ���Ϊ����һ����̽ڵ㡣
        child = replace->right;
        parent = rb_parent(replace);
        // ����"ȡ���ڵ�"����ɫ
        color = rb_color(replace);

        // "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�"
        if (parent == node)
        {
            parent = replace;
        } 
        else
        {
            // child��Ϊ��
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // ����"ȡ���ڵ�"����ɫ
    color = node->color;

    if (child)
        child->parent = parent;

    // "node�ڵ�"���Ǹ��ڵ�
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/* 
 * ɾ��������м�ֵΪkey�Ľڵ�
 * ����˵����
 *     pNode ������ĸ����
 */

template <class T>
void RedBlackTree<T>::remove(T key)
{
    RedBlackTreeNode<T> *node; 

    // ����key��Ӧ�Ľڵ�(node)���ҵ��Ļ���ɾ���ýڵ�
    if ((node = search(mRoot, key)) != NULL)
        remove(mRoot, node);
}


// ���ٺ����
template <class T>
void RedBlackTree<T>::destroy(RedBlackTreeNode<T>* &pNode)
{
    if (pNode==NULL)
        return ;

    if (pNode->left != NULL)
        return destroy(pNode->left);
    if (pNode->right != NULL)
        return destroy(pNode->right);

    delete pNode;
    pNode=NULL;
}

template <class T>
void RedBlackTree<T>::destroy()
{
    destroy(mRoot);
}

/*
 * ��ӡ"���������"
 *
 * key        -- �ڵ�ļ�ֵ 
 * direction  --  0����ʾ�ýڵ��Ǹ��ڵ�;
 *               -1����ʾ�ýڵ������ĸ���������;
 *                1����ʾ�ýڵ������ĸ������Һ��ӡ�
 */
template <class T>
void RedBlackTree<T>::print(RedBlackTreeNode<T>* pNode, T key, int direction)
{
    if(pNode != NULL)
    {
        if(direction==0)    // pNode�Ǹ��ڵ�
            cout << setw(2) << pNode->key << "(B) is root" << endl;
        else                // pNode�Ƿ�֧�ڵ�
            cout << setw(2) << pNode->key <<  (rb_is_red(pNode)?"(R)":"(B)") << " is " << setw(2) << key << "'s "  << setw(12) << (direction==1?"right child" : "left child") << endl;

        print(pNode->left, pNode->key, -1);
        print(pNode->right,pNode->key,  1);
    }
}

template <class T>
void RedBlackTree<T>::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}

#ifdef UNIT_TEST
int Test_RedBlackTree();
#endif


#endif



