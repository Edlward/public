#ifndef _Tree_H_
#define _Tree_H_

#include <iostream>
using namespace std;


#ifdef __cplusplus





//¶þ²æÊ÷
template<typename T> class BinaryTree2;

template<typename T> class BinTreeNode2{
public:
	friend class BinaryTree2<T>;
	BinTreeNode2():m_pleft(NULL),m_pright(NULL){}
	BinTreeNode2(T item,BinTreeNode2<T> *left=NULL,BinTreeNode2<T> *right=NULL)
		:m_data(item),m_pleft(left),m_pright(right){}

	T GetData() const;		//get thd data
	BinTreeNode2<T> *GetLeft() const;		//get the left node
	BinTreeNode2<T> *GetRight() const;	//get the right node

	void SetData(const T data);			//change the data
	void SetLeft(const BinTreeNode2<T> *left);	//change thd left node
	void SetRight(const BinTreeNode2<T> *right);	//change the right node

	void InOrder();		//inorder the tree with the root of the node
	void PreOrder();	//perorder the tree with the root of the node
	void PostOrder();	//postoder the tree with the root of the node
	
	int Size();			//get size
	int Height();		//get height
	BinTreeNode2<T> *Copy(const BinTreeNode2<T> *copy);	//copy the node
	void Destroy(){		//destroy the tree with the root of the node
		if(this!=NULL){
			this->m_pleft->Destroy();
			this->m_pright->Destroy();
			delete this;
		}
	}

	friend bool equal<T>(const BinTreeNode2<T> *s,const BinTreeNode2<T> *t);	//is equal?

private:
	BinTreeNode2<T> *m_pleft,*m_pright;
	T m_data;
};

template<typename T> T BinTreeNode2<T>::GetData() const
{
	return this!=NULL?m_data:-1;
}

template<typename T> BinTreeNode2<T>* BinTreeNode2<T>::GetLeft() const
{
	return this!=NULL?m_pleft:NULL;
}

template<typename T> BinTreeNode2<T>* BinTreeNode2<T>::GetRight() const
{
	return this!=NULL?m_pright:NULL;
}

template<typename T> void BinTreeNode2<T>::SetData(const T data)
{
	if(this!=NULL)
    {
		m_data=data;
	}
}

template<typename T> void BinTreeNode2<T>::SetLeft(const BinTreeNode2<T> *left)
{
	if(this!=NULL)
    {
		m_pleft=left;
	}
}

template<typename T> void BinTreeNode2<T>::SetRight(const BinTreeNode2<T> *right)
{
	if(this!=NULL)
    {
		m_pright=right;
	}
}

template<typename T> BinTreeNode2<T>* BinTreeNode2<T>::Copy(const BinTreeNode2<T> *copy)
{
	if(copy==NULL)
    {
		return NULL;
	}

	BinTreeNode2<T> *temp=new BinTreeNode2<T>(copy->m_data);
	temp->m_pleft=Copy(copy->m_pleft);
	temp->m_pright=Copy(copy->m_pright);
	return temp;
}

template<typename T> bool equal(const BinTreeNode2<T> *s,const BinTreeNode2<T> *t)
{
	if(s==NULL&&t==NULL)
    {
		return 1;
	}
	if(s&&t&&s->m_data==t->m_data&&equal(s->m_pleft,t->m_pleft)&&equal(s->m_pright,t->m_pright))
    {
		return 1;
	}
	return 0;
}

template<typename T> void BinTreeNode2<T>::InOrder()
{
	if(this!=NULL)
    {
		this->m_pleft->InOrder();
		cout<<"--->"<<this->m_data;
		this->m_pright->InOrder();
	}
}

template<typename T> void BinTreeNode2<T>::PreOrder(){
	if(this!=NULL)
    {
		cout<<"--->"<<this->m_data;
		this->m_pleft->PreOrder();
		this->m_pright->PreOrder();
	}
}

template<typename T> void BinTreeNode2<T>::PostOrder(){
	if(this!=NULL)
    {
		this->m_pleft->PostOrder();
		this->m_pright->PostOrder();
		cout<<"--->"<<this->m_data;
	}
}

template<typename T> int BinTreeNode2<T>::Size()
{
	if(this==NULL)
    {
		return 0;
	}
	return 1+this->m_pleft->Size()+this->m_pright->Size();
}

template<typename T> int BinTreeNode2<T>::Height()
{
	if(this==NULL)
    {
		return -1;
	}
	int lheight,rheight;
	lheight=this->m_pleft->Height();
	rheight=this->m_pright->Height();
	return 1+(lheight>rheight?lheight:rheight);
}

template<typename T> class BinaryTree2
{
public:
    BinaryTree2():m_proot(NULL){}
    BinaryTree2(const T stop):m_stop(stop),m_proot(NULL){}
    BinaryTree2(BinaryTree2<T>& copy);
    virtual ~BinaryTree2()
    {
        m_proot->Destroy();
    }
    virtual bool IsEmpty()
    {		//is empty?
        return m_proot==NULL;
    }

    virtual BinTreeNode2<T> *GetLeft(BinTreeNode2<T> *current);	//get the left node
    virtual BinTreeNode2<T> *GetRight(BinTreeNode2<T> *current);//get the right node
    virtual BinTreeNode2<T> *GetParent(BinTreeNode2<T> *current);//ghe thd parent
    const BinTreeNode2<T> *GetRoot() const;	//get root

    virtual bool Insert(const T item);		//insert a new node
    virtual BinTreeNode2<T> *Find(const T item) const;	//find thd node with the data

    void InOrder();	
    void PreOrder();
    void PostOrder();

    int Size();		//get size
    int Height();	//get height

    BinaryTree2<T>& operator=(const BinaryTree2<T> copy);	//evaluate node

    friend bool operator== <T>(const BinaryTree2<T> s,const BinaryTree2<T> t);//is equal?
    friend ostream& operator<< <T>(ostream& ,BinaryTree2<T>&);	//output the data
    friend istream& operator>> <T>(istream& ,BinaryTree2<T>&);	//input the data

private:
    T m_stop;		//just using for input the data;
    BinTreeNode2<T> *m_proot;

    //find the parent of current in the tree with the root of start
    BinTreeNode2<T> *GetParent(BinTreeNode2<T> *start,BinTreeNode2<T> *current);
    void Print(BinTreeNode2<T> *start,int n=0);	//print the tree with the root of start
};

template<typename T> BinaryTree2<T>::BinaryTree2(BinaryTree2<T>& copy)
{
    if(copy.m_proot)
    {
        this->m_stop=copy.m_stop;
    }
    m_proot=m_proot->Copy(copy.m_proot);
}
template<typename T> BinTreeNode2<T>* BinaryTree2<T>::GetLeft(BinTreeNode2<T> *current)
{
    return m_proot&&current?current->m_pleft:NULL;
}

template<typename T> BinTreeNode2<T>* BinaryTree2<T>::GetRight(BinTreeNode2<T> *current)
{
    return m_proot&&current?current->m_pright:NULL;
}

template<typename T> const BinTreeNode2<T>* BinaryTree2<T>::GetRoot() const
{
    return m_proot;
}

template<typename T> BinTreeNode2<T>* BinaryTree2<T>::GetParent(BinTreeNode2<T> *start, BinTreeNode2<T> *current)
{
    if(start==NULL||current==NULL)
    {
        return NULL;
    }
    if(start->m_pleft==current||start->m_pright==current)
    {
        return start;
    }
    BinTreeNode2<T> *pmove;  //find the parent in the left subtree
    if((pmove=GetParent(start->m_pleft,current))!=NULL)
    {
        return pmove;
    }
    else  //find the parent in the right subtree
    {
        return GetParent(start->m_pright,current);	
    }
}

template<typename T> BinTreeNode2<T>* BinaryTree2<T>::GetParent(BinTreeNode2<T> *current)
{
    return m_proot==NULL||current==m_proot?NULL:GetParent(m_proot,current);	
}


template<typename T> bool BinaryTree2<T>::Insert(const T item)
{
    BinTreeNode2<T> *pstart=m_proot,*newnode=new BinTreeNode2<T>(item);
    if(m_proot==NULL)
    {
        m_proot=newnode;
        return 1;
    }
    while(1)
    {
        if(item==pstart->m_data)
        {
            cout<<"The item "<<item<<" is exist!"<<endl;
            return 0;
        }
        if(item<pstart->m_data)
        {
            if(pstart->m_pleft==NULL)
            {
                pstart->m_pleft=newnode;
                return 1;
            }
            pstart=pstart->m_pleft;	//if less than the node then insert to the left subtree
        }
        else
        {
            if(pstart->m_pright==NULL)
            {
                pstart->m_pright=newnode;
                return 1;
            }
            pstart=pstart->m_pright;//if more than the node then insert to the right subtree
        }
    }
}

template<typename T> BinTreeNode2<T>* BinaryTree2<T>::Find(const T item) const
{
    BinTreeNode2<T> *pstart=m_proot;
    while(pstart)
    {
        if(item==pstart->m_data)
        {
            return pstart;
        }
        if(item<pstart->m_data)
        {
            pstart=pstart->m_pleft;	//if less than the node then find in the left subtree
        }
        else
        {
            pstart=pstart->m_pright;//if more than the node then find in the right subtree
        }
    }
    return NULL;
}

template<typename T> void BinaryTree2<T>::Print(BinTreeNode2<T> *start, int n)
{
    if(start==NULL)
    {
        for(int i=0;i<n;i++)
        {
            cout<<"     ";
        }
        cout<<"NULL"<<endl;
        return;
    }
    Print(start->m_pright,n+1);	//print the right subtree
    for(int i=0;i<n;i++)    //print blanks with the height of the node
    {	
        cout<<"     ";
    }
    if(n>=0)
    {
        cout<<start->m_data<<"--->"<<endl;//print the node
    }
    Print(start->m_pleft,n+1);	//print the left subtree
}

template<typename T> BinaryTree2<T>& BinaryTree2<T>::operator=(const BinaryTree2<T> copy)
{
    if(copy.m_proot)
    {
        this->m_stop=copy.m_stop;
    }
    m_proot=m_proot->Copy(copy.m_proot);
    return *this;
}

template<typename T> ostream& operator<<(ostream& os,BinaryTree2<T>& out)
{
    out.Print(out.m_proot);
    return os;
}

template<typename T> istream& operator>>(istream& is,BinaryTree2<T>& in)
{
    T item;
    cout<<"initialize the tree:"<<endl<<"Input data(end with "<<in.m_stop<<"!):";
    is>>item;
    while(item!=in.m_stop)
    {	//m_stop is the end of input
        in.Insert(item);
        is>>item;
    }
    return is;
}

template<typename T> bool operator==(const BinaryTree2<T> s,const BinaryTree2<T> t)
{
    return equal(s.m_proot,t.m_proot);
}

template<typename T> void BinaryTree2<T>::InOrder()
{
    this->m_proot->InOrder();
}

template<typename T> void BinaryTree2<T>::PreOrder()
{
    this->m_proot->PreOrder();
}

template<typename T> void BinaryTree2<T>::PostOrder()
{
    this->m_proot->PostOrder();
}

template<typename T> int BinaryTree2<T>::Size()
{
    return this->m_proot->Size();

}

template<typename T> int BinaryTree2<T>::Height()
{
    return this->m_proot->Height();
}














#endif


#ifdef UNIT_TEST
int Test_Tree();
#endif

#endif

