cmake_minimum_required(VERSION 3.22)

target_include_directories(qtagio
    PRIVATE src
    PUBLIC inc
	.
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
	agiosettings.h agiosettings.cpp
	formloop.h formloop.cpp
	formloop_udpcomm.cpp
    formloop_nmea.cpp
    formloop_ntripcomm.cpp
    formloop_ui.cpp
    qmlutil.h
    #inc/IORouter.h src/IORouter.cpp
    inc/Connection.h src/Connection.cpp
    inc/SerialConnection.h src/SerialConnection.cpp
    inc/UdpConnection.h src/UdpConnection.cpp
	inc/CTraffic.h src/CTraffic.cpp
	inc/CScanReply.h src/CScanReply.cpp
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

