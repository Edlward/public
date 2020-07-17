#include <ostream>
#include <istream>
#include <string.h>
#include "SString.h"

using std::ostream;
using std::istream;


NAME_SPACE_MAIN_BEGIN


String::String():
    _cstr(NULL),
    _used(new size_t(1)),
    _length(0),
    _capacity(0)
{
}

String::String(const String &s):
    _cstr(s._cstr),
    _used(s._used),
    _length(s._length),
    _capacity(s._capacity)
{
    ++*_used;
}

String::String(const char *cstr)
{
    if(cstr == NULL)
        return;
    size_t len = strlen(cstr);
    _initString(cstr, len);
}

String::String(const char *cstr, size_t len)
{
    if(cstr == NULL)
        return;
    size_t str_len = strlen(cstr);
    if(len <= str_len)
    {
        _initString(cstr, len);
    }
}

String::~String()
{
    _decUsed();
}

String& String::operator=(const String &s)
{
    ++*(s._used);
    _decUsed();
    _cstr = s._cstr;
    _length = s._length;
    _capacity = s._capacity;
    return *this;
}

String& String::operator=(const char *cstr)
{
    if(cstr != NULL)
    {
        _decUsed();
        size_t len = strlen(cstr);
        _initString(cstr, len);
    }
    return *this;
}

String& String::operator+=(const String &s)
{
    _addAssignOpt(s._cstr, s._length);
    return *this;
}

String& String::operator+=(const char *cstr)
{
    if(cstr != NULL)
        _addAssignOpt(cstr, strlen(cstr));
    return *this;
}

void String::clear()
{
    _decUsed();
    _cstr = NULL;
    _used = new size_t(1);
    _length = 0;
    _capacity = 0;
}

void String::_initString(const char *cstr, size_t len)
{
    if(cstr == NULL)
        return;

    _cstr = new char[len + 1];
    memcpy(_cstr, cstr, len);
    _cstr[len] = 0;
    _used = new size_t(1);
    _length = len;
    _capacity = len;
}

void String::_decUsed()
{
    --*_used;
    if(*_used == 0)
    {
        if(_cstr != NULL)
        {
            delete[] _cstr;
            _cstr = NULL;
            _length = 0;
            _capacity = 0;
        }
        delete _used;
        _used = NULL;
    }
}

char* String::_renewAndCat(const char *cstr, size_t len)
{
    size_t new_len = len + _length;
    size_t capacity = new_len;
    capacity += (capacity >> 1);

    char *ptr = new char[capacity+1];
    if(_cstr != NULL)
        memcpy(ptr, _cstr, _length);
    ptr[_length] = 0;

    _length = new_len;
    _capacity = capacity;
    strncat(ptr, cstr, len);
    return ptr;
}



void String::_addString(const char *cstr, size_t len)
{
    //本函数，只有在引用计数为1时，才可用
    if(*_used != 1)
        return;

    if(len + _length > _capacity)
    {
        char *ptr = _renewAndCat(cstr, len);
        delete[] _cstr;
        _cstr = ptr;
    }
    else
    {
        strncat(_cstr, cstr, len);
        _length += len;
    }
}

void String::_addAssignOpt(const char *cstr, size_t len)
{
    if(*_used == 1)
        _addString(cstr, len);
    else
    {
        _decUsed();
        _cstr = _renewAndCat(cstr, len);
        _used = new size_t(1);
    }
}

String operator +(const String &lhs, const String &rhs)
{
    String stemp(lhs);
    stemp += rhs;
    return stemp;
}

ostream& operator << (ostream &os, const String &s)
{
	os<<s.cstr();
	return os;
}

istream& operator >> (istream &in, String &s)
{
	const int BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE];
	char *end = buffer + BUFFER_SIZE -1;
	s.clear();

	do
	{
        //用于判断是否读完输入内容，因为如果还未读取的输入字符数大于buffer
        //的容量，则buffer的最后一个字符会被get函数置为'\0'
        *end = '#';
		in.get(buffer, BUFFER_SIZE);
        s += buffer;
	}while(*end == '\0');
	in.get();

	return in;
}











#if 0


C++ string类的隐式共享写时拷贝的实现及设计要点
	标签： C++string隐式共享写时复制代码设计实现
	2014-04-04 00:08 5356人阅读 评论(6) 收藏 举报
	分类： C/C++（35）  
	版权声明：本文为博主原创文章，未经博主允许不得转载。
	字符串一种在程序中经常要使用到的数据结构，然而在C中却没有字符串这种类型。在C++中，为了方便字符串的使用，在STL中提供了一个string类。该类维护一个char指针，并封装和提供各种的字符串操作。

	一、为什么要实现隐式公享写时拷贝
	试想一下，如果我们要自己实现一个string类，最简单的方式是什么？就是让每一个string类的实例维护一个在内存中独立的字符数组，每个string对象各不相干。这样一个对象的任何变化都不会影响到其他的对象。这样做的好处就是处理简单，不易出错，但是这样做的缺点却是内存的使用量、程序的效率也低。
	例如，对于如下的例子：
	[cpp] view plain copy print?
	int swap(string &x, string &y)  
{  
	string tmp(x);  
	x = y;  
	y = tmp;  
}  
在上面的代码中，我们需要做的事情非常简单，只是想交换一下两个string对象的值而已。然而，如果我们采用上面所说的实现方式，一共在内存上进行了三次的用new来创建一个新的数组并复制数据，同时还会调用两次的delete[]。我们花了这么大的力气才完成了这样一个简单的动作。而隐式共享写时复制的内存管理策略却可以解决这样的问题。虽然C++11用&&运算符解决了上述问题，但是在程序中使用大量的string的副本，而不改变其值的情况还是不少的。例如string数组，删除一个元素后的移动操作。

	二、隐式共享写时复制的实现思想
	什么是隐式共享写时拷贝呢？就是当用一个string对象初始化另一个string对象或把一个string对象赋值给另一个string对象时，它们内部维护的指针其实指向了内存中的同一个字符数组，这就是隐式共享。

	使用这种方法，上述的代码就不需要调用new来创建数组也不需要复制，也不会调用delete[]（如果不是很明白也不要紧，看完实现代码和后自然就明白了）。然后两个指针指向同一个对象很容易引发错误，当其中一个对象执行析构函数释放掉其内部指针指向的内存时，另一个对象却对此完全不知情，可能会引用一个不存在的内存，从而让程序崩溃。所以为了方便资源的管理，我们引用智能指针的思想，为每个内存中的字符数组（用new创建，存在于堆中）添加一个引用计数used，表示有多少个对象引用这个块内存（即字符数组）。当一个对象析构时，它会把引用计数used减1，当used为0时，表示没有对象引用这块内存，从而把这块内存释放掉。当然由于used也要在对象中共享，所以它也是一个堆中的数据，每个对象有一个指向它的指针。

	而当一个string对象需要改变它的值时，例如
	[cpp] view plain copy print?
	string s1("abc");  
string s2(s1);  
string s3("edf");  
s2 += s3;  
此时，s1和s2指向了堆内存中的同一个字符数组，而当s2的值要改变时，因为如果直接在其指向的内存中修改，则会影响到对象s1，所以为了让s2的操作不影响到s1，s2会在重新new出一块内存，然后先把之前所引用的字符数组的数据复制到新的字符数组中，然后再把s3中的字符数据复制到新的字符数组中。这就是写时拷贝。注意，同时还要把之前指向的内存的引用计数减1（因为它指向了新的堆中的字符数组），并在堆中重新new一个块内存，用于保存新的引用计数，同时把新的字符数组的引用计数置为1。因为此时只有一个对象（就是改变值的对象）在使用这个内存。

	三、代码实现及设计要点详解
	说了这么多，还是来看看代码的实现吧，为了与标准C++的string类区别开来，这样采用第一个字母大写来表示自定义的字符串类String。

	源代码可以点击下面的连接下载：
http://download.csdn.net/detail/ljianhui/7143351

其头文件_stringv2.h如下：
	[cpp] view plain copy print?
#ifndef _STRINGV2_H_INCLUDED  
#define _STRINGV2_H_INCLUDED  
	/*** 
	String类的部分实现，采用的内存管理策略是：隐式共享，写时复制 
	实现方法：与智能指针的实现类似 
	***/  
class String  
{  
public:  
	String();  
	String(const String& s);  
	String(const char *pc, size_t len);  
	String(const char *pc);  
	~String();  
	String& operator=(const String &s);  
	String& operator=(const char *s);  
	String& operator+=(const String &rhs);  
	String& operator+=(const char *rhs);  
	void clear();  
	size_t getLength()const {return _length;}  
	const char* cstr()const {return _cstr;}  
private://function  
	void _initString(const char *cstr, size_t len);  
	void _decUsed();  
	char* _renewAndCat(const char *cstr, size_t len);  
	void _addString(const char *cstr, size_t len);  
	void _addAssignOpt(const char *cstr, size_t len);  
private://data  
	char *_cstr;  
	size_t *_used;  
	size_t _length;  
	size_t _capacity;  
};  
String operator+(const String &lhs, const String &rhs);  
std::ostream& operator <<(std::ostream &os, const String &s);  
std::istream& operator >>(std::istream &in, String &s);  
#endif // _STRINGV2_H_INCLUDED  
从上面的String的数据成员，我们可以看到String在其内部维护一个指向堆内存的字符数组的char指针_cstr和一个指向堆内存中字符数组的引用计数的size_t指针_used。本类并没有实现String的所有操作，只是实现了大部分的初始化和String跟写操作有关的函数。

	注意：为了说明的方便，我会使用s._cstr等方式来指明一个成员变量所属的对象，或使用*s._cstr等方式来引用一个对象的指针成员所指的内存。但这并不是说在类的外面访问成员变量，只是为了说明的方便和清晰而已。为了方便代码的阅读，类的成员变量或私有函数都以下划线“_”开头。

	下面就来一个函数一个函数地解释其实现方式。
	1）默认构造函数
	[cpp] view plain copy print?
	String::String():  
	_cstr(NULL),  
	_used(new size_t(1)),  
	_length(0),  
	_capacity(0)  
{  
}  
这里需要注意的地方就是，在默认初始化中，我们并不使用new来申请内存，而是直接把_cstr置为NULL，这样做是因为我们不知道程序接下来会做什么动作，贸然为其分配内存是不合理的。例如，对于如下操作，则无需分配内存，
	[cpp] view plain copy print?
	String s1("abc");  
String s2;  
s2 = s1;  
根据隐式共享的原则，只需要把s2._cstr的值赋为s1._cstr即可。而为什么没有为对象分配内存，而*_used的值却为1呢？这里只要是为了操作的统一，考虑上面的语句s2 = s1，其产生的操作应该是把s2._used所指向的内存数据（引用计数）的值减1，因为s2._cstr不再指向原先的字符数据。s1._used所指向的内存数据的值加1。若*s2._userd的值为0，就释放s2._userd和s2._cstr所指向的内存。而如果在这里，*s2._userd的初始值为0，0减1就会变成-1，而_userd是一个无符号整数的指针，它的值就会变成2^32-1，从而让程序运行的结果不符合我们的预想。而*s2._userd的初始值为1则可完美地避免这个问题。

	2）复制构造函数
	[cpp] view plain copy print?
	String::String(const String &s):  
	_cstr(s._cstr),  
	_used(s._used),  
	_length(s._length),  
	_capacity(s._capacity)  
{  
	++*_used;  
}  
本函数非常易懂，就是把s的成员的值全部复制给*this即可，但是由于多了*this这个对象引用s的字符数组，所以应该把该字符数组的引用计数加1。注意，此时this->_used和s._used指向了同一个对象。

	3）带C字串参数的构造函数
	[cpp] view plain copy print?
	String::String(const char *cstr, size_t len)  
{  
	if(cstr == NULL)  
		return;  
	size_t str_len = strlen(cstr);  
	if(len <= str_len)  
	{  
		_initString(cstr, len);  
	}  
}  
void String::_initString(const char *cstr, size_t len)  
{  
	if(cstr == NULL)  
		return;  
	_cstr = new char[len + 1];  
	memcpy(_cstr, cstr, len);  
	_cstr[len] = 0;  
	_used = new size_t(1);  
	_length = len;  
	_capacity = len;  
}  
该函数非常简单，由于是构造函数，而且使用的参数是C风格的字符串，所以默认为其字符串一定不是某个对象所引用的字符数组，所以直接为其分配内存，并复制字符。非常明显，因为是该对象第一个创建该字符数组的，所以其引用为1.

	4）带C风格字符串的构造函数
	[cpp] view plain copy print?
	String::String(const char *cstr)  
{  
	if(cstr == NULL)  
		return;  
	size_t len = strlen(cstr);  
	_initString(cstr, len);  
}  
其实现与上原理相同，只是参数不同，不再详述。

	5）析构函数
	[cpp] view plain copy print?
	String::~String()  
{  
	_decUsed();  
}  
void String::_decUsed()  
{  
	--*_used;  
	if(*_used == 0)  
	{  
		if(_cstr != NULL)  
		{  
			delete[] _cstr;  
			_cstr = NULL;  
			_length = 0;  
			_capacity = 0;  
		}  
		delete _used;  
		_used = NULL;  
	}  
}  
_decUsed()函数可以说是该类内存释放的管理函数，可以看到，每当一个对象被析构时，其指向的堆中的字符数组的引用计数就会减1，当引用计数为0时，就释放字符数组和引用计数。

	6）赋值操作函数
	[cpp] view plain copy print?
	String& String::operator=(const String &s)  
{  
	++*(s._used);  
	_decUsed();  
	_cstr = s._cstr;  
	_length = s._length;  
	_capacity = s._capacity;  
	return *this;  
}  
该赋值操作函数的参数一个本类的对象，该类赋值操作函数第一个要避免的就是自身赋值的问题，在有指针存在的类中是特别要重视这个问题，而在这个String类也不可例外。为什么这样说呢？因为我们调用赋值操作函数时，必须要减少左值的引用计数，增加右值的引用计数，这个在第1）点已经说过了，而如果是自身赋值的话，在减少其引用计数时，其引用计数可能为0，从而导致字符数组的释放，从而让_cstr指针悬空（delete[]掉了，却在赋值的过程中，重新赋为delete[]前的值，即_cstr的值没有在赋值过程中改变）。

	一般的程序的做法是判断参数的地址与this是否相等来避免自身赋值，而这里却可以采用巧妙的策略来避免这个问题，可以看到上面的代码并没有if判断语句。我们首先对*s._used加1，这样*s._used至少为2，然后再对*(this->_used)减1，这样即使s与*this是同一个对象，也可以保证*(this->_used)的值至少为1，不会变为0，从而让字符数组不会被释放。因为复制是使用隐式共享的，所以直接复制指针，使指针_cstr其指向与s同一个存在中的字符数组并复制其他的数据成员即可。

	同时，我们还要记得返回当前对象的引用。

	7）重载的赋值操作函数
	[cpp] view plain copy print?
	String& String::operator=(const char *cstr)  
{  
	if(cstr != NULL)  
	{  
		_decUsed();  
		size_t len = strlen(cstr);  
		_initString(cstr, len);  
	}  
	return *this;  
}  
该赋值操作函数的参数一个C风格的字符串，因而不会发生自身赋值的问题。与String(const char *cstr)函数相似，唯一不同的是使用赋值操作函数时，对象已经存在，所以要调用_decUsed来减少该对象的_cstr原先指向的字符数组的引用计数。然后生成根据cstr创建一个全新的字符数组。并返回当前对象的引用。

	8）重载+=操作符，实现字符串连接
	[cpp] view plain copy print?
	String& String::operator+=(const String &s)  
{  
	_addAssignOpt(s._cstr, s._length);  
	return *this;  
}  
String& String::operator+=(const char *cstr)  
{  
	if(cstr != NULL)  
		_addAssignOpt(cstr, strlen(cstr));  
	return *this;  
}  

void String::_addAssignOpt(const char *cstr, size_t len)  
{  
	if(*_used == 1)  
		_addString(cstr, len);  
	else  
	{  
		_decUsed();  
		_cstr = _renewAndCat(cstr, len);  
		_used = new size_t(1);  
	}  
}  
void String::_addString(const char *cstr, size_t len)  
{  
	//本函数，只有在引用计数为1时，才可用  
	if(*_used != 1)  
		return;  
	if(len + _length > _capacity)  
	{  
		char *ptr = _renewAndCat(cstr, len);  
		delete[] _cstr;  
		_cstr = ptr;  
	}  
	else  
	{  
		strncat(_cstr, cstr, len);  
		_length += len;  
	}  
}  
char* String::_renewAndCat(const char *cstr, size_t len)  
{  
	size_t new_len = len + _length;  
	size_t capacity = new_len;  
	capacity += (capacity >> 1);  
	char *ptr = new char[capacity+1];  
	if(_cstr != NULL)  
		memcpy(ptr, _cstr, _length);  
	ptr[_length] = 0;  
	_length = new_len;  
	_capacity = capacity;  
	strncat(ptr, cstr, len);  
	return ptr;  
}  
+=是一个复杂的操作，也是我实现时琢磨得最久的操作。因为它是写的操作，根据写时拷贝的原则，它需要减少其原先字符数组的引用计数，同时创建一个新的字符数组来储存增加长度后的字符串。并且该对象所引用的字符数组可能只有该对象自己在引用，也就是说其引用计数为1，此时减少其引用计数还可能导致原先字符数组的释放，从而丢失数据，并在使用指向原先字符数组的指针进行数据复制时发生错误。所以引用计数是否为1应该采用不同的策略。

	而当引用计数为1时，我们可以认为该对象独立享有该字符数组，可以对其进行任何操作而不影响其他对象，这时，我们可以把字符串直接追加到已经的字符数组的后面，而这样做可能因为字符数组的容量不够而不能进行，这时为字符数组的重新分配合适的空间。

	当引用计数不为1时，我们首先调用_decUsed（）来减少原字符数组的引用计数，然后调用_renewAndCat来连接并产生新的字符数组，然后重置_cstr的指向，并new一个新的引用计数，初始值置为1.

	上面的函数中，_renewAndCat的功能就是分配新的字符数组，同时把原先的字符数据复制到新的字符数组中，再在新的字符数组中追加字符串，返回新的字符数组的首地址。_addString是只有当引用计数为1时才能调用的函数，其字符数组足以容纳连接后的字符串，则直接连接，若不能，则调用_renewAndCat重新分配合适的字符数组，并进行复制，最后，把旧的字符数组delete[]掉，再把_cstr赋值为_renewAndCat创建的新字符数组的首地址。

	注：本人认为，如果一个字符串对象做了一次+=运算，那么它很可能会很快做第二次，所以在分配内存时，我采用了预分配的策略，每次分配都分配连接完成后的字符串的长度的1.5倍。这样当下一次执行+=时，字符数组就可能有足够多的容量来保存连接后的字符串，而不用重新分配和复制。而且我们注意到，当调用+=一次之后，*_used肯定为1，即下次运行+=时，是极有可能直接加到字符数组的后面的。

	为了提高程序的运行效率，在进行1.5倍的预分配时，没有使用浮点数乘法，更没有使用除法，而是采用了移位运算，如下：
	int capacity = len;
capacity += (capacity >> 1)
	其对应的数学表达式为：“x = capacity + capacity/2；capacity = x”。因为右移运算相当于除以2，这样就实现了乘以1.5的运算操作。

	9）清空字符串
	[cpp] view plain copy print?
	void String::clear()  
{  
	_decUsed();  
	_cstr = NULL;  
	_used = new size_t(1);  
	_length = 0;  
	_capacity = 0;  
}  
该函数用于清除字符串对象引用的字符数组的数据，所以我们只需要调用_decUsed函数，减少对象所引用的字符数组的引用计数，并把其他成员变量设置为默认的值即可。即与默认构造函数所设置的值一致。


	注：以下函数不是String的成员函数
	10）重载+操作符
	[cpp] view plain copy print?
	String operator +(const String &lhs, const String &rhs)  
{  
	String stemp(lhs);  
	stemp += rhs;  
	return stemp;  
}  
该函数的实现可以借助上面实现的+=操作符，先用第一个对象rhs复制构造一个临时对象stemp，然后通过把第二个参数追加到临时对象stemp上，返回stemp即可简单轻松地实现+操作符的重载。

	11）重载输出操作符
	[cpp] view plain copy print?
	ostream& operator << (ostream &os, const String &s)  
{  
	os<<s.cstr();  
	return os;  
}  
12）重载输入操作符
	[cpp] view plain copy print?
	istream& operator >> (istream &in, String &s)  
{  
	const int BUFFER_SIZE = 256;  
	char buffer[BUFFER_SIZE];  
	char *end = buffer + BUFFER_SIZE -1;  
	s.clear();  
	do  
	{  
		//用于判断是否读完输入内容，因为如果还未读取的输入字符数大于buffer  
		//的容量，则buffer的最后一个字符会被get函数置为'\0'  
		*end = '#';  
		in.get(buffer, BUFFER_SIZE);  
		s += buffer;  
	}while(*end == '\0');  
	in.get();  
	return in;  
}  
实现输入操作符的重载的一个困难之处就是我们不知道用户要输入的字符串的长度，也就不知道应该分配一个多大的缓冲区来接收输入的字符。所以在这里，设置一个一定大小的缓冲，采用循环读取，连续添加到字符串对象中的方法来实现。那么如何知道该循环读取输入多少次呢？也就是说，怎么知道已经把所有的输入字符读取完毕呢？在这里，我使用了一个标准输入流istream的get成员函数，该成员函数从输入流中读取指定个数的字符或遇到输入流结束而返回，注意最后它会自动加入一个空字符‘\0’作为结束。这个空字符也作为读入的字符数量的计数。例如，如果有一个大小为6的char型数组作为buffer，从标准输入流中读入6个字符，实际上只会从标准输入中读入最多5个字符（因为可能遇到流结束），并把空字符‘\0’加入到buffer的末尾。

	所以我们可以把buffer的最后一个字节，设置成我们自己特定的一个字符（只是是非‘\0’即可），如这里的'#'，然后读入buffer大小的字符数。若还没有读取完毕，我们设置的这个特殊的字符会被空字符'\0'覆盖，我们从而知道，还没读取完标准输入的数据。若我们设置的特殊字符没有被覆盖，就说明，读到的数据不足以填满buffer，也就是说，我们已经没有数据可读了，从而可以判断已经读取完所有输入的字符。

	注：输入也是一个写的操作，并且会把对象之前的内容覆盖掉，所以在输入到对象之前，要先调用clear成员函数，把对象清空。

	四、测试代码
	[cpp] view plain copy print?
#include <iostream>  
#include "_stringv2.h"  
	using std::cin;  
using std::cout;  
using std::cin;  
using std::endl;  
int main()  
{  
	String s1;  
	s1 = "abc";  
	{  
		String s2(s1);  
		s2 += s1;  
		cout << s2 << endl;  
	}  
	String s3(s1);  
	cin >> s3;  
	cout << s3 << endl;  
	cout << s1 << endl;  
	String s4 = s1 + s3;  
	cout << s4 << endl;  
}  
运行结果如下：


	五、代码分析
	首先定义一个String的对象s1，s1调用默认构造函数，生成一个默认的对象，然后调用赋值操作函数，为s1分配堆内存字符数组。对象s2是以对象s1的样本复制构造出来的对象，其作用域只在花括号内。我们可以看到s2的改变并没有影响到s1。其他的调用也一样，从而可以看到是实现了隐式共享，写时拷贝。从运行的结果可以看出，一切的运行都是没有问题的，与标准库的string的输出一致。


#endif







#include <iostream>
using std::cin;
using std::cout;
using std::cin;
using std::endl;


#ifdef UNIT_TEST

int Test_String()
{
	String s1;
	s1 = "abc";
	{
		String s2(s1);
		s2 += s1;
		cout << s2 << endl;
	}
	String s3(s1);
	cin >> s3;
	cout << s3 << endl;
	cout << s1 << endl;
	String s4 = s1 + s3;
	cout << s4 << endl;
	return 0;
}
#endif


NAME_SPACE_MAIN_END