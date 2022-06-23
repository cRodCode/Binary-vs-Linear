// Project:     StatusClass
// Author:      Mark Berrett
// Date:        May 8, 2021
// Purpose:     Header file for StatusClass
//				Display status string in hwnd label
//

#pragma once

#define STARTING 0
#define LOADING 1
#define LOADFAIL 2
#define LOADED 3
#define ENTERED 4
#define NOENTRY 5
#define MATCH 6
#define NOMATCH 7
#define LINEAR 8
#define BINARY 9

class StatusClass
{
public:
	void Change(HWND, int);					// display text in hwnd

private:
	string strStatus[10] = {
		"Starting...",
		"Loading file 'S21_023Lab_19_Data.txt' ...",
		"Unable to load file 'S21_023_Lab_19_Data.txt'",
		"Binary Tree loaded",
		"Search string entered",
		"Search string required",
		"Search string found",
		"Search string not found",
		"Running linear search",
		"Running binary search"
	};
};

