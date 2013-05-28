﻿#include "dp/common/cli.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"
#include "dp/display/displaykey.h"
#include "dp/display/display.h"
#include "dp/display/displaymode.h"
#include "dp/display/displayconfig.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <cstdio>

typedef std::vector< dp::DisplayKey > Keys;
typedef std::unique_lock< std::mutex > Lock;

bool inputInt(
    int &   _result
)
{
    std::printf( "> " );

    char    buffer[ 100 ];

    std::fgets(
        buffer
        , sizeof( buffer )
        , stdin
    );

    char *  endPtr = nullptr;
    _result = strtol(
        buffer
        , &endPtr
        , 10
    );

    // 1文字以上入力され、入力データが全て変換されているなら正常な入力
    bool    inputed = buffer != endPtr && *endPtr == '\n';

    while( buffer[ std::strlen( buffer ) - 1 ] != '\n' ) {
        std::fgets(
            buffer
            , sizeof( buffer )
            , stdin
        );
    }

    return inputed;
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

void showDisplayesWithIndex(
    std::mutex &            _mutex
    , const Keys &          _KEYS
    , dp::DisplayManager &  _manager
)
{
    Lock    lock( _mutex );

    auto    index = 0;
    for( const auto & KEY : _KEYS ) {
        std::printf( "%d : ", index );
        index++;

        const auto  DISPLAY = _manager.getDisplay(
            KEY
        );

        showDisplay(
            DISPLAY
        );
    }
}

void showDisplayDetails(
    const dp::DisplayKey &  _KEY
    , dp::DisplayManager &  _manager
)
{
    const auto  DISPLAY = _manager.getDisplay(
        _KEY
    );

    const auto  MODE = _manager.getDisplayMode(
        DISPLAY.getModeKey()
    );

    std::printf(
        "size : %dx%d\n"
        , DISPLAY.getWidth()
        , DISPLAY.getHeight()
    );
    std::printf(
        "position : %dx%d\n"
        , DISPLAY.getX()
        , DISPLAY.getY()
    );
    std::printf( "mode :\n" );
    std::printf(
        "  size : %dx%d\n"
        , MODE.getWidth()
        , MODE.getHeight()
    );
}

void showDisplayDetailsMenu(
    std::mutex &            _mutex
    , const Keys &          _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "show display details\n" );
        showDisplayesWithIndex(
            _mutex
            , _KEYS
            , _manager
        );
        std::printf( "\n" );
        std::printf( "* : cancel\n" );

        int index;
        if( inputInt( index ) == false ) {
            return;
        }

        dp::DisplayKey  key;

        {
            Lock    lock( _mutex );

            if( index >= 0 && index < _KEYS.size() ) {
                key = _KEYS[ index ];
            } else {
                return;
            }
        }

        showDisplayDetails(
            key
            , _manager
        );
    }
}

void configDisplayInputX(
    dp::DisplayConfig & _config
)
{
    std::printf( "input x\n" );

    int x;

    if( inputInt( x ) ) {
        _config.setX( x );
    }
}

void configDisplayInputY(
    dp::DisplayConfig & _config
)
{
    std::printf( "input y\n" );

    int y;

    if( inputInt( y ) ) {
        _config.setY( y );
    }
}

void applyDisplayConfig(
    dp::DisplayConfig &         _config
    , const dp::DisplayKey &    _KEY
    , dp::DisplayManager &      _manager
)
{
    _manager.applyDisplayConfig(
        _KEY
        , _config
    );
}

void configDisplay(
    const dp::DisplayKey &  _KEY
    , dp::DisplayManager &  _manager
)
{
    const auto  DISPLAY = _manager.getDisplay(
        _KEY
    );

    dp::DisplayConfig   config(
        DISPLAY
    );

    while( 1 ) {
        std::printf(
            "config %dx%d+%d+%d\n"
            , DISPLAY.getWidth()
            , DISPLAY.getHeight()
            , DISPLAY.getX()
            , DISPLAY.getY()
        );
        std::printf( "1 : x = %d\n", config.getX() );
        std::printf( "2 : y = %d\n", config.getY() );
        std::printf( "\n" );
        std::printf( "0 : apply\n" );
        std::printf( "\n" );
        std::printf( "* : cancel\n" );

        int input;
        if( inputInt( input ) == false ) {
            return;
        }

        switch( input ) {
        case 1:
            configDisplayInputX(
                config
            );
            break;

        case 2:
            configDisplayInputY(
                config
            );
            break;

        case 0:
            applyDisplayConfig(
                config
                , _KEY
                , _manager
            );
            break;

        default:
            return;
            break;
        }
    }
}

void configDisplayMenu(
    std::mutex &            _mutex
    , const Keys &          _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "config display\n" );
        showDisplayesWithIndex(
            _mutex
            , _KEYS
            , _manager
        );
        std::printf( "\n" );
        std::printf( "* : cancel\n" );

        int index;
        if( inputInt( index ) == false ) {
            return;
        }

        dp::DisplayKey  key;

        {
            Lock    lock( _mutex );

            if( index >= 0 && index < _KEYS.size() ) {
                key = _KEYS[ index ];
            } else {
                return;
            }
        }

        configDisplay(
            key
            , _manager
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
        std::printf( "main menu\n" );
        std::printf( "0 : show displayes\n" );
        std::printf( "1 : show display details\n" );
        std::printf( "2 : config display\n" );
        std::printf( "\n" );
        std::printf( "* : quit\n" );

        int input;
        if( inputInt( input ) == false ) {
            return;
        }

        switch( input ) {
        case 0:
            showDisplayes(
                _mutex
                , _KEYS
                , _manager
            );
            break;

        case 1:
            showDisplayDetailsMenu(
                _mutex
                , _KEYS
                , _manager
            );
            break;

        case 2:
            configDisplayMenu(
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
