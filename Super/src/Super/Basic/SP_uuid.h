#ifndef SP_UUID_H
#define SP_UUID_H

#include "global.h"






#if 0
���ɷ���

	�Ѽ���һЩUUID�����ɷ�������������

	Shell
	Unix/Linux�����д���һ����Ϊuuidgen��С���ߣ����м�������һ��UUID����׼���
	��ȡ�ļ�/proc/sys/kernel/random/uuid����UUID�����磺

	cat /proc/sys/kernel/random/uuid
	libuuid
libuuid��һ����������UUID��C�⣬�����÷��ο�http://linux.die.net/man/3/libuuid��ʾ�����£�

#include <stdio.h>
#include <uuid/uuid.h>

int main(int argc, char **argv)
{
	uuid_t uuid;
	char str[36];

	uuid_generate(uuid);
	uuid_unparse(uuid, str);

	printf("%s\n", str);

	return 0;
}
��Linux�±���ʱ��Ҫ����uuid��

	gcc -o uuid uuid.c -luuid
	��Ubuntu�У���������������װlibuuid��

	sudo apt-get install uuid-dev
	boost uuid
	Boost����һ������ֲ�Ŀ�ԴC++�⣬���ṩ��UUID��ʵ�֡�

	����Ĵ����������һ��UUID

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

	boost::uuids::uuid a_uuid = boost::uuids::random_generator();
Qt QUuid
	Qt��һ����ƽ̨��C++��̿�ܣ�QUuid��ʵ����UUID�����ɡ��Ƚϡ�ת���ȹ��ܡ�

	����QUuid createUuid();����������һ���漴UUID��ʾ�����£�

#include <iostream>
#include <QUuid>
#include <QString>

	int main()
{
	QUuid uuid = QUuid::createUuid();
	std::cout << qPrintable(uuid.toString()) << std::endl;
	return 0;
}
CoCreateGuid
	Windows���ṩ�˺���CoCreateGuid��������GUID����Ҫʹ�õ�ͷ�ļ��ǡ�objbase.h������Ҫ���ӵĿ���ole32.lib������ԭ��Ϊ��

	HRESULT CoCreateGuid(GUID *pguid);
GUID��ԭ��Ϊ

	typedef struct _GUID
{
	DWORD Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
} GUID;
Java
	JDK 1.5����֧��UUID���÷����£�

	import java.util.UUID;
String uuid = UUID.randomUUID().toString();
#endif




#endif





