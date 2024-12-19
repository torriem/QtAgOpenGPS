cmake_minimum_required(VERSION 3.22)

project(QtAgIO
        VERSION 0.0
        HOMEPAGE_URL https://github.com/torriem/QtAgOpenGPS
        LANGUAGES CXX
)


#Qt magic
qt_standard_project_setup()



# Create the AgIO executable
qt_add_executable(QtAgIO
    main.cpp
    $<$<NOT:$<BOOL:${LOCAL_QML}>>:agio.qrc>
    agioproperties.cpp
    agioproperty.h agioproperty.cpp
    agiosettings.h agiosettings.cpp
    formloop.h formloop.cpp
    formloop_udpcomm.cpp
    formloop_nmea.cpp
    formloop_ntripcomm.cpp
    formloop_ui.cpp
    formloop_settings.cpp
    properties.h
    qmlsettings.h qmlsettings.cpp
    qmlsettings_addkeys.cpp
    qmlutil.h
    formloop_formudp.cpp
    inc/CTraffic.h src/CTraffic.cpp
    inc/CScanReply.h src/CScanReply.cpp
    interfaceproperty.h
)

if(LOCAL_QML)
    add_compile_definitions(LOCAL_QML)

    # Display the qrc file in the project tree without compiling it
    add_custom_target(AgIOQML SOURCES agio.qrc)

    message("Local QML is ON!")
elseif(NOT ANDROID)
    message("Local QML is off. Turn on for faster QML development")
    message("See https://github.com/torriem/QtAgOpenGPS/wiki/Building-QtAgOpenGPS-without-compiling-resources-with-QMake for more information")
endif()
target_include_directories(QtAgIO
    PRIVATE src
    PUBLIC inc
    PRIVATE .
)

# Libraries to link
target_link_libraries(QtAgIO PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::Multimedia
)

target_compile_options(QtAgIO PRIVATE 
    -Wall 
    -Werror
)

