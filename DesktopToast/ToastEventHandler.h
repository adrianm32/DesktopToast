#pragma once

typedef ABI::Windows::Foundation::ITypedEventHandler < ABI::Windows::UI::Notifications::ToastNotification *, IInspectable *> DesktopToastActivatedEventHandler;
typedef ABI::Windows::Foundation::ITypedEventHandler < ABI::Windows::UI::Notifications::ToastNotification *, ABI::Windows::UI::Notifications::ToastDismissedEventArgs *> DesktopToastDismissedEventHandler;
typedef ABI::Windows::Foundation::ITypedEventHandler < ABI::Windows::UI::Notifications::ToastNotification *, ABI::Windows::UI::Notifications::ToastFailedEventArgs *> DesktopToastFailedEventHandler;


class ToastEventHandler :
	public Microsoft::WRL::Implements<DesktopToastActivatedEventHandler, DesktopToastDismissedEventHandler, DesktopToastFailedEventHandler>
{
public:
	ToastEventHandler(_In_ HWND hToActivate, _In_ HWND hEdit);
	~ToastEventHandler() {};

	// DesktopToastActivatedEventHandler 
	IFACEMETHODIMP Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ IInspectable * args);

	// DesktopToastDismissedEventHandler
	IFACEMETHODIMP Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ ABI::Windows::UI::Notifications::IToastDismissedEventArgs *e);

	// DesktopToastFailedEventHandler
	IFACEMETHODIMP Invoke(_In_ ABI::Windows::UI::Notifications::IToastNotification *sender, _In_ ABI::Windows::UI::Notifications::IToastFailedEventArgs *e);

	//IUnknown
	IFACEMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&_ref); };

	IFACEMETHODIMP_(ULONG) Release()
	{
		ULONG ref = InterlockedDecrement(&_ref);

		if (ref == 0) delete this;
		return ref;
	};

	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _COM_Outptr_ void **ppv)
	{
		HRESULT hr = S_OK;

		if (IsEqualIID(riid, IID_IUnknown))
		{
			*ppv = static_cast<IUnknown*>(static_cast<DesktopToastActivatedEventHandler*>(this));
		}
		else if (IsEqualIID(riid, __uuidof(DesktopToastActivatedEventHandler)))
		{
			*ppv = static_cast<DesktopToastActivatedEventHandler*>(this);
		}
		else if (IsEqualIID(riid, __uuidof(DesktopToastDismissedEventHandler)))
		{
			*ppv = static_cast<DesktopToastDismissedEventHandler*>(this);
		}
		else if (IsEqualIID(riid, __uuidof(DesktopToastFailedEventHandler)))
		{
			*ppv = static_cast<DesktopToastFailedEventHandler*>(this);
		}
		else
		{
			*ppv = nullptr;
		}

		if (*ppv)
		{
			reinterpret_cast<IUnknown*>(*ppv)->AddRef();
			hr = S_OK;
		}
		else
		{
			hr = E_NOINTERFACE;
		}

		return hr;
	}


private:
	ULONG _ref;
	HWND _hToActivate;
	HWND _hEdit;

};