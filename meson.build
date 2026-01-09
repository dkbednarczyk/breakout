project('Breakout', 'c', 'cpp',
  version : '0.1',
  default_options : [
    'c_std=c17',
    'cpp_std=c++20'
  ]
)

raylib = dependency('raylib')

local_inc = include_directories('include')
raygui_inc = include_directories('include/raygui/src')

src_files = files(
    'src/main.cpp',
    'src/game.cpp',
    'src/ball.cpp',
    'src/player.cpp',
    'src/level.cpp',
)

executable('breakout',
  src_files,
  include_directories : [local_inc, raygui_inc],
  dependencies : raylib
)