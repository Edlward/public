#ifndef _Tree_H_
#define _Tree_H_

#include <iostream>
using namespace std;



#ifdef __cplusplus




 //普通树的实现
template<typename T>
class NormalTree
{
public:
    //普通树的实现，兄弟孩子表示法，插入数据，查询数据，前序，中序，后序遍历
    //typedef int ElementDataType;
    typedef T ElementDataType;
    struct NormalTreeNode
    {
        ElementDataType data;
        //T data;
        NormalTreeNode*  pFirstChild;
        NormalTreeNode*  pNextBrother;
        NormalTreeNode(ElementDataType Value )
        {
            data = Value;
            pFirstChild = NULL;
            pNextBrother = NULL;
        }
    }; 
    //typedef struct NormalTreeNode* NormalTreeNode*;
public:
    //NormalTree();
    //NormalTree(ElementDataType Value);
    //~NormalTree();
    //void Insert( ElementDataType parentValue, ElementDataType Value );	//	parentValue：该节点的父结点；Value：该节点的值
    //void InsertBrother( NormalTreeNode*  pParentNode, ElementDataType Value );
    //NormalTreeNode*  Search(NormalTreeNode*  pNode, ElementDataType Value );
    //void Preorder(NormalTreeNode*  pNode );        //前序遍历
    //void Inorder(NormalTreeNode*  pNode );          //中序遍历
    //void postorder(NormalTreeNode*  pNode );      //后续遍历
    //void PrintNode(NormalTreeNode*  pNode );
    //void FreeMemory(NormalTreeNode*  pNode );  //释放内存
private:
    NormalTreeNode*  pRoot;
public:
    NormalTree::NormalTree():pRoot(NULL)
    {
        //pRoot = NULL;
    }
    NormalTree::NormalTree(ElementDataType Value):pRoot(NULL)
    {
        pRoot = new NormalTreeNode(Value);
        //if (pRoot==NULL )
        //    return;
    }
    NormalTree::~NormalTree()
    {
        //if (pRoot == NULL )
        //    return;
        FreeMemory( pRoot );
    }

    void NormalTree::FreeMemory( NormalTreeNode*  pNode )
    {
        if ( pNode == NULL )
            return;

        if ( pNode->pFirstChild != NULL )
            FreeMemory( pNode->pFirstChild );

        if ( pNode->pNextBrother != NULL )
            FreeMemory( pNode->pNextBrother );

        delete pNode;
        pNode = NULL;
    }

    void NormalTree::Insert( ElementDataType parentValue, ElementDataType Value )
    {
        if ( pRoot == NULL )
            return;

        NormalTreeNode*  pFindNode = Search( pRoot, parentValue );
        if ( pFindNode == NULL )
            return;

        if ( pFindNode->pFirstChild == NULL )
        {
            pFindNode->pFirstChild = new NormalTreeNode( Value );
            return;
        }
        else
        {
            InsertBrother( pFindNode->pFirstChild, Value );
            return;
        }
    }

    void NormalTree::InsertBrother( NormalTreeNode*  pBrotherNode, ElementDataType Value )
    {
        if ( pBrotherNode->pNextBrother != NULL )
            InsertBrother( pBrotherNode->pNextBrother, Value );
        else
        {
            pBrotherNode->pNextBrother = new NormalTreeNode( Value );
            return;
        }
    }

    NormalTreeNode*  NormalTree::Search(NormalTreeNode*  pNode, ElementDataType Value )
    {
        if ( pNode == NULL )
            return NULL;

        if ( pNode->data == Value )
            return pNode;

        if ( pNode->pFirstChild == NULL && pNode->pNextBrother == NULL )
            return NULL;
        else
        {
            if ( pNode->pFirstChild != NULL )
            {
                NormalTreeNode*  pNodeTemp = Search( pNode->pFirstChild, Value );
                if ( pNodeTemp != NULL )
                    return pNodeTemp;
                else
                {
                    return Search( pNode->pNextBrother, Value );
                }
            }
            else
                return Search( pNode->pNextBrother, Value );
        }
    }

    void NormalTree::Preorder( NormalTreeNode*  pNode )
    {
        if (pNode == NULL)
            return;
        cout << " " << pNode->data << " ";

        Preorder( pNode->pFirstChild );
        Preorder( pNode->pNextBrother );
    }

    void NormalTree::Inorder( NormalTreeNode*  pNode )
    {
        if ( pNode == NULL )
            return;

        Inorder( pNode->pFirstChild );
        cout << " " << pNode->data << " ";
        Inorder( pNode->pNextBrother );
    }

    void NormalTree::postorder( NormalTreeNode*  pNode )
    {
        if ( pNode == NULL )
            return;

        postorder( pNode->pFirstChild );
        postorder( pNode->pNextBrother );
        cout << " " << pNode->data << " ";
    }
};













//二叉树
template<typename Type> class BinaryTree;

template<typename Type> class BinTreeNode{
public:
	friend class BinaryTree<Type>;
	BinTreeNode():m_pleft(NULL),m_pright(NULL){}
	BinTreeNode(Type item,BinTreeNode<Type> *left=NULL,BinTreeNode<Type> *right=NULL)
		:m_data(item),m_pleft(left),m_pright(right){}

	Type GetData() const;		//get thd data
	BinTreeNode<Type> *GetLeft() const;		//get the left node
	BinTreeNode<Type> *GetRight() const;	//get the right node

	void SetData(const Type data);			//change the data
	void SetLeft(const BinTreeNode<Type> *left);	//change thd left node
	void SetRight(const BinTreeNode<Type> *right);	//change the right node

	void InOrder();		//inorder the tree with the root of the node
	void PreOrder();	//perorder the tree with the root of the node
	void PostOrder();	//postoder the tree with the root of the node
	
	int Size();			//get size
	int Height();		//get height
	BinTreeNode<Type> *Copy(const BinTreeNode<Type> *copy);	//copy the node
	void Destroy(){		//destroy the tree with the root of the node
		if(this!=NULL){
			this->m_pleft->Destroy();
			this->m_pright->Destroy();
			delete this;
		}
	}

	friend bool equal<Type>(const BinTreeNode<Type> *s,const BinTreeNode<Type> *t);	//is equal?

private:
	BinTreeNode<Type> *m_pleft,*m_pright;
	Type m_data;
};

template<typename Type> Type BinTreeNode<Type>::GetData() const{
	return this!=NULL?m_data:-1;
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetLeft() const{
	return this!=NULL?m_pleft:NULL;
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::GetRight() const{
	return this!=NULL?m_pright:NULL;
}

template<typename Type> void BinTreeNode<Type>::SetData(const Type data){
	if(this!=NULL){
		m_data=data;
	}
}

template<typename Type> void BinTreeNode<Type>::SetLeft(const BinTreeNode<Type> *left){
	if(this!=NULL){
		m_pleft=left;
	}
}

template<typename Type> void BinTreeNode<Type>::SetRight(const BinTreeNode<Type> *right){
	if(this!=NULL){
		m_pright=right;
	}
}

template<typename Type> BinTreeNode<Type>* BinTreeNode<Type>::Copy(const BinTreeNode<Type> *copy){
	if(copy==NULL){
		return NULL;
	}

	BinTreeNode<Type> *temp=new BinTreeNode<Type>(copy->m_data);
	temp->m_pleft=Copy(copy->m_pleft);
	temp->m_pright=Copy(copy->m_pright);
	return temp;
}

template<typename Type> bool equal(const BinTreeNode<Type> *s,const BinTreeNode<Type> *t){
	if(s==NULL&&t==NULL){
		return 1;
	}
	if(s&&t&&s->m_data==t->m_data&&equal(s->m_pleft,t->m_pleft)&&equal(s->m_pright,t->m_pright)){
		return 1;
	}
	return 0;
}

template<typename Type> void BinTreeNode<Type>::InOrder(){
	if(this!=NULL){
		this->m_pleft->InOrder();
		cout<<"--->"<<this->m_data;
		this->m_pright->InOrder();
	}
}

template<typename Type> void BinTreeNode<Type>::PreOrder(){
	if(this!=NULL){
		cout<<"--->"<<this->m_data;
		this->m_pleft->PreOrder();
		this->m_pright->PreOrder();
	}
}

template<typename Type> void BinTreeNode<Type>::PostOrder(){
	if(this!=NULL){
		this->m_pleft->PostOrder();
		this->m_pright->PostOrder();
		cout<<"--->"<<this->m_data;
	}
}

template<typename Type> int BinTreeNode<Type>::Size(){
	if(this==NULL){
		return 0;
	}
	return 1+this->m_pleft->Size()+this->m_pright->Size();
}

template<typename Type> int BinTreeNode<Type>::Height(){
	if(this==NULL){
		return -1;
	}
	int lheight,rheight;
	lheight=this->m_pleft->Height();
	rheight=this->m_pright->Height();
	return 1+(lheight>rheight?lheight:rheight);
}

template<typename Type> class BinaryTree{
public:
    BinaryTree():m_proot(NULL){}
    BinaryTree(const Type stop):m_stop(stop),m_proot(NULL){}
    BinaryTree(BinaryTree<Type>& copy);
    virtual ~BinaryTree(){
        m_proot->Destroy();
    }
    virtual bool IsEmpty(){		//is empty?
        return m_proot==NULL;
    }

    virtual BinTreeNode<Type> *GetLeft(BinTreeNode<Type> *current);	//get the left node
    virtual BinTreeNode<Type> *GetRight(BinTreeNode<Type> *current);//get the right node
    virtual BinTreeNode<Type> *GetParent(BinTreeNode<Type> *current);//ghe thd parent
    const BinTreeNode<Type> *GetRoot() const;	//get root

    virtual bool Insert(const Type item);		//insert a new node
    virtual BinTreeNode<Type> *Find(const Type item) const;	//find thd node with the data

    void InOrder();	
    void PreOrder();
    void PostOrder();

    int Size();		//get size
    int Height();	//get height

    BinaryTree<Type>& operator=(const BinaryTree<Type> copy);	//evaluate node

    friend bool operator== <Type>(const BinaryTree<Type> s,const BinaryTree<Type> t);//is equal?
    friend ostream& operator<< <Type>(ostream& ,BinaryTree<Type>&);	//output the data
    friend istream& operator>> <Type>(istream& ,BinaryTree<Type>&);	//input the data

private:
    Type m_stop;		//just using for input the data;
    BinTreeNode<Type> *m_proot;

    //find the parent of current in the tree with the root of start
    BinTreeNode<Type> *GetParent(BinTreeNode<Type> *start,BinTreeNode<Type> *current);
    void Print(BinTreeNode<Type> *start,int n=0);	//print the tree with the root of start
};

template<typename Type> BinaryTree<Type>::BinaryTree(BinaryTree<Type>& copy){
    if(copy.m_proot){
        this->m_stop=copy.m_stop;
    }
    m_proot=m_proot->Copy(copy.m_proot);
}
template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetLeft(BinTreeNode<Type> *current){
    return m_proot&&current?current->m_pleft:NULL;
}

template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetRight(BinTreeNode<Type> *current){
    return m_proot&&current?current->m_pright:NULL;
}

template<typename Type> const BinTreeNode<Type>* BinaryTree<Type>::GetRoot() const{
    return m_proot;
}

template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetParent(BinTreeNode<Type> *start, BinTreeNode<Type> *current){
    if(start==NULL||current==NULL){
        return NULL;
    }
    if(start->m_pleft==current||start->m_pright==current){
        return start;
    }
    BinTreeNode<Type> *pmove;
    if((pmove=GetParent(start->m_pleft,current))!=NULL){//find the parent in the left subtree
        return pmove;
    }
    else{
        return GetParent(start->m_pright,current);	//find the parent in the right subtree
    }
}

template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::GetParent(BinTreeNode<Type> *current){
    return m_proot==NULL||current==m_proot?NULL:GetParent(m_proot,current);	
}


template<typename Type> bool BinaryTree<Type>::Insert(const Type item){
    BinTreeNode<Type> *pstart=m_proot,*newnode=new BinTreeNode<Type>(item);
    if(m_proot==NULL){
        m_proot=newnode;
        return 1;
    }
    while(1){
        if(item==pstart->m_data){
            cout<<"The item "<<item<<" is exist!"<<endl;
            return 0;
        }
        if(item<pstart->m_data){
            if(pstart->m_pleft==NULL){
                pstart->m_pleft=newnode;
                return 1;
            }
            pstart=pstart->m_pleft;	//if less than the node then insert to the left subtree
        }
        else{
            if(pstart->m_pright==NULL){
                pstart->m_pright=newnode;
                return 1;
            }
            pstart=pstart->m_pright;//if more than the node then insert to the right subtree
        }
    }
}

template<typename Type> BinTreeNode<Type>* BinaryTree<Type>::Find(const Type item) const{
    BinTreeNode<Type> *pstart=m_proot;
    while(pstart){
        if(item==pstart->m_data){
            return pstart;
        }
        if(item<pstart->m_data){
            pstart=pstart->m_pleft;	//if less than the node then find in the left subtree
        }
        else{
            pstart=pstart->m_pright;//if more than the node then find in the right subtree
        }
    }
    return NULL;
}

template<typename Type> void BinaryTree<Type>::Print(BinTreeNode<Type> *start, int n){
    if(start==NULL){
        for(int i=0;i<n;i++){
            cout<<"     ";
        }
        cout<<"NULL"<<endl;
        return;
    }
    Print(start->m_pright,n+1);	//print the right subtree
    for(int i=0;i<n;i++){	//print blanks with the height of the node
        cout<<"     ";
    }
    if(n>=0){
        cout<<start->m_data<<"--->"<<endl;//print the node
    }
    Print(start->m_pleft,n+1);	//print the left subtree
}

template<typename Type> BinaryTree<Type>& BinaryTree<Type>::operator=(const BinaryTree<Type> copy){
    if(copy.m_proot){
        this->m_stop=copy.m_stop;
    }
    m_proot=m_proot->Copy(copy.m_proot);
    return *this;
}

template<typename Type> ostream& operator<<(ostream& os,BinaryTree<Type>& out){
    out.Print(out.m_proot);
    return os;
}

template<typename Type> istream& operator>>(istream& is,BinaryTree<Type>& in){
    Type item;
    cout<<"initialize the tree:"<<endl<<"Input data(end with "<<in.m_stop<<"!):";
    is>>item;
    while(item!=in.m_stop){	//m_stop is the end of input
        in.Insert(item);
        is>>item;
    }
    return is;
}

template<typename Type> bool operator==(const BinaryTree<Type> s,const BinaryTree<Type> t){
    return equal(s.m_proot,t.m_proot);
}

template<typename Type> void BinaryTree<Type>::InOrder(){
    this->m_proot->InOrder();
}

template<typename Type> void BinaryTree<Type>::PreOrder(){
    this->m_proot->PreOrder();
}

template<typename Type> void BinaryTree<Type>::PostOrder(){
    this->m_proot->PostOrder();
}

template<typename Type> int BinaryTree<Type>::Size(){
    return this->m_proot->Size();

}

template<typename Type> int BinaryTree<Type>::Height(){
    return this->m_proot->Height();
}




//12、线索二叉树
//   ThreadNode.h

template<typename Type> class ThreadTree;
template<typename Type> class ThreadInorderIterator;

template<typename Type> class ThreadNode
{
public:
    friend class ThreadTree<Type>;
    friend class ThreadInorderIterator<Type>;
    ThreadNode():m_nleftthread(1),m_nrightthread(1){
        m_pleft=this;
        m_pright=this;
    }
    ThreadNode(const Type item):m_data(item),m_pleft(NULL),m_pright(NULL)
        ,m_nleftthread(0),m_nrightthread(0){}

private:
    int m_nleftthread,m_nrightthread;
    ThreadNode<Type> *m_pleft,*m_pright;
    Type m_data;
};


template<typename Type> class ThreadTree{
public:
    friend class ThreadInorderIterator<Type>;
    ThreadTree():m_proot(new ThreadNode<Type>()){}
private:
    ThreadNode<Type> *m_proot;
};

template<typename Type> class ThreadInorderIterator{
public:
    ThreadInorderIterator(ThreadTree<Type> &tree):m_ptree(tree),m_pcurrent(tree.m_proot){
        //InThread(m_ptree.m_proot->m_pleft,m_ptree.m_proot);
    }

    ThreadNode<Type> *First();
    ThreadNode<Type> *Prior();
    ThreadNode<Type> *Next();

    void Print();
    void Print(ThreadNode<Type> *start, int n=0);
    void InOrder();
    void InsertLeft(ThreadNode<Type> *left);
    void InsertRight(ThreadNode<Type> *right);
    ThreadNode<Type> *GetParent(ThreadNode<Type> *current);


private:
    ThreadTree<Type> &m_ptree;
    ThreadNode<Type> *m_pcurrent;
    void InThread(ThreadNode<Type> *current,ThreadNode<Type> *pre);
};

template<typename Type> void ThreadInorderIterator<Type>::InThread(
    ThreadNode<Type> *current, ThreadNode<Type> *pre){
        if(current!=m_ptree.m_proot){
            InThread(current->m_pleft,pre);
            if(current->m_pleft==NULL){
                current->m_pleft=pre;
                current->m_nleftthread=1;
            }
            if(pre->m_pright==NULL){
                pre->m_pright=current;
                pre->m_nrightthread=1;
            }

            pre=current;
            InThread(current->m_pright,pre);
        }
}

template<typename Type> ThreadNode<Type>* ThreadInorderIterator<Type>::First(){
    while(m_pcurrent->m_nleftthread==0){
        m_pcurrent=m_pcurrent->m_pleft;
    }
    return m_pcurrent;
}

template<typename Type> ThreadNode<Type>* ThreadInorderIterator<Type>::Prior(){
    ThreadNode<Type> *pmove=m_pcurrent->m_pleft;
    if(0==m_pcurrent->m_nleftthread){
        while(0==pmove->m_nrightthread){
            pmove=pmove->m_pright;
        }
    }
    m_pcurrent=pmove;
    if(m_pcurrent==m_ptree.m_proot){
        return NULL;
    }
    return m_pcurrent;
}

template<typename Type> ThreadNode<Type>* ThreadInorderIterator<Type>::Next(){
    ThreadNode<Type> *pmove=m_pcurrent->m_pright;
    if(0==m_pcurrent->m_nrightthread){
        while(0==pmove->m_nleftthread){
            pmove=pmove->m_pleft;
        }
    }
    m_pcurrent=pmove;
    if(m_pcurrent==m_ptree.m_proot){
        return NULL;
    }
    return m_pcurrent;
}

template<typename Type> void ThreadInorderIterator<Type>::InOrder(){
    ThreadNode<Type> *pmove=m_ptree.m_proot;
    while(pmove->m_pleft!=m_ptree.m_proot){
        pmove=pmove->m_pleft;
    }
    m_pcurrent=pmove;
    cout<<"root";
    while(pmove!=m_ptree.m_proot&&pmove){
        cout<<"--->"<<pmove->m_data;
        pmove=this->Next();
    }
    cout<<"--->end";
}

template<typename Type> void ThreadInorderIterator<Type>::InsertLeft(ThreadNode<Type> *left){
    left->m_pleft=m_pcurrent->m_pleft;
    left->m_nleftthread=m_pcurrent->m_nleftthread;
    left->m_pright=m_pcurrent;
    left->m_nrightthread=1;
    m_pcurrent->m_pleft=left;
    m_pcurrent->m_nleftthread=0;
    if(0==left->m_nleftthread){
        m_pcurrent=left->m_pleft;
        ThreadNode<Type> *temp=First();
        temp->m_pright=left;
    }
    m_pcurrent=left;
}

template<typename Type> void ThreadInorderIterator<Type>::InsertRight(ThreadNode<Type> *right){
    right->m_pright=m_pcurrent->m_pright;
    right->m_nrightthread=m_pcurrent->m_nrightthread;
    right->m_pleft=m_pcurrent;
    right->m_nleftthread=1;
    m_pcurrent->m_pright=right;
    m_pcurrent->m_nrightthread=0;
    if(0==right->m_nrightthread){
        m_pcurrent=right->m_pright;
        ThreadNode<Type> *temp=First();
        temp->m_pleft=right;
    }
    m_pcurrent=right;
}

template<typename Type> ThreadNode<Type>* ThreadInorderIterator<Type>::GetParent(
    ThreadNode<Type> *current){
        ThreadNode<Type> *pmove=current;
        while(0==pmove->m_nleftthread){
            pmove=pmove->m_pleft;
        }
        pmove=pmove->m_pleft;
        if(pmove==m_ptree.m_proot){
            if(pmove->m_pleft==current){
                return NULL;
            }
        }
        if(pmove->m_pright==current){
            return pmove;
        }
        pmove=pmove->m_pright;
        while(pmove->m_pleft!=current){
            pmove=pmove->m_pleft;
        }
        return pmove;
}

template<typename Type> void ThreadInorderIterator<Type>::Print(ThreadNode<Type> *start, int n){
    if(start->m_nleftthread&&start->m_nrightthread){
        for(int i=0;i<n;i++){
            cout<<"     ";
        }
        if(n>=0){
            cout<<start->m_data<<"--->"<<endl;
        }

        return;
    }
    if(start->m_nrightthread==0){
        Print(start->m_pright,n+1);
    }
    for(int i=0;i<n;i++){
        cout<<"     ";
    }
    if(n>=0){
        cout<<start->m_data<<"--->"<<endl;
    }
    if(start->m_nleftthread==0){
        Print(start->m_pleft,n+1);
    }
}

template<typename Type> void ThreadInorderIterator<Type>::Print(){
    Print(m_ptree.m_proot->m_pleft);
}







#endif

int Test_Tree();
#endif

