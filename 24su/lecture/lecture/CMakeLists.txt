cmake_minimum_required(VERSION 3.16)

project(lecture LANGUAGES C)

add_executable(lecture main.c
)
include_directories(SYSTEM "/foo/bar")

include(GNUInstallDirs)
install(TARGETS lecture
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
