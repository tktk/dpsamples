#include "dp/common/cli.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"
#include "dp/display/displaykey.h"
#include "dp/display/display.h"
#include "dp/display/displaymode.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <cstdio>

typedef std::vector< dp::DisplayKey > Keys;
typedef std::unique_lock< std::mutex > Lock;

int inputInt(
)
{
    char    buffer[ 100 ];

    std::fgets(
        buffer
        , sizeof( buffer )
        , stdin
    );

    char *  endPtr = nullptr;
    auto    input = strtol(
        buffer
        , &endPtr
        , 10
    );
    if( buffer == endPtr || *endPtr != '\n' ) {
        input = -1;
    }

    while( buffer[ std::strlen( buffer ) - 1 ] != '\n' ) {
        std::fgets(
            buffer
            , sizeof( buffer )
            , stdin
        );
    }

    return input;
}

void showDisplay(
    const dp::Display & _DISPLAY
)
{
    std::printf(
        "%dx%d+%d+%d\n"
        , _DISPLAY.getWidth()
        , _DISPLAY.getHeight()
        , _DISPLAY.getX()
        , _DISPLAY.getY()
    );
}

void showDisplayes(
    std::mutex &            _mutex
    , const Keys &          _KEYS
    , dp::DisplayManager &  _manager
)
{
    Lock    lock( _mutex );

    for( const auto & KEY : _KEYS ) {
        const auto  DISPLAY = _manager.getDisplay(
            KEY
        );

        showDisplay(
            DISPLAY
        );
    }
}

void mainMenu(
    std::mutex &            _mutex
    , const Keys &          _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "0 : show displayes\n" );
        std::printf( "* : quit\n" );

        switch( inputInt() ) {
        case 0:
            showDisplayes(
                _mutex
                , _KEYS
                , _manager
            );
            break;

        default:
            return;
            break;
        }
    }
}

Keys::iterator findKey(
    const dp::DisplayKey &  _KEY
    , Keys &                _keys
)
{
    return std::find_if(
        _keys.begin()
        , _keys.end()
        , [
            &_KEY
        ]
        (
            const dp::DisplayKey &  _KEY2
        )
        {
            return _KEY == _KEY2;
        }
    );
}

void addKey(
    const dp::DisplayKey &  _KEY
    , Keys &                _keys
)
{
    if( findKey(
        _KEY
        , _keys
    ) != _keys.end() ) {
        return;
    }

    _keys.push_back( _KEY );
}

void removeKey(
    const dp::DisplayKey &  _KEY
    , Keys &                _keys
)
{
    auto    it = findKey(
        _KEY
        , _keys
    );
    if( it == _keys.end() ) {
        return;
    }

    _keys.erase( it );
}

int dpMain(
    dp::Args &
)
{
    std::mutex  mutex;
    Keys        keys;

    dp::DisplayManagerInfo  displayManagerInfo;
    displayManagerInfo.setConnectEventHandler(
        [
            &mutex
            , &keys
        ]
        (
            dp::DisplayManager &        _manager
            , const dp::DisplayKey &    _KEY
            , bool                      _connected
        )
        {
            Lock    lock( mutex );

            if( _connected ) {
                addKey(
                    _KEY
                    , keys
                );
            } else {
                removeKey(
                    _KEY
                    , keys
                );
            }
        }
    );

    dp::DisplayManager  displayManager( displayManagerInfo );

    mainMenu(
        mutex
        , keys
        , displayManager
    );

    return 0;
}
