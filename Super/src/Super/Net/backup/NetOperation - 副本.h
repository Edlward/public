#ifndef NetOperation_H
#define NetOperation_H
//修改IP地址等基础操作--待完善，去掉qt依赖

#include <vector>
#include <string>
//#include <QString>
//struct AdapterInformation {
//	int index;
//	QString name;
//	QString ip;
//	QString netmask;
//	QString gateway;
//	QString dns;
//};

struct AdapterInformation 
{
    int index;
    std::string name;
    std::string ip;
    std::string netmask;
    std::string gateway;
    std::string dns;
};
int getNetworkInfo(std::vector<AdapterInformation> &);
void populateDNS(std::vector<AdapterInformation> &);
bool changeIP(std::string name, std::string ip, std::string netmask, std::string gateway, std::string dns);


#endif


