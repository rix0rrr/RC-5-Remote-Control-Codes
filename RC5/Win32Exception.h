#pragma once

#include "stdafx.h"

class Win32Exception : public std::runtime_error
{
public:
    Win32Exception() : std::runtime_error( ErrorMessage( &error_code ) )
    { };

    virtual ~Win32Exception() { };

    /// return the actual error code
    DWORD ErrorCode() const throw() { return error_code; };

private:
    DWORD error_code;

    static std::string ErrorMessage( DWORD* error_code )
    {
        *error_code = ::GetLastError();

        std::string error_messageA;
        wchar_t* error_messageW = NULL;
        DWORD len = ::FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM | 
                                      FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                      FORMAT_MESSAGE_IGNORE_INSERTS,
                                      NULL,
                                      *error_code,
                                      MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                                      reinterpret_cast< LPWSTR >( &error_messageW ),
                                      0,
                                      NULL );
        if( NULL != error_messageW )
        {
            // this may generate a C4244 warning. It is safe to ignore.
            std::copy( error_messageW, 
                       error_messageW + len, 
                       std::back_inserter( error_messageA ) );
            ::LocalFree( error_messageW );
        }
        return error_messageA;
    };

}; // class Win32Exception