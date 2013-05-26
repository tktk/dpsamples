#include "dp/common/cli.h"

#include <cstdio>

int dpMain(
    dp::Args &  _args
)
{
    for( const auto & ARG : _args ) {
        std::printf( "%s\n", ARG.c_str() );
    }

    return 0;
}
