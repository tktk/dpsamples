# -*- coding: utf-8 -*-

from . import args, string, gamepad, display

def build( _ctx ):
    args.build( _ctx )
    string.build( _ctx )
    gamepad.build( _ctx )
    display.build( _ctx )
