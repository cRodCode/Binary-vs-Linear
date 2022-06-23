// Project:     S21_023_Lab_19
// Author:      Carlos Rodriguez
// Date:        May 14, 2021
// Purpose:     Explore the advantages of using a binary tree with unordered data
//              Learn how to create a binary tree
//              Learn how to code a binary tree loading operation with unordered data
//              Review performing a search on an unordered list
//              Learn how to code a search in a binary tree
//

#include "framework.h"
#include "S21_023_Lab_19.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

StatusClass Status;                             // displays status text

HWND hWnd_BTN_String;							// Search String button
HWND hWnd_BTN_Linear;							// Linear Search button
HWND hWnd_BTN_BTree;							// BTree Search button
HWND hWnd_Input;								// search string input edit control
HWND hWnd_StringFound;							// search result message  static
HWND hWnd_LinStart;								// linear search start time message static
HWND hWnd_LinEnd;								// linear search end time message static
HWND hWnd_BTreeStart;							// b-tree search start time message static
HWND hWnd_BTreeEnd;								// b-tree search end time message static
HWND hWnd_Linear_Comps;							// # of linear comparisons message static
HWND hWnd_BTree_Comps;							// # of b-tree comparisons message static
HWND hWnd_LBL_Status;                           // "Loading..."

string strSearchString;                         // user provided search string

// create struct node
struct node
{
    string data;                            // stored data
    struct node* left;                      // left pointer
    struct node* right;                     // right pointer
};

struct node* newNode(string strData)
{
    struct node* node = new struct node;    // create new node
    node->data = strData;                   // set incoming strData to data
    node->left = NULL;                      // defualt left pointer to null
    node->right = NULL;                     // default right pointer to null
    return node;                            // return node
}

vector<node*> bTree;                        // vector to store nodes

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void LoadFile();                                // load file into list
void LoadBinaryTree(string);                    // add file data to binary tree
int LinearSearch();                             // perform linear search on unordered list
int BinaryTreeSearch();                         // perform search on binary tree


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_S21023LAB19, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_S21023LAB19));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_S21023LAB19));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_S21023LAB19);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 450, 360, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case BN_CLICKED:

            // enter search string button
            if (LOWORD(lParam) == (WORD)hWnd_BTN_String)
            {
                TCHAR szInput[TCHAR_SIZE];       // user entry

                // get user input
                SendMessage(hWnd_Input, WM_GETTEXT,
                    MAX_LOADSTRING,
                    LPARAM(szInput));

                // convert user input to search string
                TCHAR2String(szInput, &strSearchString);

                if (strSearchString.length() > 0)   // if user entered text
                {
                    // set status
                    Status.Change(hWnd_LBL_Status, ENTERED);

                    // set search button
                    EnableWindow(hWnd_BTN_Linear, true);	// linear on
                }
                else                                // user entry is blank
                {
                    // set status
                    Status.Change(hWnd_LBL_Status, NOENTRY);

                    // set search button
                    EnableWindow(hWnd_BTN_Linear, false);	// linear off
                }

                // clear search results
                SetWindowText(hWnd_Linear_Comps, _T(""));
                SetWindowText(hWnd_BTree_Comps, _T(""));
                SetWindowText(hWnd_LinStart, _T("88:88:888"));
                SetWindowText(hWnd_LinEnd, _T("88:88:888"));
                SetWindowText(hWnd_BTreeStart, _T("88:88:888"));
                SetWindowText(hWnd_BTreeEnd, _T("88:88:888"));

                // disable outputs
                EnableWindow(hWnd_LinStart, false);			// Linear search start time static control
                EnableWindow(hWnd_LinEnd, false);			// Linear search end time static control
                EnableWindow(hWnd_BTreeStart, false);			// Binary search start time static control
                EnableWindow(hWnd_BTreeEnd, false);			// Binary search end time static control

                EnableWindow(hWnd_BTN_BTree, false);	// binary off
            }

            // linear search button
            else if (LOWORD(lParam) == (WORD)hWnd_BTN_Linear) // perform linear search
            {
                SYSTEMTIME sysTime;                             // time struct
                int iFoundLine = -1;                            // number of hits
                TCHAR szOutput[TCHAR_SIZE];                     // 

                // change status
                Status.Change(hWnd_LBL_Status, LINEAR);         // running linear search

                // get and display start time
                GetSystemTime(&sysTime);
                _stprintf_s(szOutput, TEXT("%d:%d.%d"), sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
                SetWindowText(hWnd_LinStart, szOutput);
                EnableWindow(hWnd_LinStart, true);				// Linear search start time static control

                iFoundLine = LinearSearch();					// perform the linear search
                if (iFoundLine > -1)							// if found
                {
                    // message, line number
                    Int2TCHAR(iFoundLine, szOutput);
                    SetWindowText(hWnd_Linear_Comps, szOutput);
                    EnableWindow(hWnd_Linear_Comps, true);

                    EnableWindow(hWnd_BTN_BTree, true);	// binary on

                    // change status
                    Status.Change(hWnd_LBL_Status, MATCH);      // found
                }
                else
                    // change status
                    Status.Change(hWnd_LBL_Status, NOMATCH);    // not found


                // get and display end time
                GetSystemTime(&sysTime);
                _stprintf_s(szOutput, TEXT("%d:%d.%d"), sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
                SetWindowText(hWnd_LinEnd, szOutput);
                EnableWindow(hWnd_LinEnd, true);				// Linear search end time static control

            }

            // binary search button
            else if (LOWORD(lParam) == (WORD)hWnd_BTN_BTree) // perform binary search
            {
                SYSTEMTIME sysTime;                         // time struct
                TCHAR szOutput[TCHAR_SIZE];                 // output 
                int iHits = -1;                             // return value from BinarySearch()

                // change status
                Status.Change(hWnd_LBL_Status, BINARY);     // running binary search

                // get and display start time
                GetSystemTime(&sysTime);
                _stprintf_s(szOutput, TEXT("%d:%d.%d"), sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
                SetWindowText(hWnd_BTreeStart, szOutput);
                EnableWindow(hWnd_BTreeStart, true);			// B-Tree search start time static control

                iHits = BinaryTreeSearch();			        // perform search on binary tree

                Int2TCHAR(iHits, szOutput);                 // in to TCHAR
                SetWindowText(hWnd_BTree_Comps, szOutput); // set window text
                EnableWindow(hWnd_BTree_Comps, true);

                // get and display end time
                GetSystemTime(&sysTime);
                _stprintf_s(szOutput, TEXT("%d:%d.%d"), sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
                SetWindowText(hWnd_BTreeEnd, szOutput);
                EnableWindow(hWnd_BTreeEnd, true);			// B-Tree search end time static control

                // change status
                Status.Change(hWnd_LBL_Status, MATCH);      // found
            }

            break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;


    case WM_CREATE:

        // set the text in the title bar to your name
        SetWindowText(hWnd, TEXT("Carlos Rodriguez"));

        // buttons
        hWnd_BTN_String = CreateWindow(TEXT("BUTTON"), TEXT("Search String"),
            WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 60, 110, 30, hWnd, NULL, NULL, NULL);

        hWnd_BTN_Linear = CreateWindow(TEXT("BUTTON"), TEXT("Linear Search"),
            WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 125, 110, 30, hWnd, NULL, NULL, NULL);

        hWnd_BTN_BTree = CreateWindow(TEXT("BUTTON"), TEXT("B-Tree Search"),
            WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            20, 190, 110, 30, hWnd, NULL, NULL, NULL);

        // input
        hWnd_Input = CreateWindow(TEXT("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_CHILD,
            160, 65, 250, 20, hWnd, NULL, NULL, NULL);

        // static text - labels
        CreateWindow(TEXT("STATIC"),
            TEXT("LINEAR vs BINARY-TREE SEARCH COMPARISON"),
            WS_VISIBLE | WS_CHILD,50, 20, 320, 20, hWnd, NULL, NULL, NULL);

        hWnd_LinStart = CreateWindow(TEXT("STATIC"),
            TEXT("88:88.888"),
            WS_VISIBLE | WS_CHILD, 210, 120, 80, 20, hWnd, NULL, NULL, NULL);

        hWnd_LinEnd = CreateWindow(TEXT("STATIC"),
            TEXT("88:88.888"),
            WS_VISIBLE | WS_CHILD, 345, 120, 80, 20, hWnd, NULL, NULL, NULL);

        hWnd_Linear_Comps = CreateWindow(TEXT("STATIC"),
            TEXT(""),
            WS_VISIBLE | WS_CHILD, 345, 140, 300, 20, hWnd, NULL, NULL, NULL);

        hWnd_BTreeStart = CreateWindow(TEXT("STATIC"),
            TEXT("88:88.888"),
            WS_VISIBLE | WS_CHILD, 210, 185, 80, 20, hWnd, NULL, NULL, NULL);

        hWnd_BTreeEnd = CreateWindow(TEXT("STATIC"),
            TEXT("88:88.888"),
            WS_VISIBLE | WS_CHILD, 345, 185, 80, 20, hWnd, NULL, NULL, NULL);

        hWnd_BTree_Comps = CreateWindow(TEXT("STATIC"),
            TEXT(""),
            WS_VISIBLE | WS_CHILD, 345, 205, 300, 20, hWnd, NULL, NULL, NULL);


        CreateWindow(TEXT("STATIC"),
            TEXT("Start:"),
            WS_VISIBLE | WS_CHILD, 160, 120, 40, 20, hWnd, NULL, NULL, NULL);

        CreateWindow(TEXT("STATIC"),
            TEXT("End:"),
            WS_VISIBLE | WS_CHILD, 300, 120, 40, 20, hWnd, NULL, NULL, NULL);

        CreateWindow(TEXT("STATIC"),
            TEXT("Number of comparisons:"),
            WS_VISIBLE | WS_CHILD, 160, 140, 200, 20, hWnd, NULL, NULL, NULL);

        CreateWindow(TEXT("STATIC"),
            TEXT("Start:"),
            WS_VISIBLE | WS_CHILD, 160, 185, 40, 20, hWnd, NULL, NULL, NULL);

        CreateWindow(TEXT("STATIC"),
            TEXT("End:"),
            WS_VISIBLE | WS_CHILD, 300, 185, 40, 20, hWnd, NULL, NULL, NULL);

        CreateWindow(TEXT("STATIC"),
            TEXT("Number of comparisons:"),
            WS_VISIBLE | WS_CHILD, 160, 205, 200, 20, hWnd, NULL, NULL, NULL);

        hWnd_LBL_Status = CreateWindow(TEXT("STATIC"),
            TEXT("Starting..."),
            WS_VISIBLE | WS_CHILD, 160, 240, 250, 40, hWnd, NULL, NULL, NULL);


        // disable buttons, input and output to forse sequensing
        EnableWindow(hWnd_BTN_String, false);		// Search String button disabled until file loaded
        EnableWindow(hWnd_Input, false);    		// Input edit control disabled until file loaded
        EnableWindow(hWnd_BTN_Linear, false);		// Linear Search button disabled until search string entered
        EnableWindow(hWnd_BTN_BTree, false);		// Binary Search button disabled until search string entered

        EnableWindow(hWnd_LinStart, false);			// Linear search start time static control
        EnableWindow(hWnd_LinEnd, false);			// Linear search end time static control
        EnableWindow(hWnd_BTreeStart, false);			// Binary search start time static control
        EnableWindow(hWnd_BTreeEnd, false);			// Binary search end time static control

        LoadFile();                                 // load file into list

        break;

    case WM_DESTROY:
        bTree.clear();          // clear vector
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// load file into binary tree
void LoadFile()
{
    ifstream fInput;                            // file handle
    string strData;                             // line of text from file

    // change status
    Status.Change(hWnd_LBL_Status, LOADING);

    try
    {
        fInput.open("S21_023_Lab_19_Data.txt");     // open data file

        if (!fInput)                            // did the file open okay
            throw 1;                            //      no, throw exception

        while (getline(fInput, strData))        // read to end of file
        {
            LoadBinaryTree(strData);            // add file data to binary tree
        }

        fInput.close();                         // close data file

        // change status
        Status.Change(hWnd_LBL_Status, LOADED);
        EnableWindow(hWnd_BTN_String, true);	// Search String button enabled
        EnableWindow(hWnd_Input, true);    		// Input edit control enabled

    }
    catch (int)
    {
        // change status
        Status.Change(hWnd_LBL_Status, LOADFAIL);
    }

}

// add file data to binary tree
void LoadBinaryTree(string strData)
{
    struct node* nodeData = newNode(strData);
    bTree.push_back(nodeData); // set root node into b tree 
    int newIndex = bTree.size() - 1;   // index


    if (newIndex > 0)                 // nothing links to the root node
    {
        int iIndex = 0;
        node* tempNode = bTree[iIndex]; // start at root
        bool done = false;             // set to true when new string inserted into bTree
        while (!done) // loop until string inserted
        {
            // compare new string with temp string
            if (nodeData->data < tempNode->data) // if new < temp, go left
            {
                if (tempNode->left == NULL)   // if left link is empty
                {
                    tempNode->left = nodeData;  // temp node now points to new node
                    done = true; // insertion done
                }
                else
                {
                    tempNode = tempNode->left;          // move to node on the left
                }
            }
            else // else new > temp, go right
            {
                if (tempNode->right == NULL)  // if right link is empty
                {
                    tempNode->right = nodeData; // temp node now points to new node
                    done = true; // insertion done
                }
                else
                {
                    tempNode = tempNode->right; // move to node on the right
                }
            }
        }
    }
}


// perform linear search on unordered list
int LinearSearch()
{
    int hits = 0;   // counter

    for (int i = 0; i < bTree.size(); i++)
    {
        // increment
        hits++;

        // compare search with list
        int iCompare = strSearchString.compare(bTree[i]->data);

        // if iCompare equals zero match found
        if (iCompare == 0)
        {
            return hits;    // return number of tries
        }
    }


    // not found
    return -1;
}

// perform search on binary tree
int BinaryTreeSearch()
{
    int hits = 0;                           // counter
    bool done = false;                      // end search
    bool found = false;                     // match ?
    int iIndex = 0;
    node* tempNode = bTree[iIndex];         // start at root

    while (!done) // loop till found or end of btree
    {
        // increment
        hits++;

        // look at current node
        if (strSearchString == tempNode->data) // match?
        {
            found = true; // got a match
            done = true; // end search
            return hits;    // return number of tries
        }

        else if (strSearchString < tempNode->data) // if search < node, go left
            if (tempNode->left == NULL)         // if left is empty
                done = true;                   // end of search
            else
            {
                tempNode = tempNode->left;     // move to the left
            }

        else // must be >, go right
            if (tempNode->right == NULL)     // if right is empty
                done = true;                // end of search
            else
            {
                tempNode = tempNode->right;   // move to the right
            }
    }

    // not found
    return -1;
}