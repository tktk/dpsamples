#include "dp/dp.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"

#include <cstdio>

int dpMain(
    dp::Args &
)
{
    dp::DisplayManagerInfo  displayManagerInfo;

    //TODO

    dp::DisplayManager  displayManager( displayManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();

    return 0;
}
