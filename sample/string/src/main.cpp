#include "dp/dp.h"
#include "dp/common/string.h"

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

void toString(
    const dp::Utf8 &    _UTF8
    , const dp::Utf16 & _UTF16
    , const dp::Utf32 & _UTF32
)
{
    std::printf( "  to %s\n", STRING );

    dp::String  fromUtf8;
    dp::toString(
        fromUtf8
        , _UTF8
    );
    dump(
        UTF8
        , fromUtf8
    );

    dp::String  fromUtf16;
    dp::toString(
        fromUtf16
        , _UTF16
    );
    dump(
        UTF16
        , fromUtf16
    );

    dp::String  fromUtf32;
    dp::toString(
        fromUtf32
        , _UTF32
    );
    dump(
        UTF32
        , fromUtf32
    );
}

void toUtf8(
    const dp::String &  _STRING
    , const dp::Utf16 & _UTF16
    , const dp::Utf32 & _UTF32
)
{
    std::printf( "  to %s\n", UTF8 );

    dp::Utf8    fromString;
    dp::toUtf8(
        fromString
        , _STRING
    );
    dump(
        STRING
        , fromString
    );

    dp::Utf8    fromUtf16;
    dp::toUtf8(
        fromUtf16
        , _UTF16
    );
    dump(
        UTF16
        , fromUtf16
    );

    dp::Utf8    fromUtf32;
    dp::toUtf8(
        fromUtf32
        , _UTF32
    );
    dump(
        UTF32
        , fromUtf32
    );
}

void toUtf16(
    const dp::String &  _STRING
    , const dp::Utf8 &  _UTF8
    , const dp::Utf32 & _UTF32
)
{
    std::printf( "  to %s\n", UTF16 );

    dp::Utf16   fromString;
    dp::toUtf16(
        fromString
        , _STRING
    );
    dump(
        STRING
        , fromString
    );

    dp::Utf16   fromUtf8;
    dp::toUtf16(
        fromUtf8
        , _UTF8
    );
    dump(
        UTF8
        , fromUtf8
    );

    dp::Utf16   fromUtf32;
    dp::toUtf16(
        fromUtf32
        , _UTF32
    );
    dump(
        UTF32
        , fromUtf32
    );
}

void toUtf32(
    const dp::String &  _STRING
    , const dp::Utf8 &  _UTF8
    , const dp::Utf16 & _UTF16
)
{
    std::printf( "  to %s\n", UTF32 );

    dp::Utf32   fromString;
    dp::toUtf32(
        fromString
        , _STRING
    );
    dump(
        STRING
        , fromString
    );

    dp::Utf32   fromUtf8;
    dp::toUtf32(
        fromUtf8
        , _UTF8
    );
    dump(
        UTF8
        , fromUtf8
    );

    dp::Utf32   fromUtf16;
    dp::toUtf32(
        fromUtf16
        , _UTF16
    );
    dump(
        UTF16
        , fromUtf16
    );
}

int dpMain(
    dp::Args &  _args
)
{
    for( const auto & ARG : _args ) {
        dp::String  str( ARG );

        dp::Utf8    utf8;
        dp::toUtf8(
            utf8
            , str
        );

        dp::Utf16   utf16;
        dp::toUtf16(
            utf16
            , str
        );

        dp::Utf32   utf32;
        dp::toUtf32(
            utf32
            , str
        );

        std::printf( "%s : \"%s\"\n", STRING, str.c_str() );

        toString(
            utf8
            , utf16
            , utf32
        );

        toUtf8(
            str
            , utf16
            , utf32
        );

        toUtf16(
            str
            , utf8
            , utf32
        );

        toUtf32(
            str
            , utf8
            , utf16
        );
    }

    return 0;
}
