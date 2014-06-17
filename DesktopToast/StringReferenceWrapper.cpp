#include "stdafx.h"
#include "StringReferenceWrapper.h"

StringReferenceWrapper::StringReferenceWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) throw()
{
	HRESULT hr = WindowsCreateStringReference(stringRef, length, &_header, &_hstring);

	if (FAILED(hr))
	{
		RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
	}
}


