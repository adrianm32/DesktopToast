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

template<size_t N>
StringReferenceWrapper::StringReferenceWrapper(_In_reads_(N) wchar_t const(&stringRef)[N]) throw ()
{
	HRESULT hr = WindowsCreateStringReference(stringRef, N - 1, &_header, &_hstring);

	if (FAILED(hr))
	{
		RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
	}

}

template<size_t _>
StringReferenceWrapper::StringReferenceWrapper(_In_reads_(_) wchar_t(&stringRef)[_]) throw ()
{
	UINT32 length;

	HRESULT hr = SIZETToUInt32(wcslen(stringRef), &length);
	if (FAILED(hr))
	{
		RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
	}

	hr = WindowsCreateStringReference(stringRef, N - 1, &_header, &_hstring);

	if (FAILED(hr))
	{
		RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
	}

}

