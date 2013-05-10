#include "dp/dp.h"
#include "dp/input/gamepadmanager.h"
#include "dp/input/gamepadmanagerinfo.h"
#include "dp/input/gamepadkey.h"
#include "dp/input/gamepad.h"
#include "dp/input/gamepadinfo.h"
#include "dp/common/string.h"

#include <map>
#include <memory>
#include <utility>
#include <cstdio>

int dpMain(
    dp::Args &
)
{
    std::map<
        dp::GamePadKey
        , std::unique_ptr< dp::GamePad >
    > gamePads;

    dp::GamePadInfo gamePadInfo;
    gamePadInfo.setButtonEventHandler(
        [](
            dp::GamePad &   _gamePad
            , unsigned int  _index
            , bool          _pressed
        )
        {
            std::printf(
                "gamepad button[%u] %s\n"
                , _index
                , _pressed
                    ? "press"
                    : "release"
            );
        }
    );
    gamePadInfo.setAxisEventHandler(
        [](
            dp::GamePad &   _gamePad
            , unsigned int  _index
            , int           _value
        )
        {
            std::printf(
                "gamepad axis[%u] value : %d\n"
                , _index
                , _value
            );
        }
    );

    dp::GamePadManagerInfo  gamePadManagerInfo;
    gamePadManagerInfo.setConnectEventHandler(
        [
            &gamePads
            , &gamePadInfo
        ]
        (
            dp::GamePadManager &        _manager
            , const dp::GamePadKey &    _KEY
        )
        {
            if( gamePads.find( _KEY ) != gamePads.end() ) {
                return;
            }

            auto    gamePad = new dp::GamePad(
                _KEY
                , gamePadInfo
            );

            std::unique_ptr< dp::GamePad >  uniquePtr( gamePad );

            gamePads.insert(
                decltype( gamePads )::value_type(
                    _KEY
                    , std::move( uniquePtr )
                )
            );

            dp::String  name;
            dp::toString(
                name
                , gamePad->getName()
            );

            std::printf( "name : %s\n", name.c_str() );
            std::printf( "buttons : %u\n", gamePad->getButtons() );
            std::printf( "axes : %u\n", gamePad->getAxes() );
        }
    );
    gamePadManagerInfo.setDisconnectEventHandler(
        [
            &gamePads
        ]
        (
            dp::GamePadManager &        _manager
            , const dp::GamePadKey &    _KEY
        )
        {
            auto    it = gamePads.find( _KEY );
            if( it == gamePads.end() ) {
                return;
            }

            auto    uniquePtr = std::move( it->second );

            gamePads.erase( it );

            std::printf( "gamepad disconnect\n" );
        }
    );

    dp::GamePadManager  gamePadManager( gamePadManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();

    return 0;
}
