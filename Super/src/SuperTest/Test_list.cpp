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

	  //β������ insert from tail
	  for(int i=0;i<10;i++)
	  {
		  	  st_user d;
			  d.id=i;
			  sprintf(d.name,"%s %d","NNNNNN",i);  
			  listUser.push_back(d);
	  }

	  //ǰ����� insert from front
	  for(int i=0;i<10;i++)
	  {
		  st_user d;
		  d.id=i;
		  sprintf(d.name,"%s %d","NNNNNN",i);  
		  listUser.push_front(d);
	  }
	  



	  //������ӡ Traverse and print
	  printf("After Init:\n");  
	  for(std::list<st_user>::iterator it =listUser.begin(); it!=listUser.end(); ++it)  
	  {  
		  printf("%s \n", (it)->name);  
	  }  

	  //����ɾ�� fand and remove 
	  for(std::list<st_user>::iterator it = listUser.begin(); it != listUser.end();)  
	  {  
		  if((*it).id ==0)  
		  {  
		   	   it =listUser.erase(it);  // ��ȷ
			  // listUser.erase(iter);       //���� ������� error  program crashes 
		  }  
		  else  
		  {
			  ++it;    
			  //it++;
		  }
	  }  

	  //������ӡ Traverse and print
	  printf("After delete:\n");  
	  for(std::list<st_user>::iterator it =listUser.begin(); it!=listUser.end(); ++it)  
	  {  
		  printf("%s \n", (it)->name);  
	  }  


	  //�м����








	  //ɾ��ȫ��
	  //����1: method1
	  //listUser.clear();   //��ȷ OK
	  //����2: method2
	  for(it= listUser.begin(); it != listUser.end();it++)  
	  {  
		  //listUser.erase(it);   //Ҳ����ȷ�� ok?
	  }

	  //����д����ȷ The following was right
	  for(it= listUser.begin(); it != listUser.end();)  
	  {  
		  it=listUser.erase(it);   //
	  }

	  //����д������:��©ɾ The following was wrong  
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
