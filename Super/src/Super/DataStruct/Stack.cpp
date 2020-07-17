#include "Stack.h"


 
#ifdef UNIT_TEST

int Test_Stack()
{
#if 1
    SeqStack<int> seqStack(10);
    int init[10]={1,2,6,9,0,3,8,7,5,4};
    for(int i=0;i<10;i++){
        seqStack.Push(init[i]);
    }
    seqStack.Print();
    seqStack.Push(88);
    cout<<seqStack.Pop()<<endl;
    seqStack.Print();
    seqStack.MakeEmpty();
    seqStack.Print();
    seqStack.Pop();
    return 0;
#endif


#if 1
    LinkStack<int> linkstack;
    int initl[10]={1,3,5,7,4,2,8,0,6,9};
    for(int i=0;i<10;i++){
        linkstack.Push(initl[i]);
    }
    linkstack.Print();
    cout<<linkstack.Pop()<<endl;
    linkstack.Print();
    cout<<linkstack.GetTop()<<endl;
    linkstack.Print();
    cout<<linkstack.Pop()<<endl;
    linkstack.Print();
    linkstack.MakeEmpty();
    linkstack.Print();
    linkstack.Pop();
    return 0;
#endif




#if 1
    StackBydqueue<int> st ;
    st.push(1);
    st.pop();
    st.push(2);
    st.push(3);
    st.pop();
    cout << "1,st.top() ==  "  << st.top()  << endl ;
    st.push(4);
    cout << "2,st.top() ==  "  << st.top()  << endl ;
    cout << "3,st.size() ==  "  << st.size()  << endl ;
    return 0;
#endif

}

#endif

