#include "dp/common/cli.h"
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

typedef std::unique_ptr< dp::GamePad > GamePadUniquePtr;

typedef std::map<
    dp::GamePadKey
    , GamePadUniquePtr
> GamePads;

void connectGamePad(
    const dp::GamePadKey &      _KEY
    , const dp::GamePadInfo &   _INFO
    , GamePads &                _gamePads
)
{
    if( _gamePads.find( _KEY ) != _gamePads.end() ) {
        return;
    }

    GamePadUniquePtr    uniquePtr(
        new dp::GamePad(
            _KEY
            , _INFO
        )
    );
    const auto &    GAME_PAD = *uniquePtr;

    _gamePads.insert(
        GamePads::value_type(
            _KEY
            , std::move( uniquePtr )
        )
    );

    dp::String  name;
    dp::toString(
        name
        , GAME_PAD.getName()
    );

    std::printf( "gamepad connected\n" );
    std::printf( "  name : %s\n", name.c_str() );
    std::printf( "  buttons : %u\n", GAME_PAD.getButtons() );
    std::printf( "  axes : %u\n", GAME_PAD.getAxes() );
}

void disconnectGamePad(
    const dp::GamePadKey &  _KEY
    , GamePads &            _gamePads
)
{
    auto    it = _gamePads.find( _KEY );
    if( it == _gamePads.end() ) {
        return;
    }

    auto    uniquePtr = std::move( it->second );

    _gamePads.erase( it );

    std::printf( "gamepad disconnected\n" );
}

int dpMain(
    dp::Args &
)
{
    GamePads    gamePads;

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
            , bool                      _connected
        )
        {
            if( _connected ) {
                connectGamePad(
                    _KEY
                    , gamePadInfo
                    , gamePads
                );
            } else {
                disconnectGamePad(
                    _KEY
                    , gamePads
                );
            }
        }
    );

    dp::GamePadManager  gamePadManager( gamePadManagerInfo );

    std::printf( "Press ENTER to quit\n" );

    std::getchar();

    return 0;
}
