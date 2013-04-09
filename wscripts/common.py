# -*- coding: utf-8 -*-

import os.path

APPNAME = 'dpsamples'

DPDIR = 'dpdir'

X86 = 'x86'
AMD64 = 'amd64'

LINUX = 'linux'
WINDOWS = 'windows'

DEBUG = 'debug'
RELEASE = 'release'

COMMON = 'common'

BUILD_DIR = 'build'
INCLUDE_DIR = 'inc'
SOURCE_DIR = 'src'

SAMPLE_DIR = 'sample'

def generateLibraryName( _name ):
    return 'dp_' + _name

def buildSample(
    _ctx,
    _target,
    _includes,
    _libpath,
    _sources,
    _libraries,
):
    _ctx.program(
        target = _target,
        includes = _includes,
        source = _sources,
        lib = _libraries,
        libpath = _libpath,
    )

def generateLibpath( _ctx ):
    return [
        os.path.join(
            _ctx.env[ i ],
            BUILD_DIR,
            _ctx.variant,
        )
        for i in [
            DPDIR,
        ]
    ]
