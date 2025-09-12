cmake_minimum_required(VERSION 3.16)

file (STRINGS "version" BUILD_VERSION)

project(alpaca_zorro_plugin
        VERSION ${BUILD_VERSION}
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
endif()

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
endif()

configure_file(alpaca_zorro_plugin/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
configure_file(resources/resources.rc.in ${CMAKE_CURRENT_BINARY_DIR}/resources.rc)

# Enable precompiled headers
# set(CMAKE_CXX_PCH_ENABLED TRUE)

file(GLOB_RECURSE SOURCE_FILES alpaca_zorro_plugin/*.cpp alpaca/*.cpp market_data/*.cpp websocket/*.cpp)

set(RESOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/resources.rc
)

add_library(alpaca_zorro_plugin SHARED ${SOURCE_FILES} ${RESOURCES})
target_include_directories(alpaca_zorro_plugin PRIVATE 
    alpaca_zorro_plugin
    alpaca_zorro_plugin/zorro
    3rdparty/HowardHinnant/date/include
    3rdparty/rapidjson/include
    3rdparty/websocket_proxy/include
    3rdparty/slick_queue/include
    3rdparty/slick_logger/include
    resources
)

add_definitions(-D_WIN32_WINNT=0x0A00)
set(CMAKE_SUPPRESS_REGENERATION true)   # supress zero_check
set_target_properties(alpaca_zorro_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(alpaca_zorro_plugin PROPERTIES OUTPUT_NAME "Alpaca")

if (DEFINED ENV{ZORRO_ROOT})
    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "win32")
        set_target_properties(alpaca_zorro_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "$ENV{ZORRO_ROOT}/plugin")
    else()
        set_target_properties(alpaca_zorro_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "$ENV{ZORRO_ROOT}/plugin64")
    endif()
else()
    set_target_properties(alpaca_zorro_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
endif()
set_target_properties(alpaca_zorro_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

set_target_properties(alpaca_zorro_plugin PROPERTIES LINK_INCREMENTAL ON)
target_compile_definitions(alpaca_zorro_plugin PUBLIC _UNICODE) # set CharacterSet to unicode
target_compile_definitions(alpaca_zorro_plugin PUBLIC WIN32 _WINDOWS _USRDLL ALPACA_EXPORTS) # set CharacterSet to unicode
set_target_properties(alpaca_zorro_plugin PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# target_precompile_headers(alpaca_zorro_plugin PRIVATE alpaca_zorro_plugin/stdafx.h)

if (CMAKE_BUILD_TYPE MATCHES Release)
    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "win32")
        add_custom_command(TARGET alpaca_zorro_plugin
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/dist"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/Release/include"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/alpaca_zorro_plugin/AlpacaBrokerCommands.h" "${CMAKE_BINARY_DIR}/bin/Release/include/"
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/websocket_proxy/bin ${CMAKE_BINARY_DIR}/bin/Release/websocket_proxy
            COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/dist/AlpacaZorroPlugin_${BUILD_VERSION}.zip" --format=zip "./"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release"
            COMMENT "Creating zip archive"
        )
    else()
        add_custom_command(TARGET alpaca_zorro_plugin
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/dist"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/Release/include"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/alpaca_zorro_plugin/AlpacaBrokerCommands.h" "${CMAKE_BINARY_DIR}/bin/Release/include/"
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/websocket_proxy/bin ${CMAKE_BINARY_DIR}/bin/Release/websocket_proxy
            COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/dist/AlpacaZorroPlugin64_${BUILD_VERSION}.zip" --format=zip "./"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release"
            COMMENT "Creating zip archive 64bit"
        )
    endif()

    if (DEFINED ENV{ZORRO_ROOT})
        if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "win32")
            add_custom_command(TARGET alpaca_zorro_plugin
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/Release/Alpaca.dll $ENV{ZORRO_ROOT}/plugin/Alpaca.dll
            )
        else()
            add_custom_command(TARGET alpaca_zorro_plugin
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/Release/Alpaca.dll $ENV{ZORRO_ROOT}/plugin64/Alpaca.dll
            )
        endif()
    endif()
endif()