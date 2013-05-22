#include "dp/dp.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"
#include "dp/display/displaykey.h"
#include "dp/display/display.h"
#include "dp/display/displaymode.h"

#include <cstdio>

int dpMain(
    dp::Args &
)
{
    dp::DisplayManagerInfo  displayManagerInfo;
    displayManagerInfo.setConnectEventHandler(
        [](
            dp::DisplayManager &        _manager
            , const dp::DisplayKey &    _KEY
        )
        {
            const auto  DISPLAY = _manager.getDisplay(
                _KEY
            );

            const auto  DISPLAY_MODE = _manager.getDisplayMode(
                DISPLAY.getModeKey()
            );

            std::printf( "display connected\n" );
            std::printf(
                "  position : %dx%d\n"
                , DISPLAY.getX()
                , DISPLAY.getY()
            );
            std::printf(
                "  size : %dx%d\n"
                , DISPLAY.getWidth()
                , DISPLAY.getHeight()
            );
            std::printf(
                "  mode :\n"
            );
            std::printf(
                "    size : %dx%d\n"
                , DISPLAY_MODE.getWidth()
                , DISPLAY_MODE.getHeight()
            );
        }
    );
    displayManagerInfo.setDisconnectEventHandler(
        [](
            dp::DisplayManager &        _manager
            , const dp::DisplayKey &    _KEY
        )
        {
            std::printf( "display disconnected\n" );
        }
    );

    dp::DisplayManager  displayManager( displayManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();

    return 0;
}
