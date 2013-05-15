#include "dp/dp.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"
#include "dp/display/displaykey.h"

#include <cstdio>

int dpMain(
    dp::Args &
)
{
    //TODO

    dp::DisplayManagerInfo  displayManagerInfo;
    displayManagerInfo.setConnectEventHandler(
        [](
            dp::DisplayManager &        _manager
            , const dp::DisplayKey &    _KEY
        )
        {
            //TODO

            std::printf( "display connected\n" );
        }
    );
    displayManagerInfo.setDisconnectEventHandler(
        [](
            dp::DisplayManager &        _manager
            , const dp::DisplayKey &    _KEY
        )
        {
            //TODO

            std::printf( "display disconnected\n" );
        }
    );

    dp::DisplayManager  displayManager( displayManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();

    return 0;
}
