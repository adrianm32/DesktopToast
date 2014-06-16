#include "stdafx.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace Windows::Foundation;

int WINAPI WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow)
{
	HRESULT hr = S_OK;
  
	hr = Windows::Foundation::Initialize(RO_INIT_TYPE::RO_INIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		{
			DesktopToastsApp app;
			hr = app.Initialize();

			if (SUCCEEDED(hr))
			{
				app.RunMessageLoop();
			}

			Windows::Foundation::Uninitialize();
		}
	}

	return SUCCEEDED(hr);
}

DesktopToastsApp::DesktopToastsApp() : _hWnd(nullptr), _hEdit(nullptr)
{
}

DesktopToastsApp::~DesktopToastsApp()
{}

HRESULT DesktopToastsApp::Initialize()
{
	HRESULT hr = TryCreateShortcut();

	if (SUCCEEDED(hr))
	{
		ATOM atom;

		WNDCLASSEX wcex;

		//Register window class
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DesktopToastsApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // standard Icon ID   in winuser.h
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // standard cursor ID  in winuser.h
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"DesktopToastsApp";
		wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		atom = RegisterClassEx(&wcex);

		hr = atom ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			_hWnd = CreateWindow(
				L"DesktopToastsApp",  /*  The class name can be any name registered with RegisterClass or RegisterClassEx, provided that the module that registers the class is also the module that creates the window. The class name can also be any of the predefined system class names. For a list of system class names,*/
				L"Desktop Toasts Demo App",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				350,
				200,
				nullptr,  //parent window
				nullptr,
				HINST_THISCOMPONENT,
				this  //lparam, passing this class as parameter to the window.
				);

			hr = _hWnd ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{
				CreateWindow(
					L"Button", /* predefined system class name: button, combobox, edit, listbox,richedit, scrollbar,static*/
					L"View Text Toast",
					BS_PUSHBUTTON |  //Button Styles
						WS_CHILD | WS_VISIBLE,  // visible Child Window
					0,0, //top left
					150, //width
					25,  //height
					_hWnd, //parent window
					reinterpret_cast<HMENU>(HM_TEXTBUTTON), // For a child window, hMenu specifies the child-window identifier, an integer value used by a dialog box control to notify its parent about events. The application determines the child-window identifier; it must be unique for all child windows with the same parent window.
					HINST_THISCOMPONENT,
					nullptr  //lpparam
					);

				_hEdit = CreateWindow(
					L"EDIT",
					L"Whatever action you take on the displayed toast will be shown here",
					ES_LEFT | //aligns text with left margin
						ES_MULTILINE | ES_READONLY |
						WS_CHILD | WS_VISIBLE | WS_BORDER, //visible child window with Border
					0,
					30,
					300,
					50,
					_hWnd, //parent window
					nullptr,
					HINST_THISCOMPONENT,
					nullptr  //lparam
					);

				ShowWindow(_hWnd, SW_SHOWNORMAL);  //sets the window state

				UpdateWindow(_hWnd); //The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT message to the window if the window's update region is not empty. The function sends a WM_PAINT message directly to the window procedure of the specified window, bypassing the application queue. If the update region is empty, no message is sent.
			}
		}
		else
		{
			MessageBox(
				nullptr, //hwnd
				L"Failed to install shortcut. Try running this application as administrator", //text
				L"Failed Launch", //caption
				MB_OK | MB_ICONERROR  //show ok button and error icon
				);
		}

	}


	return hr;
}

void DesktopToastsApp::RunMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DesktopToastsApp::TryCreateShortcut()
{

	return S_OK;
}

HRESULT DesktopToastsApp::InstallShortcut(_In_ wchar_t * shortcutPath)
{

	return S_OK;
}

/*
Once upon a time, Windows was 16 - bit.Each message could carry with it two pieces of data, called WPARAM and LPARAM.The first one was a 16 - bit value("word"), so it was called W.The second one was a 32 - bit value("long"), so it was called L.

You used the W parameter to pass things like handles and integers.You used the L parameter to pass pointers.

When Windows was converted to 32 - bit, the WPARAM parameter grew to a 32 - bit value as well.So even though the "W" stands for "word", it isn't a word any more. (And in 64-bit Windows, both parameters are 64-bit values!) 

It is helpful to understand the origin of the terms.If you look at the design of window messages, you will see that if the message takes a pointer, the pointer is usually passed in the LPARAM, whereas if the message takes a handle or an integer, then it is passed in the WPARAM. (And if a message takes both, the integer goes in the WPARAM and the pointer goes in the LPARAM.)
*/
LRESULT CALLBACK DesktopToastsApp::WndProc(    //callback and winapi are both macros for __stdcall which is callee clears the stack. __cdecl is caller clears the stack. The __fastcall calling convention specifies that arguments to functions are to be passed in registers, when possible.
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		DesktopToastsApp * app = reinterpret_cast<DesktopToastsApp *>(pcs->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);  //GWLP=> GetWindowLongPtr

		return 1; 
	}

	DesktopToastsApp * pApp = reinterpret_cast<DesktopToastsApp *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (pApp)
	{
		switch (message)
		{
		case WM_COMMAND:   //message sent when a user clicks on a button control/window.
		{
							   int wmId = LOWORD(wParam);
							   switch (wmId)
							   {
							   case HM_TEXTBUTTON:
								   pApp->TextButtonClicked();
								   break;
							   default:
								   return DefWindowProc(hWnd, message, wParam, lParam);
							   }
		}
			break;
		case WM_PAINT:
		{
						 PAINTSTRUCT ps;
						 HDC hdc;
						 hdc = BeginPaint(hWnd, &ps); // you then use the hdc to draw stuff eg. ::Rectangle(hdc, left, top, right, bottom) or ::Ellipse etc.
						 //dont draw anything, just refresh the windows.
						 EndPaint(hWnd, &ps);

		}

			break;
		case WM_DESTROY:
		{
						   PostQuitMessage(0);
		}
			return 1; 
		}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT DesktopToastsApp::TextButtonClicked()
{
	return S_OK;
}

HRESULT DesktopToastsApp::DisplayToast()
{
	return S_OK;
}

HRESULT DesktopToastsApp::CreateToastXml(
	_In_ IToastNotificationManagerStatics * toastManager,
	_Outptr_ IXMLDocument **xml
	)
{
	return S_OK;
}

HRESULT DesktopToastsApp::CreateToast(
	_In_ IToastNotificationManagerStatics * toastManager,
	_In_ IXMLDocument *xml
	)
{
	return S_OK;
}

HRESULT DesktopToastsApp::SetImageSrc(
	_In_z_ wchar_t * imagePath,   //_z_ null terminated parameter
	_In_ IXMLDocument *xml
	)
{
	return S_OK;
}

HRESULT DesktopToastsApp::SetTextValues(
	_In_reads_(textValuesCount) wchar_t ** textValues,   //2 dimensional array of strings
	_In_ UINT32 textValuesCount,
	_In_reads_(textValuesCount) UINT32 *textValuesLengths, // 1 dimensional array of string lengths
	_In_ IXMLDocument * toastXml
	)
{
	return S_OK;
}

HRESULT DesktopToastsApp::SetNodeValueString(
	_In_ HSTRING outputString,
	_In_ ABI::Windows::Data::Xml::Dom::IXmlNode * node,
	_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument * xml
	)
{
	return S_OK;
}
