// Project:     StatusClass
// Author:      Mark Berrett
// Date:        May 8, 2021
// Purpose:     Implimentation file for StatusClass

#include "framework.h"
#include "S21_023_Lab_19.h"

void StatusClass::Change(HWND hWnd, int iStatus)
{
	TCHAR szOutput[TCHAR_SIZE];					// test to display

	// convert status text to TCHAR
	String2TCHAR(&strStatus[iStatus], szOutput);

	// set status window text
	SetWindowText(hWnd, szOutput);

}
