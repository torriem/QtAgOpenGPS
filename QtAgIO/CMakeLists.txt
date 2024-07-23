cmake_minimum_required(VERSION 3.22)

# Create the agio library
add_library(qtagio
    #inc/IORouter.h src/IORouter.cpp
    inc/Connection.h src/Connection.cpp
    inc/SerialConnection.h src/SerialConnection.cpp
    inc/UdpConnection.h src/UdpConnection.cpp
	inc/CTraffic.h src/CTraffic.cpp
	inc/CScanReply.h src/CScanReply.cpp
        inc/agiosettings.h src/agiosettings.cpp
	inc/formloop.h src/formloop.cpp
	src/formloop_udpcomm.cpp
        src/formloop_nmea.cpp
        src/formloop_ntripcomm.cpp
        src/formloop_ui.cpp
)


target_include_directories(qtagio
    PRIVATE src
    PUBLIC inc
)

target_link_libraries(qtagio
    Qt6::Core
    Qt6::Network
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
)

# Create the AgIO executable
qt_add_executable(QtAgIO
    main.cpp
	agio.qrc
)

# Libraries to link
target_link_libraries(QtAgIO PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::QuickWidgets
    qtagio
)

target_compile_options(QtAgIO PRIVATE 
    -Wall 
    -Werror
)

