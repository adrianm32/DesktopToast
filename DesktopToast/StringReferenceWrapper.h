class StringReferenceWrapper
{
public:
	// Constructor which takes an existing string buffer and its length as the parameters.
	// It fills an HSTRING_HEADER struct with the parameter.      
	// Warning: The caller must ensure the lifetime of the buffer outlives this      
	// object as it does not make a copy of the wide string memory.   
	 StringReferenceWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) throw ();

	 template<size_t N>
	 StringReferenceWrapper(_In_reads_(N) wchar_t const(&stringRef)[N]) throw ();

	 template<size_t _>
	 StringReferenceWrapper(_In_reads_(_) wchar_t (&stringRef)[_]) throw ();  //difference between this and previous is const keyword

	 HSTRING Get() const throw ();  //const after a function declaration means that the function is not allowed to change any class members (except ones that are marked mutable). 

	~StringReferenceWrapper();

private:
	HSTRING _hstring;
	HSTRING_HEADER _header;

};

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
StringReferenceWrapper::StringReferenceWrapper(_In_reads_(_) wchar_t (&stringRef)[_]) throw ()
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

HSTRING StringReferenceWrapper::Get() const throw()
{
	return _hstring;
}

StringReferenceWrapper::~StringReferenceWrapper()
{
	WindowsDeleteString(_hstring);
}