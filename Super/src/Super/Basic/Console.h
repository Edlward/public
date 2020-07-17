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

	/* ��ɫ���� */
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

	/* ��Ļ��궨λ,xΪ��(��),yΪ��(��) */

	static int GotoTextPos(uint16_t x, uint16_t y)
	{
		COORD cd;
		cd.X = x;
		cd.Y = y;
		return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
	}

	/*********************************************************************************/

	/* �����ı���ɫ,0~15 */

	static int SetTextColor(uint8_t fColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, fColor | (csbInfo.wAttributes&~0x0F));
	}

	/*********************************************************************************/

	/* ��ȡ�ı���ɫ,0~15 */

	static E_COLOR GetTextColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return (E_COLOR)(csbInfo.wAttributes&0x0F);
	}

	/*********************************************************************************/

	/* �����ı�������ɫ,0~15 */

	static int SetBackColor(uint8_t bColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, (bColor << 4) | (csbInfo.wAttributes&~0xF0));
	}

	/*********************************************************************************/

	/* ��ȡ�ı�������ɫ,0~15 */

	static E_COLOR GetBackColor(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return (E_COLOR)((csbInfo.wAttributes&0xF0) >> 4);
	}

	/*********************************************************************************/

	/* �����ı����䱳����ɫ,0~15 */

	static int SetColors(uint8_t fColor, uint8_t bColor)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(hd, &csbInfo);
		return SetConsoleTextAttribute(hd, fColor | (bColor << 4) | (csbInfo.wAttributes&~0xFF));
	}

	/*********************************************************************************/

	/* ����/ȡ��ǰ/������ɫ�Ľ������� */

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

	/* �趨/ȡ�����ֵ��»��� */

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

	/* ��ȡ����̨�ı��е���󳤶�[Ĭ��Ϊ80] */

	static int16_t GetLineWidth(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwSize.X;
	}

	/*********************************************************************************/

	/* ��ȡ���ĺ�����[����] */

	static int16_t GetCursorX(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwCursorPosition.X;
	}

	/*********************************************************************************/

	/* ��ȡ����������[����] */

	static int16_t GetCursorY(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbInfo);
		return csbInfo.dwCursorPosition.Y;
	}

	/*********************************************************************************/

	/* ���ù��Ŀɼ��� */

	static int SetCursorVisible(bool b)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO ccInfo;
		::GetConsoleCursorInfo(hd, &ccInfo);
		ccInfo.bVisible = !!b;
		return ::SetConsoleCursorInfo(hd, &ccInfo);
	}

	/*********************************************************************************/

	/* ���ù���(���)�ߴ�,1-100 */

	static int SetCursorSize(uint8_t s)
	{
		HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO ccInfo;
		::GetConsoleCursorInfo(hd, &ccInfo);
		ccInfo.dwSize = s;
		return ::SetConsoleCursorInfo(hd, &ccInfo);
	}

	/*********************************************************************************/

	/* ��ȡ����̨�ı����ַ��� */

	static int GetTitle(char *title, uint8_t len)
	{
		return ::GetConsoleTitle(title, len);
	}

	/*********************************************************************************/

	/* ���ÿ���̨�ı����ַ��� */

	static int SetTitle(char *title)
	{
		return ::SetConsoleTitle(title);
	}

	/*********************************************************************************/
};
#endif

#endif //_MYLIB_CONSOLE_H_INCLUDE_20081127_


