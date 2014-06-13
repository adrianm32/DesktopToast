#pragma once

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