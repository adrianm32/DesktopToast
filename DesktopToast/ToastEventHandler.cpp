#include "stdafx.h"

using namespace ABI::Windows::UI::Notifications;  //this way dont have to keep using FQN everytime

ToastEventHandler::ToastEventHandler(_In_ HWND hToActivate, _In_ HWND hEdit) : 
_ref(1),   //constructor initialization lists are more performant than  initializing inside ctor body
_hToActivate(hToActivate),  // as it saves  initialization to default value/null and then resetting to new value
_hEdit(hEdit)
{
}


// DesktopToastActivatedEventHandler 
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ IInspectable * args)
{
	BOOL succeeded = SetForegroundWindow(_hToActivate);

	if (succeeded)
	{
		LRESULT result = SendMessage(_hEdit, WM_SETTEXT, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(L"The User clicked on the toast."));
		succeeded |= result ? TRUE : FALSE;
	}

	return succeeded ? S_OK : E_FAIL;
}

// DesktopToastDismissedEventHandler
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e)
{
	ToastDismissalReason tdr;
	HRESULT hr = e->get_Reason(&tdr);

	if (SUCCEEDED(hr))
	{
		wchar_t * message;
		switch (tdr)
		{
		case ABI::Windows::UI::Notifications::ToastDismissalReason_UserCanceled:
			message = L"The user dismissed the toast";
			break;
		case ABI::Windows::UI::Notifications::ToastDismissalReason_ApplicationHidden:
			message = L"The application hid the toast";
			break;
		case ABI::Windows::UI::Notifications::ToastDismissalReason_TimedOut:
			message = L"The toast timed out";
			break;
		default:
			message = L"The toast did not activate";
			break;
		}

		LRESULT result = SendMessage(_hEdit, WM_SETTEXT, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(message));
		hr |= result ? S_OK : E_FAIL;
	}

	return hr;
}

// DesktopToastFailedEventHandler
IFACEMETHODIMP ToastEventHandler::Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e)
{
	LRESULT result = SendMessage(_hEdit, WM_SETTEXT, reinterpret_cast<WPARAM>(nullptr), reinterpret_cast<LPARAM>(L"Toast encountered an error"));
	return result ? S_OK : E_FAIL;
}

