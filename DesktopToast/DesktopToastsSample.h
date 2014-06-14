#pragma once

#define LINE_LENGTH 15 //How many characters we'll allot for each line of a toast

using namespace ABI::Windows::UI::Notifications;

#ifndef HINST_THISCOMPONENT
/*
extern "C" makes a function-name in C++ have 'C' linkage (compiler does not mangle the name) so that client C code can link to (i.e use) your function using a 'C' compatible header file that contains just the declaration of your function. Your function definition is contained in a binary format (that was compiled by your C++ compiler) that the client 'C' linker will then link to using the 'C' name.
Since C++ has overloading of function names and C does not, the C++ compiler cannot just use the function name as a unique id to link to, so it mangles the name by adding information about the arguments. A C compiler does not need to mangle the name since you can not overload function names in C. When you state that a function has extern "C" linkage in C++, the C++ compiler does not add argument/parameter type information to the name used for linkage.
*/
/*
as the extern extends the visibility to the whole program, by externing a variable we can use the variables anywhere in the program provided we know the declaration of them and the variable is defined somewhere.
extern only declares , it does not allocate memory. The variable must be defined somewhere in order to allocate memory and for it to be used.
*/
EXTERN_C IMAGE_DOS_HEADER __imageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__imageBase)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef HM_TEXTBUTTON
#define HM_TEXTBUTTON 1
#endif

class DesktopToastsApp
{
public:
	DesktopToastsApp();
	~DesktopToastsApp();
	HRESULT Initialize();
	void RunMessageLoop();

private:
	HRESULT TryCreateShortcut();
	HRESULT InstallShortcut(_In_ wchar_t * shortcutPath);

	static LRESULT CALLBACK WndProc(    //callback and winapi are both macros for __stdcall which is callee clears the stack. __cdecl is caller clears the stack. The __fastcall calling convention specifies that arguments to functions are to be passed in registers, when possible.
		_In_ HWND hWnd,
		_In_ UINT message,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
		);

	HRESULT TextButtonClicked();
	HRESULT DisplayToast();
	HRESULT CreateToastXml(
		_In_ IToastNotificationManagerStatics * toastManager,
		_Outptr_ IXMLDocument **xml
		);
	HRESULT CreateToast(
		_In_ IToastNotificationManagerStatics * toastManager,
		_In_ IXMLDocument *xml
		);

	HRESULT SetImageSrc(
		_In_z_ wchar_t * imagePath,   //_z_ null terminated parameter
		_In_ IXMLDocument *xml
		);

	HRESULT SetTextValues(
		_In_reads_(textValuesCount) wchar_t ** textValues,   //2 dimensional array of strings
		_In_ UINT32 textValuesCount,
		_In_reads_(textValuesCount) UINT32 *textValuesLengths, // 1 dimensional array of string lengths
		_In_ IXMLDocument * toastXml
		);

	HRESULT SetNodeValueString(
		_In_ HSTRING outputString,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlNode * node,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument * xml
		);

	HWND _hWnd;
	HWND _hEdit;

};
