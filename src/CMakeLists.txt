cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(wx_listctrl_tutorial_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# hack for buggy CMake's FindwxWidgets script
if (DEFINED ENV_WX_CONFIG)
    set (ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)

find_package(wxWidgets REQUIRED)

set(SRCS main.cpp)

include(${wxWidgets_USE_FILE})

if (APPLE)
    # create bundle on apple compiles
    add_executable(main MACOSX_BUNDLE ${SRCS} )
    # Set a custom plist file for the app bundle - needed for Mac OS Retina display
    set_target_properties(main PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platforms
    add_executable(main WIN32 ${SRCS})
endif()

target_link_libraries(main PRIVATE ${wxWidgets_LIBRARIES})

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/../data/aapl-1d.csv
                ${CMAKE_CURRENT_BINARY_DIR}/aapl-1d.csv)

add_compile_definitions(CSV_DIR="${CMAKE_CURRENT_BINARY_DIR}")

# Install configuration
if (APPLE)
    # Install the macOS bundle
    install(TARGETS main
            BUNDLE DESTINATION .
            RUNTIME DESTINATION .)
    
    # Install the CSV data file into the bundle's Resources directory
    install(FILES ${CMAKE_SOURCE_DIR}/../data/aapl-1d.csv
            DESTINATION main.app/Contents/Resources)
else()
    # Install the executable
    install(TARGETS main
            RUNTIME DESTINATION bin)
    
    # Install the CSV data file alongside the executable
    install(FILES ${CMAKE_SOURCE_DIR}/../data/aapl-1d.csv
            DESTINATION bin)
endif()

# Set proper permissions for the executable
if (UNIX AND NOT APPLE)
    install(CODE "execute_process(COMMAND chmod +x \${CMAKE_INSTALL_PREFIX}/bin/main)")
endif()