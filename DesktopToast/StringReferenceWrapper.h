#pragma once

class StringReferenceWrapper
{
public:
	// Constructor which takes an existing string buffer and its length as the parameters.
	// It fills an HSTRING_HEADER struct with the parameter.      
	// Warning: The caller must ensure the lifetime of the buffer outlives this      
	// object as it does not make a copy of the wide string memory.   
	 StringReferenceWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) throw ();


	 //templated methods must be defined/inlined in header file.
	 //the compiler needs to have access to the implementation of the methods, to instantiate them with the template argument (in this case int). If these implementations were not in the header, they wouldn't be accessible, and therefore the compiler wouldn't be able to instantiate the template.
	 template<size_t N>
	 StringReferenceWrapper(_In_reads_(N) wchar_t const(&stringRef)[N]) throw ()
	 {
		 HRESULT hr = WindowsCreateStringReference(stringRef, N - 1, &_header, &_hstring);

		 if (FAILED(hr))
		 {
			 RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
		 }
	 }

	 template<size_t _>
	 StringReferenceWrapper(_In_reads_(_) wchar_t(&stringRef)[_]) throw ()  //difference between this and previous is const keyword
	 {
		 UINT32 length;

		 HRESULT hr = SIZETToUInt32(wcslen(stringRef), &length);
		 if (FAILED(hr))
		 {
			 RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
		 }

		 hr = WindowsCreateStringReference(stringRef, length, &_header, &_hstring);

		 if (FAILED(hr))
		 {
			 RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, nullptr);
		 }
	 }

	 HSTRING Get() const throw ()  //const after a function declaration means that the function is not allowed to change any class members (except ones that are marked mutable). 
	 {
		return _hstring;    //defining a function inside header file makes it an inline function and so no multiple defintion linker errors.
	 }

	~StringReferenceWrapper();

	HSTRING _hstring;
	HSTRING_HEADER _header;

};
 
// another way to include function defintion inside header file is to explicitly use 'inline' modifier so that the function
//gets inlined and there are no multiple defintion linker errors lnk 2005. The other way is to define the 
// method inside the declaration itself like the Get() method above.
inline StringReferenceWrapper::~StringReferenceWrapper()
{
	WindowsDeleteString(_hstring);
}