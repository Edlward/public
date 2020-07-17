#include "Test_list.h"

#include <list>
#include <iostream>

struct st_user  
{  
	int id;  
	char name[255];  
};  


int Test_list()
{
      std::list<st_user> listUser;
	  std::list<st_user>::iterator it;

	  //尾部插入 insert from tail
	  for(int i=0;i<10;i++)
	  {
		  	  st_user d;
			  d.id=i;
			  sprintf(d.name,"%s %d","NNNNNN",i);  
			  listUser.push_back(d);
	  }

	  //前面插入 insert from front
	  for(int i=0;i<10;i++)
	  {
		  st_user d;
		  d.id=i;
		  sprintf(d.name,"%s %d","NNNNNN",i);  
		  listUser.push_front(d);
	  }
	  



	  //遍历打印 Traverse and print
	  printf("After Init:\n");  
	  for(std::list<st_user>::iterator it =listUser.begin(); it!=listUser.end(); ++it)  
	  {  
		  printf("%s \n", (it)->name);  
	  }  

	  //查找删除 fand and remove 
	  for(std::list<st_user>::iterator it = listUser.begin(); it != listUser.end();)  
	  {  
		  if((*it).id ==0)  
		  {  
		   	   it =listUser.erase(it);  // 正确
			  // listUser.erase(iter);       //错误 程序崩溃 error  program crashes 
		  }  
		  else  
		  {
			  ++it;    
			  //it++;
		  }
	  }  

	  //遍历打印 Traverse and print
	  printf("After delete:\n");  
	  for(std::list<st_user>::iterator it =listUser.begin(); it!=listUser.end(); ++it)  
	  {  
		  printf("%s \n", (it)->name);  
	  }  


	  //中间插入








	  //删除全部
	  //方法1: method1
	  //listUser.clear();   //正确 OK
	  //方法2: method2
	  for(it= listUser.begin(); it != listUser.end();it++)  
	  {  
		  //listUser.erase(it);   //也是正确？ ok?
	  }

	  //以下写法正确 The following was right
	  for(it= listUser.begin(); it != listUser.end();)  
	  {  
		  it=listUser.erase(it);   //
	  }

	  //以下写法错误:会漏删 The following was wrong  
	  for(it= listUser.begin(); it != listUser.end();it++)  
	  {  
	       it=listUser.erase(it);   //
	  }

	  printf("After remove all:\n");  
	  for(it =listUser.begin(); it!=listUser.end(); ++it)  
	  {  
		  printf("%s \n", (it)->name);  
	  }  


	  return 0;
}
