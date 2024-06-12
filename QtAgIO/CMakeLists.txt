cmake_minimum_required(VERSION 3.22)

# Create the agio library
add_library(qtagio
    inc/IORouter.h src/IORouter.cpp
    inc/Connection.h src/Connection.cpp
    inc/SerialConnection.h src/SerialConnection.cpp
    inc/UdpConnection.h src/UdpConnection.cpp
	inc/UdpListener.h src/UdpListener.cpp
	inc/PacketHandler.h src/PacketHandler.cpp
)

target_include_directories(qtagio
    PRIVATE src
    PUBLIC inc
)

target_link_libraries(qtagio
    Qt6::Core
    Qt6::Network
)

# Create the AgIO executable
qt_add_executable(QtAgIO
    main.cpp
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

