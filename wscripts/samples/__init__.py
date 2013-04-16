# -*- coding: utf-8 -*-

from . import args, string, gamepad

def build( _ctx ):
    args.build( _ctx )
    string.build( _ctx )
    gamepad.build( _ctx )
