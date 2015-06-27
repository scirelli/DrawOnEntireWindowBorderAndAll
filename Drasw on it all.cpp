
// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <mmsystem.h> //needed to play a sound using windows also need to insert winmm.lib
					  //C:\Program Files\Microsoft Visual Studio\VC98\Lib


// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"  // class name
#define WINDOW_WIDTH  320              // size of window
#define WINDOW_HEIGHT 200

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// PROTOTYPES /////////////////////////////////////////////


// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[80];                // used to print text
HPEN green_pen;

// FUNCTIONS //////////////////////////////////////////////

//\\\\\\\\\\\\\\\\\\\\\\\\\ WinProc ///////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   //creates a handle to the device context

	//find out what the message is
	switch(msg)
	{	
	
	case WM_CREATE: //called when window is created
		{// do initialization stuff here
			
			green_pen = CreatePen(PS_SOLID, 0, RGB(0,255,0)); 

			return(0);
		} break;

	case WM_PAINT: //called when window needs repainting
		{//simply validate the window
		    
			//ValidateRect(hwnd, NULL);
			hdc = BeginPaint(hwnd,&ps);
		    EndPaint(hwnd,&ps);
			
			return(0);
		} break;

	case WM_DESTROY: 
		{// kill the application			
			//close the program
			DeleteObject(green_pen);
			PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

//\\\\\\\\\\\\\\\\\\\\\\\\ WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{

	WNDCLASS winclass;	// this will hold the class created
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	HDC		 hwdc;      //gets a handle to the dc of the entire window border and all
	HDC		 htdc;

	//fill in the window class stucture
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
							  "WinX Game Console",	 // title
							  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						 	  0,0,	   // x,y
							  WINDOW_WIDTH,  // width
						      WINDOW_HEIGHT, // height
							  NULL,	   // handle to parent 
							  NULL,	   // handle to menu
							  hinstance,// instance
							  NULL)))	// creation parms
	{
		MessageBox(hwnd, "Window Could not be Created", NULL, MB_OK); //NULL is default for Error
		return(0);
	}

	// save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance      = hinstance;

	
	// enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit message
			if (msg.message == WM_QUIT)
				break;
	
			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
    
    	//hdc= GetDC(hwnd);
		hwdc= GetWindowDC(hwnd); //this allows you to draw on the windows border
		//htdc= GetWindowDC( GetTopWindow(hwnd) ); //gets the top window of the this window
												 //seems to also work with GetDC but this draws on
												 //top of any window below it (visually)
		SelectObject(hwdc, green_pen);
		
		MoveToEx(htdc, 0,0, NULL);
		LineTo(hwdc,322,200);
		
		MoveToEx(hwdc, 322,0, NULL);
		LineTo(hwdc,0,200);

		//slow things down a bit
		Sleep(10);
		//ReleaseDC(hwnd, htdc);
		//ReleaseDC(hwnd, hdc);
		ReleaseDC(hwnd, hwdc);
	} // end while

	

	// return to Windows like this
	return(msg.wParam);

} // end WinMain


