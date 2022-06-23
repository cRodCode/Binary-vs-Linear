// Module:      Utilities
// Author:      Mark Berrett
// Date:        March 9, 2021
// Purpose:		Template class virtual functions.
//				useful utilities
//				TCHAR2Int()			converts a TCHAR to an integer
//				Int2TCHAR()			converts an integer to a TCHAR
//				TCHAR2String()		converts a TCHAR to a string
//				String2TCHAR()		converts a string to a TCHAR
//

#pragma once

#define TCHAR_SIZE 128			// standars size of TCHAR arrays

#include <string>				// string library
using namespace std;			// set standard namespace

// converts TCHAR to integer, returns integer
// truncates decimal values
// returns MAXINT if too many characters
// returns 0 if non-digital input
template <class Type>
int TCHAR2Int(Type* szIn)
{
	// convert TCHAR to int
	return _tstoi(szIn);
}


// converts integer into a TCHAR
// return value in TCHAR parameter
// assumes radix of 10
template <class Type>
void Int2TCHAR(Type iIn, TCHAR* szIn)
{
	// assumes TCHAR was created using this utility
	_itot_s(iIn, szIn, TCHAR_SIZE, 10);
}


// converts TCHAR to double
// uses the standard utility _tcstod()
// _tcstod()'s 2nd parameter is the return value that points to the 1st char that could not be converted
// returns 0.0 if non-digital input
template <class Type>
double TCHAR2Double(Type* szIn)
{
	return _tcstod(szIn, NULL); // 2nd parameter not used
}


// converts double to TCHAR
// iPrecision determines the number of decimal places
// returns pointer to local TCHAR
template <class Type>
void Double2TCHAR(Type dIn, TCHAR* szReturn, int iPrecision)
{
	TCHAR szControl[10];			// convertion mask
	TCHAR szWork[TCHAR_SIZE];		// completed TCHAR

	_stprintf_s(szControl,			// create mask conversion
		TEXT("%%.%df"),				// mask template
		iPrecision);				// user specified precision (decimal places)

	_stprintf_s(szWork,				// convert double to TCHAR
			szControl,				// control string
		dIn);						// value

	// copy completed TCHAR to return
	_tcscpy_s(szReturn, TCHAR_SIZE, szWork);
}

// converts TCHAR to string
// uses pointer to string (calling code needs &string)
template <class Type>
void TCHAR2String(TCHAR* szIn, Type* strIn)
{
	if (lstrlen(szIn) == 0)						// must be characters to store
		strIn->erase();							//		then empty return string
	else
	{
		wstring temp(szIn);						// convert TCHAR to stream
		string str(temp.begin(),				// convert stream to string
			temp.end());

		strIn->append(str);						// copy temp string to return string
	}
}

// converts string to TCHAR
// modifies TCHAR in place
// returns mbstowcs_s return code
// max TCHAR_SIZE char string length
template <class Type>
size_t String2TCHAR(Type* strIn, TCHAR* szIn)
{
	size_t pReturnValue = 0;					// return code from mbstowcs_s

	if (strIn->length() == 0 ||
		strIn->length() > TCHAR_SIZE)			// don't attempt to convert if bad input string size
	{
		memset(szIn, 0, sizeof(szIn));
	}
	else
	{
		mbstowcs_s(								// convert ASCII to Unicode
			&pReturnValue,						// convert ok
			szIn,								// destination string
			TCHAR_SIZE,							// max size of destination
			strIn->c_str(),						// source string
			TCHAR_SIZE);						// max chars to copy

	}
	return pReturnValue;						// return error code
}
