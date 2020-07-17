#pragma once

////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2002-2016
// Computations & Graphics, Inc.
// Junlin Xu
//
// All rights reserved
//
//////////////////////////////////////////////////////////////////////////

#include "double128_api.h"

namespace cgiDouble128NS
{
	class DOUBLE128PRJ_API double128
	{
	public:
		enum {STRING_BUFFER_SIZE = 128};
		enum {QUAD_SIZE = 16};

		// constructors
		double128();
		double128(double val);
		double128(int val);
		double128(const double128& val);
		double128(const char* val);

		// destructor
		~double128(void);

		// conversion operators
		inline operator double()const;
		inline operator int()const;
		inline const char* getVal()const;
		inline void copyMemoryFromQuad(const void* val);
		inline void* copyMemoryToQuad(void*)const;
		// you should supply string buffer size of at least STRING_BUFFER_SIZE = 128 chars here
		inline char* str(char* buffer, int precision = 32, bool useScientific = false)const;

		// operators
		inline double128& operator +=(const double128& other);
		inline double128& operator +=(double val);
		inline double128& operator +=(int val);
		inline double128& operator -=(const double128& other);
		inline double128& operator -=(double val);
		inline double128& operator -=(int val);
		inline double128& operator *=(const double128& other);
		inline double128& operator *=(double val);
		inline double128& operator *=(int val);
		inline double128& operator /=(const double128& other);
		inline double128& operator /=(double val);
		inline double128& operator /=(int val);
		inline double128& operator ++();	// prefix
		inline double128 operator ++(int);	// postfix
		inline double128& operator --();
		inline double128 operator --(int);
	private:
		char m_val[QUAD_SIZE];
	};


	// binary operators
	DOUBLE128PRJ_API double128 operator -(const double128& val);  // Negate - 
	DOUBLE128PRJ_API double128 operator +(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator +(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 operator +(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 operator +(double val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator +(int val1, const double128& val12);
	DOUBLE128PRJ_API double128 operator -(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator -(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 operator -(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 operator -(double val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator -(int val1, const double128& val12);
	DOUBLE128PRJ_API double128 operator *(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator *(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 operator *(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 operator *(double val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator *(int val1, const double128& val12);
	DOUBLE128PRJ_API double128 operator /(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator /(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 operator /(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 operator /(double val1, const double128& val2);
	DOUBLE128PRJ_API double128 operator /(int val1, const double128& val12);

	// logical operators
	DOUBLE128PRJ_API bool operator==(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator!=(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator> (const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator>=(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator< (const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator<=(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API bool operator==(const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator!=(const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator> (const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator>=(const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator< (const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator<=(const double128& val1, double val2);
	DOUBLE128PRJ_API bool operator==(const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator!=(const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator> (const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator>=(const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator< (const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator<=(const double128& val1, int val2);
	DOUBLE128PRJ_API bool operator==(double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator!=(double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator> (double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator>=(double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator< (double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator<=(double val1, const double128& val2);
	DOUBLE128PRJ_API bool operator==(int val1, const double128& val2);
	DOUBLE128PRJ_API bool operator!=(int val1, const double128& val2);
	DOUBLE128PRJ_API bool operator> (int val1, const double128& val2);
	DOUBLE128PRJ_API bool operator>=(int val1, const double128& val2);
	DOUBLE128PRJ_API bool operator< (int val1, const double128& val2);
	DOUBLE128PRJ_API bool operator<=(int val1, const double128& val2);

	// math functions
	DOUBLE128PRJ_API double128 fabs(const double128& val);
	DOUBLE128PRJ_API double128 floor(const double128& val);
	DOUBLE128PRJ_API double128 ceil(const double128& val);
	DOUBLE128PRJ_API double128 sqrt(const double128& val);
	DOUBLE128PRJ_API double128 trunc(const double128& val);
	DOUBLE128PRJ_API double128 exp(const double128& val);
	DOUBLE128PRJ_API double128 pow(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 pow(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 pow(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 log(const double128& val);
	DOUBLE128PRJ_API double128 log10(const double128& val);
	DOUBLE128PRJ_API double128 sin(const double128& val);
	DOUBLE128PRJ_API double128 cos(const double128& val);
	DOUBLE128PRJ_API double128 tan(const double128& val);
	DOUBLE128PRJ_API double128 asin(const double128& val);
	DOUBLE128PRJ_API double128 acos(const double128& val);
	DOUBLE128PRJ_API double128 atan(const double128& val);
	DOUBLE128PRJ_API double128 sinh(const double128& val);
	DOUBLE128PRJ_API double128 cosh(const double128& val);
	DOUBLE128PRJ_API double128 tanh(const double128& val);
	DOUBLE128PRJ_API double128 fmod(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 fmod(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 fmod(const double128& val1, int val2);
	DOUBLE128PRJ_API double128 atan2(const double128& val1, const double128& val2);
	DOUBLE128PRJ_API double128 atan2(const double128& val1, double val2);
	DOUBLE128PRJ_API double128 atan2(const double128& val1, int val2);

	// arrays operations, a little faster
	DOUBLE128PRJ_API double128 arrayAdd(const double128* pVal1, const double128* pVal2, int count);
	DOUBLE128PRJ_API double128 arraySubtract(const double128* pVal1, const double128* pVal2, int count);
	DOUBLE128PRJ_API double128 arrayMultiply(const double128* pVal1, const double128* pVal2, int count);
	DOUBLE128PRJ_API double128 arrayDivide(const double128* pVal1, const double128* pVal2, int count);

	// data range
	DOUBLE128PRJ_API bool isnan(const double128& val);
	DOUBLE128PRJ_API bool isinf(const double128& val);
	DOUBLE128PRJ_API double128 quadMin();
	DOUBLE128PRJ_API double128 quadMax();
	DOUBLE128PRJ_API double128 quadLowest();
	DOUBLE128PRJ_API double128 quadEpsilon();
}