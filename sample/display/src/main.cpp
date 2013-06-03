#include "dp/common/cli.h"
#include "dp/display/displaymanager.h"
#include "dp/display/displaymanagerinfo.h"
#include "dp/display/displaykey.h"
#include "dp/display/display.h"
#include "dp/display/displaymode.h"
#include "dp/display/displayconfig.h"
#include "dp/display/displayrotate.h"
#include "dp/display/displaymanagerexception.h"
#include "dp/util/exception.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <exception>

typedef std::vector< dp::DisplayKey > DisplayKeys;
typedef std::vector< dp::DisplayModeKey > DisplayModeKeys;
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

void showDisplaysImpl(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    Lock    lock( _mutex );

    for( const auto & KEY : _KEYS ) {
        try {
            const auto  DISPLAY = _manager.getDisplay(
                KEY
            );

            showDisplay(
                DISPLAY
            );
        } catch( const dp::DisplayManagerException & ) {
            std::printf( "missing display\n" );

            throw;
        }
    }
}

void showDisplays(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    try {
        showDisplaysImpl(
            _mutex
            , _KEYS
            , _manager
        );
    } catch( const dp::DisplayManagerException & ) {}
}

void showDisplaysWithIndex(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    Lock    lock( _mutex );

    auto    index = 0;
    for( const auto & KEY : _KEYS ) {
        std::printf( "%d : ", index );
        index++;

        try {
            const auto  DISPLAY = _manager.getDisplay(
                KEY
            );

            showDisplay(
                DISPLAY
            );
        } catch( const dp::DisplayManagerException & ) {
            std::printf( "missing display\n" );

            throw;
        }
    }
}

void showDisplayDetails(
    const dp::DisplayKey &  _KEY
    , dp::DisplayManager &  _manager
)
{
    dp::Display display;
    try {
        display = _manager.getDisplay(
            _KEY
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "missing display\n" );

        throw;
    }

    std::printf(
        "size : %dx%d\n"
        , display.getWidth()
        , display.getHeight()
    );
    std::printf(
        "position : %dx%d\n"
        , display.getX()
        , display.getY()
    );

    dp::DisplayMode mode;
    try {
        mode = _manager.getDisplayMode(
            display.getModeKey()
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "missing display mode\n" );

        throw;
    }

    std::printf( "mode :\n" );
    std::printf(
        "  size : %dx%d\n"
        , mode.getWidth()
        , mode.getHeight()
    );
}

void showDisplayDetailsMenuImpl(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "show display details\n" );
        showDisplaysWithIndex(
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

void showDisplayDetailsMenu(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    try {
        showDisplayDetailsMenuImpl(
            _mutex
            , _KEYS
            , _manager
        );
    } catch( const dp::DisplayManagerException & ) {}
}

void showDisplayMode(
    const dp::DisplayMode & _MODE
)
{
    std::printf(
        "%dx%d %.1fHz\n"
        , _MODE.getWidth()
        , _MODE.getHeight()
        , _MODE.getRefreshRate()
    );
}

void showDisplayModesWithIndex(
    const DisplayModeKeys & _KEYS
    , dp::DisplayManager &  _manager
)
{
    auto    index = 0;
    for( const auto & KEY : _KEYS ) {
        std::printf( "%d : ", index );
        index++;

        try {
            const auto  MODE = _manager.getDisplayMode(
                KEY
            );

            showDisplayMode(
                MODE
            );
        } catch( const dp::DisplayManagerException & ) {
            std::printf( "missing display mode\n" );

            throw;
        }
    }
}

const auto  ROTATE_NORMAL = "normal";
const auto  ROTATE_RIGHT = "right";
const auto  ROTATE_INVERTED = "inverted";
const auto  ROTATE_LEFT = "left";

const char * getRotateStr(
    dp::DisplayRotate   _rotate
)
{
    switch( _rotate ) {
    case dp::DisplayRotate::NORMAL:
        return ROTATE_NORMAL;
        break;

    case dp::DisplayRotate::RIGHT:
        return ROTATE_RIGHT;
        break;

    case dp::DisplayRotate::INVERTED:
        return ROTATE_INVERTED;
        break;

    case dp::DisplayRotate::LEFT:
        return ROTATE_LEFT;
        break;
    }

    DPTHROW( std::exception() )
}

void configDisplayInputX(
    dp::DisplayConfig & _config
)
{
    std::printf( "input x\n" );

    int x;

    if( inputInt( x ) == false ) {
        return;
    }

    _config.setX( x );
}

void configDisplayInputY(
    dp::DisplayConfig & _config
)
{
    std::printf( "input y\n" );

    int y;

    if( inputInt( y ) == false ) {
        return;
    }

    _config.setY( y );
}

void configDisplayInputRotate(
    dp::DisplayConfig & _config
)
{
    std::printf( "input rotate\n" );
    std::printf( "0 : normal\n" );
    std::printf( "1 : right\n" );
    std::printf( "2 : inverted\n" );
    std::printf( "3 : left\n" );
    std::printf( "\n" );
    std::printf( "* : cancel\n" );

    int rotateInt;
    if( inputInt( rotateInt ) == false ) {
        return;
    }

    dp::DisplayRotate   rotate;
    switch( rotateInt ) {
    case 0:
        rotate = dp::DisplayRotate::NORMAL;
        break;

    case 1:
        rotate = dp::DisplayRotate::RIGHT;
        break;

    case 2:
        rotate = dp::DisplayRotate::INVERTED;
        break;

    case 3:
        rotate = dp::DisplayRotate::LEFT;
        break;

    default:
        return;
        break;
    }

    _config.setRotate( rotate );
}

void configDisplayInputMode(
    dp::DisplayConfig &         _config
    , const dp::DisplayKey &    _KEY
    , dp::DisplayManager &      _manager
    , dp::DisplayMode &         _mode
)
{
    DisplayModeKeys modeKeys;
    try {
        modeKeys = _manager.getDisplayModeKeys(
            _KEY
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "missing display\n" );

        throw;
    }

    std::printf( "input mode\n" );
    showDisplayModesWithIndex(
        modeKeys
        , _manager
    );
    std::printf( "\n" );
    std::printf( "* : cancel\n" );

    int index;
    if( inputInt( index ) == false ) {
        return;
    }

    if( index >= 0 && index < modeKeys.size() ) {
        const auto &    MODE_KEY = modeKeys[ index ];

        _config.setModeKey(
            MODE_KEY
        );

        try {
            _mode = _manager.getDisplayMode(
                MODE_KEY
            );
        } catch( const dp::DisplayManagerException & ) {
            std::printf( "missing display mode\n" );

            throw;
        }
    }
}

void applyDisplayConfig(
    dp::DisplayConfig &         _config
    , const dp::DisplayKey &    _KEY
    , dp::DisplayManager &      _manager
    , dp::Display &             _display
)
{
    try {
        _manager.applyDisplayConfig(
            _KEY
            , _config
        );

        _display = _manager.getDisplay(
            _KEY
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "failed config display\n" );

        return;
    }
}

void configDisplay(
    const dp::DisplayKey &  _KEY
    , dp::DisplayManager &  _manager
)
{
    dp::Display display;
    try {
        display = _manager.getDisplay(
            _KEY
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "missing display\n" );

        throw;
    }

    dp::DisplayConfig   config(
        display
    );

    dp::DisplayMode mode;
    try {
        mode = _manager.getDisplayMode(
            config.getModeKey()
        );
    } catch( const dp::DisplayManagerException & ) {
        std::printf( "missing display mode\n" );

        throw;
    }

    while( 1 ) {
        std::printf(
            "config %dx%d+%d+%d\n"
            , display.getWidth()
            , display.getHeight()
            , display.getX()
            , display.getY()
        );
        std::printf( "1 : x = %d\n", config.getX() );
        std::printf( "2 : y = %d\n", config.getY() );
        std::printf( "3 : rotate = %s\n", getRotateStr( config.getRotate() ) );
        std::printf( "4 : mode = " );
        showDisplayMode(
            mode
        );
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

        case 3:
            configDisplayInputRotate(
                config
            );
            break;

        case 4:
            configDisplayInputMode(
                config
                , _KEY
                , _manager
                , mode
            );
            break;

        case 0:
            applyDisplayConfig(
                config
                , _KEY
                , _manager
                , display
            );
            break;

        default:
            return;
            break;
        }
    }
}

void configDisplayMenuImpl(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "config display\n" );
        showDisplaysWithIndex(
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

void configDisplayMenu(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    try {
        configDisplayMenuImpl(
            _mutex
            , _KEYS
            , _manager
        );
    } catch( const dp::DisplayManagerException & ) {}
}

void mainMenu(
    std::mutex &            _mutex
    , const DisplayKeys &   _KEYS
    , dp::DisplayManager &  _manager
)
{
    while( 1 ) {
        std::printf( "main menu\n" );
        std::printf( "0 : show displays\n" );
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
            showDisplays(
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

DisplayKeys::iterator findDisplayKey(
    const dp::DisplayKey &  _KEY
    , DisplayKeys &         _keys
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

void addDisplayKey(
    const dp::DisplayKey &  _KEY
    , DisplayKeys &         _keys
)
{
    if( findDisplayKey(
        _KEY
        , _keys
    ) != _keys.end() ) {
        return;
    }

    _keys.push_back( _KEY );
}

void removeDisplayKey(
    const dp::DisplayKey &  _KEY
    , DisplayKeys &         _keys
)
{
    auto    it = findDisplayKey(
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
    DisplayKeys keys;

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
                addDisplayKey(
                    _KEY
                    , keys
                );
            } else {
                removeDisplayKey(
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
