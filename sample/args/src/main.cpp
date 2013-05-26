#include "dp/common/cli.h"
#include "dp/common/string.h"

#include <cstdio>

int dpMain(
    dp::Args &  _args
)
{
    dp::String  str;
    for( const auto & ARG : _args ) {
        dp::toString(
            str
            , ARG
        );

        std::printf( "%s\n", str.c_str() );
    }

    return 0;
}
