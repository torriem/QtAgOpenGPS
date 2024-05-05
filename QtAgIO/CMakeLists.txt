cmake_minimum_required(VERSION 3.22)

# Create the AgIO executable
qt_add_executable(QtAgIO
    main.cpp
    IORouter.h IORouter.cpp
    Connection.h Connection.cpp
    SerialConnection.h SerialConnection.cpp
    UdpConnection.h UdpConnection.cpp
)

# Libraries to link
target_link_libraries(QtAgIO PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::QuickWidgets
)

target_compile_options(QtAgIO PRIVATE 
    -Wall 
    -Werror
)

