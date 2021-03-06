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


// In order to display toasts, a desktop application must have a shortcut on the Start menu.
// Also, an AppUserModelID must be set on that shortcut.
// The shortcut should be created as part of the installer. The following code shows how to create
// a shortcut and assign an AppUserModelID using Windows APIs. You must download and include the 
// Windows API Code Pack for Microsoft .NET Framework for this code to function
HRESULT DesktopToastsApp::TryCreateShortcut()
{
	HRESULT hr = S_OK;

	wchar_t shortcutPath[MAX_PATH];
	DWORD charWritten;
	DWORD attributes;
	bool fileExists;

	charWritten = GetEnvironmentVariableW(L"APPDATA", shortcutPath, MAX_PATH);
	IFC(charWritten > 0 ? S_OK : E_INVALIDARG);
	IFC(StringCchCatW(shortcutPath, ARRAY_SIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\Desktop Toasts App 1.lnk"));

	attributes = GetFileAttributes(shortcutPath);
	fileExists = attributes < INVALID_FILE_ATTRIBUTES;

	if (!fileExists)
	{
		IFC(InstallShortcut(shortcutPath));
	}

Cleanup:
	return hr;
}

HRESULT DesktopToastsApp::InstallShortcut(_In_ wchar_t * shortcutPath)
{
	HRESULT hr = S_OK;

	wchar_t exePath[MAX_PATH];
	DWORD charWritten;
	ComPtr<IShellLink> shellLink;  //shObjIdl.h  shell object api
	ComPtr<IPropertyStore> propertyStore;
	PROPVARIANT appIdPropVar;
	ComPtr<IPersistFile> persistFile;
 
	charWritten = GetModuleFileNameEx(GetCurrentProcess(), nullptr, exePath, ARRAYSIZE(exePath));
	IFC(charWritten > 0 ? S_OK : E_FAIL);

	//IFC(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink)));
	IFC(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, __uuidof(shellLink.Get()),  &shellLink));

	IFC(shellLink->SetPath(exePath));
	IFC(shellLink->SetArguments(L""));

	IFC(shellLink.As(&propertyStore));
	IFC(InitPropVariantFromString(AppId, &appIdPropVar));
	IFC(propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar));
	IFC(propertyStore->Commit());

	IFC(shellLink.As(&persistFile));
	IFC(persistFile->Save(shortcutPath, TRUE));

Cleanup:
	PropVariantClear(&appIdPropVar);
	return hr;
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
							   int wmId = LOWORD(wParam); // For a child window, hMenu specifies the child-window identifier, an integer value used by a dialog box control to notify its parent about events. The application determines the child-window identifier; it must be unique for all child windows with the same parent window.
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
	return DisplayToast();
}

// Display the toast using classic COM. Note that is also possible to create and display the toast using the new C++ /ZW options (using handles,
// COM wrappers, etc.) 
HRESULT DesktopToastsApp::DisplayToast()
{
	HRESULT hr = S_OK;

	ComPtr<IToastNotificationManagerStatics> toastStatics;
	ComPtr<ABI::Windows::Data::Xml::Dom::IXmlDocument> toastXml;

	IFC(GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), toastStatics.ReleaseAndGetAddressOf()));
	IFC(CreateToastXml(toastStatics.Get(), toastXml.ReleaseAndGetAddressOf()));
	IFC(CreateToast(toastStatics.Get(), toastXml.Get()));

Cleanup:
	return hr;
}

// Create the toast XML from a template
HRESULT DesktopToastsApp::CreateToastXml(
	_In_ IToastNotificationManagerStatics * toastManager,
	_Outptr_ ABI::Windows::Data::Xml::Dom::IXmlDocument **inputXml
	)
{
	HRESULT hr = S_OK;
	wchar_t * imagePath;
	wchar_t * textValues[] = {  //String constants are not allocated on the "heap" - where the memory that you need to free() is allocated. They are allocated in another special segment, and there's no way to actually "free" this memory - i.e. make it useful for something else in the program.
		L"Line 1",
		L"Line 2",  //L is not a macro, it's just the standard prefix for wide (wchar_t, "Unicode") string literals; the concept is similar to the L suffix for long int literals, f suffix for float literals and so on1.
		L"Line 3"
	};

	UINT textLengths[] = {6, 6, 6};

	IFC(toastManager->GetTemplateContent(ToastTemplateType_ToastImageAndText04, inputXml));

	imagePath = _wfullpath(nullptr, L"toastImageAndText.png", MAX_PATH);  //stdlib helper function.
	IFC(imagePath ? S_OK : HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));
	IFC(SetImageSrc(imagePath, *inputXml));

	IFC(SetTextValues(textValues, 3, textLengths, *inputXml));

Cleanup:
	return hr;
}

HRESULT DesktopToastsApp::CreateToast(
	_In_ IToastNotificationManagerStatics * toastManager,
	_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *xml
	)
{
	HRESULT hr = S_OK;

	ComPtr<IToastNotifier> notifier;
	ComPtr<IToastNotificationFactory> factory;
	ComPtr<IToastNotification> toast;
	EventRegistrationToken activatedToken, dismissedToken, failedToken;
	ComPtr<ToastEventHandler> eventHandler;

	IFC(toastManager->CreateToastNotifierWithId(StringReferenceWrapper(AppId).Get(), notifier.ReleaseAndGetAddressOf()));
	IFC(GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), factory.ReleaseAndGetAddressOf()));
	IFC(factory->CreateToastNotification(xml, toast.ReleaseAndGetAddressOf()));
	
	//Register the event handlers
	eventHandler = new ToastEventHandler(_hWnd, _hEdit);
	IFC(toast->add_Activated(eventHandler.Get(), &activatedToken));
	IFC(toast->add_Dismissed(eventHandler.Get(), &dismissedToken));
	IFC(toast->add_Failed(eventHandler.Get(), &failedToken));

	IFC(notifier->Show(toast.Get()));


Cleanup:
	return hr;
}

HRESULT DesktopToastsApp::SetImageSrc(
	_In_z_ wchar_t * imagePath,   //_z_ null terminated parameter
	_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *toastXml
	)
{
	HRESULT hr = S_OK;

	wchar_t imageSrc[MAX_PATH] = L"file:///";
	ComPtr<IXmlNodeList> nodeList;
	ComPtr<IXmlNode> imageNode;
	ComPtr<IXmlNamedNodeMap> attributes;
	ComPtr<IXmlNode> srcAttribute;

	IFC(StringCchCatW(imageSrc, ARRAY_SIZE(imageSrc), imagePath));  //strsafe.h functions.
	IFC(toastXml->GetElementsByTagName(StringReferenceWrapper(L"image").Get(), nodeList.ReleaseAndGetAddressOf()));
	IFC(nodeList->Item(0, imageNode.ReleaseAndGetAddressOf()));
	IFC(imageNode->get_Attributes(attributes.ReleaseAndGetAddressOf()));
	IFC(attributes->GetNamedItem(StringReferenceWrapper(L"src").Get(), srcAttribute.ReleaseAndGetAddressOf()));
	
	IFC(SetNodeValueString(StringReferenceWrapper(imageSrc).Get(), srcAttribute.Get(), toastXml));

Cleanup:
	return hr;
}

HRESULT DesktopToastsApp::SetTextValues(
	_In_reads_(textValuesCount) wchar_t ** textValues,   //2 dimensional array of strings
	_In_ UINT32 textValuesCount,
	_In_reads_(textValuesCount) UINT32 *textValuesLengths, // 1 dimensional array of string lengths
	_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument * toastXml
	)
{
	HRESULT hr = S_OK;

	ComPtr<IXmlNodeList> nodeList;
	ComPtr<IXmlNode> textNode;
	UINT32 nodeListLength;

	IFC((textValues != nullptr && textValuesCount > 0) ? S_OK : E_INVALIDARG);

	IFC(toastXml->GetElementsByTagName(StringReferenceWrapper(L"text").Get(), nodeList.ReleaseAndGetAddressOf()));

	IFC(nodeList->get_Length(&nodeListLength));
	IFC((textValuesCount <= nodeListLength) ? S_OK : E_INVALIDARG);

	for (UINT32 i = 0; i < textValuesCount; ++i)
	{
		IFC(nodeList->Item(i, textNode.ReleaseAndGetAddressOf()));
		IFC(SetNodeValueString(StringReferenceWrapper(textValues[i], textValuesLengths[i]).Get(), textNode.Get(), toastXml));
	}

Cleanup:
	return hr;
}

HRESULT DesktopToastsApp::SetNodeValueString(
	_In_ HSTRING outputString,
	_In_ ABI::Windows::Data::Xml::Dom::IXmlNode * node,
	_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument * xml
	)
{
	HRESULT hr = S_OK;

	ComPtr<IXmlText> inputText;
	ComPtr<IXmlNode> inputTextNode;
	ComPtr<IXmlNode> appendedNode;

	IFC(xml->CreateTextNode(outputString, inputText.ReleaseAndGetAddressOf()));
	IFC(inputText.As(&inputTextNode));

	IFC(node->AppendChild(inputTextNode.Get(), appendedNode.ReleaseAndGetAddressOf()));

Cleanup:
	return hr;
}
