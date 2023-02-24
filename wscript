#!/usr/bin/env python


def options(ctx):
    """
    Define configuration options.
    """

    ctx.load('compiler_cxx')

    # enable C++ debugging symbols
    ctx.add_option(
        '--debug',
        action='store_true',
        dest='debug',
        help="debugging symbols")

    ctx.recurse('vendor')
    ctx.recurse('pkg/lib')
    ctx.recurse('pkg/test')
    ctx.recurse('pkg/bq')


def configure(ctx):
    """
    Configure the project.
    """

    ctx.load('compiler_cxx')

    # configure C++ compiler
    ctx.env.append_value('CXXFLAGS', ['-std=c++17', '-Wall', '-Werror'])
    if ctx.options.debug:
        ctx.env.append_value('CXXFLAGS', ['-g', '-O0'])

    ctx.recurse('vendor')
    ctx.recurse('pkg/lib')
    ctx.recurse('pkg/test')
    ctx.recurse('pkg/bq')


def build(ctx):
    """
    Build the project.
    """

    ctx.recurse('vendor')
    ctx.recurse('pkg/lib')
    ctx.recurse('pkg/test')
    ctx.recurse('pkg/bq')
