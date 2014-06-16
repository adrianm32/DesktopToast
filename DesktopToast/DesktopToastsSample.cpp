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
		wcex.cbWndExtra = 0;
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
				L"DesktopToastsApp",
				L"Desktop Toasts Demo App",
				WS_OVERLAPPEDWINDOW,

				);
		}

	}


	return S_OK;
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

LRESULT CALLBACK DesktopToastsApp::WndProc(    //callback and winapi are both macros for __stdcall which is callee clears the stack. __cdecl is caller clears the stack. The __fastcall calling convention specifies that arguments to functions are to be passed in registers, when possible.
	_In_ HWND hWnd,
	_In_ UINT message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	return S_OK;
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
