#ifndef SP_UUID_H
#define SP_UUID_H

#include "global.h"






#if 0
生成方法

	搜集了一些UUID的生成方法，整理如下

	Shell
	Unix/Linux环境中大都有一个名为uuidgen的小工具，运行即可生成一个UUID到标准输出
	读取文件/proc/sys/kernel/random/uuid即得UUID，例如：

	cat /proc/sys/kernel/random/uuid
	libuuid
libuuid是一个用于生成UUID的C库，具体用法参考http://linux.die.net/man/3/libuuid，示例如下：

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
在Linux下编译时需要链接uuid库

	gcc -o uuid uuid.c -luuid
	在Ubuntu中，可以用下面的命令安装libuuid：

	sudo apt-get install uuid-dev
	boost uuid
	Boost库是一个可移植的开源C++库，它提供了UUID的实现。

	下面的代码可以生成一个UUID

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

	boost::uuids::uuid a_uuid = boost::uuids::random_generator();
Qt QUuid
	Qt是一个跨平台的C++编程框架，QUuid类实现了UUID的生成、比较、转换等功能。

	函数QUuid createUuid();可用于生成一个随即UUID。示例如下：

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
	Windows下提供了函数CoCreateGuid用于生成GUID。需要使用的头文件是”objbase.h”，需要链接的库是ole32.lib，函数原型为：

	HRESULT CoCreateGuid(GUID *pguid);
GUID的原型为

	typedef struct _GUID
{
	DWORD Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
} GUID;
Java
	JDK 1.5以上支持UUID，用法如下：

	import java.util.UUID;
String uuid = UUID.randomUUID().toString();
#endif




#endif





