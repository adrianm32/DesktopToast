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

