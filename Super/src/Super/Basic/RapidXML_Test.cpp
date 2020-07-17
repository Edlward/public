#include "RapidXML.hpp"
#include "RapidXML_utils.hpp"  //rapidxml::file
#include "RapidXML_print.hpp"  //rapidxml::print

#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{

    //读取xml
    rapidxml::file<> fdoc("test.xml");
    cout<<"************************powered by rapidxml**********************"<<endl;
    // cout<< fdoc.data()<< endl;
    // cout<<"length of xml:"<<fdoc.size()<<endl;


    rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<rapidxml::parse_no_data_nodes|rapidxml::parse_comment_nodes>(fdoc.data());    // 0 means default parse flags

    //DOM Tree的第一个子结点就是根结点
    rapidxml::xml_node<> *root = doc.first_node("note");

    rapidxml::xml_node<> *node_first = root->first_node();
    cout<<"first node of root:"<<endl<<*node_first<<endl;
    node_first->value("xchen");


    cout<<"******************************************************************"<<endl;
    cout<<doc<<endl;
    ofstream out("conver/after.xml");
    out << doc;
    out.close();
    return 0;
}

//版权声明：本文为CSDN博主「v_xchen_v」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/v_xchen_v/article/details/75634273