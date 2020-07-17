/******************************************************************************
*File	: Console.h
*Author : Lan Hu
*Email	: Hi_Blue@126.com
*Notes	: Some method about Win32 Console Control. 
*Licence: 
*Created: 2008-12-17
*******************************************************************************/

#ifndef _MYLIB_CONSOLE_H_INCLUDE_20081127_
#define _MYLIB_CONSOLE_H_INCLUDE_20081127_

#if 0
#include <stdlib.h>
#include <windows.h>
#include "public.h"


class Console
{
public:

	/* 颜色定义 */
	enum E_COLOR
	{
		BLACK		= 0,
		BLUE		= 1,
		DARK_GREEN	= 2,
		LIGHT_BLUE	= 3,
		RED			= 4,
		PURPLE		= 5,
		ORANGE		= 6,
		GREY		= 7,
		DARKER_GREY	= 8,
		MEDIUM_BLUE	= 9,
		LIGHT_GREEN	= 10,
		TEAL		= 11,
		RED_ORANGE	= 12,
		LIGHT_PURPLE= 13,
		YELLOW		= 14,
		WHITE		= 15
	};

public:

	/*********************************************************************************/

	static void ClearScreen(void)  //clear screen
	{
		CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;                            
		HANDLE    hConsoleOut;
		COORD    Home = {0,0};
		DWORD    dummy;

		hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

		FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);    
		csbiInfo.dwCursorPosition.X = 0;                                   
		csbiInfo.dwCursorPosition.Y = 0;                                   
		SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);    
	}

	/*********************************************************************************/

	static void ClearEndLine(void)  //clear end of line
	{
		CONSOLE_SCREEN_BUFFER_INFO        csbiInfo;                           
		HANDLE    hConsoleOut;
		COORD    Home,pos;
		DWORD    dummy;

		hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

		Home = csbiInfo.dwCursorPosition;
		pos.X = GetLineWidth() - csbiInfo.dwCursorPosition.X;

		FillConsoleOutputCharacter(hConsoleOut,' ',pos.X,Home,&dummy);
	}

	/*********************************************************************************/

	static void ClearEndScreen(void)  //clear end of screen
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;                           
		HANDLE    hConsoleOut;
		COORD    Home;
		DWORD    dummy;

		hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

		Home=csbiInfo.dwCursorPosition;
		FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	}

	/*********************************************************************************/

	static void ClearLine(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		MoveTo(0,csbInfo.dwCursorPosition.Y);
		ClearEndLine();
	}

	/*********************************************************************************/

	static void MoveTo(uint16_t x,uint16_t y)  
	{
		CONSOLE_SCREEN_BUFFER_INFO        csbiInfo;                            
		HANDLE    hConsoleOut;

		hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

		csbiInfo.dwCursorPosition.X = x;                                    
		csbiInfo.dwCursorPosition.Y = y;                                    
		SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);    
	}


	/*********************************************************************************/

	/* 屏幕光标定位,x为列(横),y为行(纵) */

	static int GotoTextPos(uint16_t x, uint16_t y)
	{
		COORD cd;
		cd.X = x;
		cd.Y = y;
		return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
	}

	/*********************************************************************************/

	/* 设置文本颜色,0~15 */

	static int SetTextColor(uint8_t fColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, fColor | (csbInfo.wAttributes&~0x0F));
	}

	/*********************************************************************************/

	/* 获取文本颜色,0~15 */

	static E_COLOR GetTextColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return (E_COLOR)(csbInfo.wAttributes&0x0F);
	}

	/*********************************************************************************/

	/* 设置文本背景颜色,0~15 */

	static int SetBackColor(uint8_t bColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, (bColor << 4) | (csbInfo.wAttributes&~0xF0));
	}

	/*********************************************************************************/

	/* 获取文本背景颜色,0~15 */

	static E_COLOR GetBackColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return (E_COLOR)((csbInfo.wAttributes&0xF0) >> 4);
	}

	/*********************************************************************************/

	/* 设置文本及其背景颜色,0~15 */

	static int SetColors(uint8_t fColor, uint8_t bColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, fColor | (bColor << 4) | (csbInfo.wAttributes&~0xFF));
	}

	/*********************************************************************************/

	/* 设置/取消前/背景颜色的交换解析 */

	static int SetSwapColors(bool b)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		if (!!b)
			return SetConsoleTextAttribute(hd, csbInfo.wAttributes | 0x4000);
		else
			return SetConsoleTextAttribute(hd, csbInfo.wAttributes & ~0x4000);
	}

	/*********************************************************************************/

	/* 设定/取消文字的下划线 */

	static int SetUnderLine(bool b)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		if (!!b)
			return SetConsoleTextAttribute(hd, csbInfo.wAttributes | 0x8000);
		else
			return SetConsoleTextAttribute(hd, csbInfo.wAttributes & ~0x8000);
	}

	/*********************************************************************************/

	/* 获取控制台文本行的最大长度[默认为80] */

	static int16_t GetLineWidth(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwSize.X;
	}

	/*********************************************************************************/

	/* 获取光标的横坐标[列数] */

	static int16_t GetCursorX(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwCursorPosition.X;
	}

	/*********************************************************************************/

	/* 获取光标的纵坐标[行数] */

	static int16_t GetCursorY(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwCursorPosition.Y;
	}

	/*********************************************************************************/

	/* 设置光标的可见性 */

	static int SetCursorVisible(bool b)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO ccInfo;
		::GetConsoleCursorInfo(hd, &ccInfo);
		ccInfo.bVisible = !!b;
		return ::SetConsoleCursorInfo(hd, &ccInfo);
	}

	/*********************************************************************************/

	/* 设置光标的(厚度)尺寸,1-100 */

	static int SetCursorSize(uint8_t s)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO ccInfo;
		::GetConsoleCursorInfo(hd, &ccInfo);
		ccInfo.dwSize = s;
		return ::SetConsoleCursorInfo(hd, &ccInfo);
	}

	/*********************************************************************************/

	/* 获取控制台的标题字符串 */

	static int GetTitle(char *title, uint8_t len)
	{
		return ::GetConsoleTitle(title, len);
	}

	/*********************************************************************************/

	/* 设置控制台的标题字符串 */

	static int SetTitle(char *title)
	{
		return ::SetConsoleTitle(title);
	}

	/*********************************************************************************/
};
#endif

#endif //_MYLIB_CONSOLE_H_INCLUDE_20081127_


