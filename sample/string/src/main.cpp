#include "dp/dp.h"
#include "dp/common/string.h"
#include "dp/common/utf8.h"
#include "dp/common/utf16.h"
#include "dp/common/utf32.h"

#include <cstdio>

const char  STRING[] = "STRING";
const char  UTF8[] = "UTF-8";
const char  UTF16[] = "UTF-16";
const char  UTF32[] = "UTF-32";

template< class STR_T >
void dump(
    const char *    _FROM
    , const STR_T & _STR
)
{
    std::printf( "    from %s\n", _FROM );

    std::printf( "      " );

    for( const auto c : _STR ) {
        std::printf( "0x%0*x ", sizeof( c ) * 2, c );
    }

    std::printf( "\n" );
}

template< class STR_T >
void to(
    const char *            _TO
    , const dp::String &    _STR
    , const dp::Utf8 &      _UTF8
    , const dp::Utf16 &     _UTF16
    , const dp::Utf32 &     _UTF32
)
{
    std::printf( "  to %s\n", _TO );

    STR_T   fromStr( _STR );
    dump(
        STRING
        , fromStr
    );

    STR_T   fromUtf8( _UTF8 );
    dump(
        UTF8
        , fromUtf8
    );

    STR_T   fromUtf16( _UTF16 );
    dump(
        UTF16
        , fromUtf16
    );

    STR_T   fromUtf32( _UTF32 );
    dump(
        UTF32
        , fromUtf32
    );
}

int dpMain(
    dp::Args &  _args
)
{
    for( const auto & ARG : _args ) {
        dp::String  str( ARG );
        dp::Utf8    utf8( ARG );
        dp::Utf16   utf16( ARG );
        dp::Utf32   utf32( ARG );

        std::printf( "%s : \"%s\"\n", STRING, str.c_str() );

        to< dp::String >(
            STRING
            , str
            , utf8
            , utf16
            , utf32
        );

        to< dp::Utf8 >(
            UTF8
            , str
            , utf8
            , utf16
            , utf32
        );

        to< dp::Utf16 >(
            UTF16
            , str
            , utf8
            , utf16
            , utf32
        );

        to< dp::Utf32 >(
            UTF32
            , str
            , utf8
            , utf16
            , utf32
        );
    }

    return 0;
}
