#include "NetOperation.h"

#if 0   //未完待续
//IP地址字符串与数组转换

//这个IPV6的缺点就是强制需要7个冒号，不支持中间0省略的情况.
//IPV4字符串转整型，因为整型占4个字节，可用一个无符号整型进行存储。

typedef unsigned char U8;
typedef unsigned int U32;
typedef U8 IPv6Addr[16];
typedef U32 IPv4Addr;

IPv4Addr StrToIPAddr(const std::string &ipStr)
{
    IPv4Addr ip;
    U8 *addr = reinterpret_cast<U8 *>(&ip);
    if(sscanf(ipStr.c_str(),"%hhu.%hhu.%hhu.%hhu",addr,addr+1,addr+2,addr+3) == 4)
    {
        return ip;      
    }else{
        return 0u;
    }
}

//整型转IPV4字符串
std::string IPAddrToStr(const IPv4Addr &ip)
{
    char ipStr[16] = "";
    const U8 * addr = reinterpret_cast<const U8 *>(&ip);
    int n = sprintf(ipStr,"%u.%u.%u.%u",addr[0],addr[1],addr[2],addr[3]);
    printf("sprintf n=%d\n",n);
    return ipStr;
}

//IPV6字符串转无符号数组
void StrToIPAddr(const std::string &ipStr,IPv6Addr &ipAddress)
{
    if (ipStr.empty())
        return;
    struct in6_addr ipAddr;
    if( inet_pton(AF_INET6,ipStr.c_str(),ipAddr.s6_addr) == 1)
    {
        memcpy(ipAddress,ipAddr.s6_addr,16);
    }else{
        memset(ipAddress,0,16);
    }   
}

//反之无符号数组转IPV6字符串
void IPAddrToStr(const IPv6Addr &ip,std::string &ipStr)
{
    char ipc[INET6_ADDRSTRLEN] = "";
    struct in6_addr ipAddr;
    memcpy(ipAddr.s6_addr,ip,16);

    if ( NULL !=inet_ntop(AF_INET6,(void*)&ipAddr,ipc,INET6_ADDRSTRLEN) )
    {
        ipStr.assign(ipc);
    }
}
#endif




#ifdef _WIN32
#include <Winsock2.h>
#include <QNetworkInterface>
#include "iphlpapi.h"
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
int getNetworkInfo(std::vector<AdapterInformation> &adapterVec)
{
	QList<QNetworkInterface> ifaceList = QNetworkInterface::allInterfaces();
	for (int i = 0; i < ifaceList.count(); i++)
	{
		QNetworkInterface var = ifaceList.at(i);
		//寻找启用的网卡
		if(var.isValid()
			&& !var.flags().testFlag(QNetworkInterface::IsLoopBack)
			&& var.flags().testFlag(QNetworkInterface::IsRunning))
		{
			AdapterInformation adapterIf;
			qDebug() <<"AdapterName:"<< var.humanReadableName() << "Index: " << var.index();
			adapterIf.index = var.index();
			adapterIf.name = var.humanReadableName().toStdString();
			//获取子网掩码
			QList<QNetworkAddressEntry> entryList = var.addressEntries();//每个网卡可能有多个ip
			foreach(QNetworkAddressEntry entry,entryList)
            {
				if(entry.ip().toIPv4Address())
                {
					//qDebug() <<"IpAddr:"<< entry.ip().toString().toStdString();
					//qDebug() <<"Netmask:"<< entry.netmask().toString().toStdString();
					adapterIf.ip = entry.ip().toString().toStdString();
					adapterIf.netmask = entry.netmask().toString().toStdString();
					break;
				}
			}
			adapterVec.push_back(adapterIf);
		}
	}
	/* Some general variables */
	ULONG ulOutBufLen;
	DWORD dwRetVal;
	unsigned int i;

	/* variables used for GetAdapterInfo */
	IP_ADAPTER_INFO *pAdapterInfo;
	IP_ADAPTER_INFO *pAdapter;

	printf("------------------------\n");
	printf("This is GetAdaptersInfo\n");
	printf("------------------------\n");

	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdapterInfo\n");
		return 1;
	}
	ulOutBufLen = sizeof (IP_ADAPTER_INFO);

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdapterInfo\n");
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR) {
		printf("GetAdaptersInfo failed with error %d\n", dwRetVal);
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return 1;
	}

	pAdapter = pAdapterInfo;
	while (pAdapter) {
		printf("\tAdapter ID: \t%d\n", pAdapter->Index);
		int adaptIndex = pAdapter->Index;
		for(int i=0;i<adapterVec.size();++i){
			if(adapterVec[i].index == adaptIndex){
				adapterVec[i].gateway = pAdapter->GatewayList.IpAddress.String;
				break;
			}
		}
		printf("\tIP Address: \t%s\n",
			pAdapter->IpAddressList.IpAddress.String);
		printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

		printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
		printf("\t***\n");

		pAdapter = pAdapter->Next;
	}
	/* Free allocated memory no longer needed */
	if (pAdapterInfo) {
		FREE(pAdapterInfo);
		pAdapterInfo = NULL;
	}
	populateDNS(adapterVec);
	for(int i=0; i<adapterVec.size();++i)
    {
		//qDebug() << "AdapterInformation: index="<<adapterVec[i].index << "name=" << adapterVec[i].name << \
			" ip=" << adapterVec[i].ip << " netmask=" << adapterVec[i].netmask << " gateway=" << \
			adapterVec[i].gateway << " dns=" << adapterVec[i].dns;

        printf("AdapterInformation: index:\n",adapterVec[i].index);
        printf("name:%s\n",adapterVec[i].name.c_str());
        printf("ip:%s\n",adapterVec[i].ip.c_str());
        printf("netmask:%s\n",adapterVec[i].netmask.c_str());
        printf("gateway:%s\n",adapterVec[i].gateway.c_str());
        printf("dns:%s\n",adapterVec[i].dns.c_str());
	}

    return 0;
}

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
void populateDNS(std::vector<AdapterInformation> &adapterVec)
{
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_INET;//ipv4

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;
	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf
				("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal =
			GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		} else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {
			int curAdaptIndex = pCurrAddresses->IfIndex;
			printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
			pDnServer = pCurrAddresses->FirstDnsServerAddress;
			if (pDnServer) {
				for (i = 0; pDnServer != NULL; i++){
					printf("\tDNS Server IP:%s\n", inet_ntoa(((sockaddr_in *)(pDnServer->Address.lpSockaddr))->sin_addr));
					int index =0;
					for(;index<adapterVec.size();++index){
						if(adapterVec[index].index == curAdaptIndex){
							adapterVec[index].dns = inet_ntoa(((sockaddr_in *)(pDnServer->Address.lpSockaddr))->sin_addr);
							break;
						}
					}
					if(index<adapterVec.size())
						break;
					pDnServer = pDnServer->Next;
				}
				//printf("\tNumber of DNS Server Addresses: %d\n", i);
			} else{
				printf("\tNo DNS Server Addresses\n");
			}
			printf("*******\n");

			pCurrAddresses = pCurrAddresses->Next;
		} 
	} else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   
				// Default language
				(LPTSTR) & lpMsgBuf, 0, NULL)) {
					printf("\tError: %s", lpMsgBuf);
					LocalFree(lpMsgBuf);
					if (pAddresses)
						FREE(pAddresses);
					exit(1);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}
}

#include <QProcess>
bool changeIP(std::string name, std::string ip, std::string netmask, std::string gateway, std::string dns)
{
	printf("begin changeIP\n");
	QString qname = QString::fromStdString(name);
	QString qip = QString::fromStdString(ip);
	QString qnetmask = QString::fromStdString(netmask);
	QString qgateway = QString::fromStdString(gateway);
	QString qdns = QString::fromStdString(dns);
	QProcess p1;
	//QProcess p2(this);
	QString chgip = "netsh interface ipv4 set address name=\"" + qname + "\" source=static address="+ qip + " mask=" + qnetmask +" gateway=" + qgateway;
	qDebug() << "ipInfo: " << chgip;
	QString chgdns = "netsh interface ipv4 set dnsservers name=\"" + qname + "\" source=static address=" + qdns;
	p1.start(chgip);
	p1.waitForFinished(3000);
	QString strResult1 = QString::fromLocal8Bit(p1.readAllStandardOutput());
	qDebug() << "strResult1: "<<strResult1;
	p1.start(chgdns);
	p1.waitForFinished(3000);
	strResult1 = QString::fromLocal8Bit(p1.readAllStandardOutput());
	qDebug() << "strResult1: "<<strResult1;
	printf("end changeIP\n");
	return true;
}

#endif
