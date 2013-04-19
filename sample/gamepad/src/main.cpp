#include "dp/dp.h"
#include "dp/input/gamepadmanager.h"
#include "dp/input/gamepadmanagerinfo.h"
#include "dp/input/gamepadkey.h"
#include "dp/input/gamepad.h"
#include "dp/input/gamepadinfo.h"
#include "dp/common/string.h"

#include <map>
#include <cstdio>

typedef std::map< const dp::GamePadKey, dp::GamePad * > GamePads;

void manageGamePads(
    GamePads &  _gamePads
)
{
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
            &_gamePads
            , &gamePadInfo
        ]
        (
            dp::GamePadManager &        _manager
            , const dp::GamePadKey &    _KEY
        )
        {
            if( _gamePads.find( _KEY ) == _gamePads.end() ) {
                return;
            }

            auto    gamePad = new dp::GamePad(
                _KEY
                , gamePadInfo
            );

            _gamePads.insert(
                GamePads::value_type(
                    _KEY
                    , gamePad
                )
            );

            dp::String  name(
                gamePad->getName()
            );

            std::printf( "name : %s\n", name.c_str() );
            std::printf( "buttons : %u\n", gamePad->getButtons() );
            std::printf( "axes : %u\n", gamePad->getAxes() );
        }
    );
    gamePadManagerInfo.setDisconnectEventHandler(
        [
            &_gamePads
        ]
        (
            dp::GamePadManager &        _manager
            , const dp::GamePadKey &    _KEY
        )
        {
            auto    it = _gamePads.find( _KEY );
            if( it == _gamePads.end() ) {
                return;
            }

            delete it->second;

            _gamePads.erase( it );

            std::printf( "gamepad disconnect\n" );
        }
    );

    dp::GamePadManager  gamePadManager( gamePadManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();
}

int dpMain(
    dp::Args &
)
{
    GamePads    gamePads;

    manageGamePads(
        gamePads
    );

    for( auto & pair : gamePads ) {
        delete pair.second;
    }

    return 0;
}
