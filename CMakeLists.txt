# Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
# SPDX-License-Identifier: GNU General Public License v3.0 or later
# CMake 3.22 ships with Ubuntu 22.04
# CMake 3.27 and QtCreator 12 recommended
cmake_minimum_required(VERSION 3.22)

# Remove _cmake suffix when the .pro is removed.  Added here for clarity
project(QtAgOpenGPS
    VERSION 0.0
    HOMEPAGE_URL https://github.com/torriem/QtAgOpenGPS
    LANGUAGES CXX
)

# Configure project options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# Find required libraries
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

# Qt magic
qt_standard_project_setup()

# Create the executable
qt_add_executable(QtAgOpenGPS
    main.cpp
    $<$<NOT:$<BOOL:${LOCAL_QML}>>:agopengps.qrc>
    aogproperties.cpp
    aogproperty.h aogproperty.cpp
    aogrenderer.h aogrenderer.cpp
    aogsettings.h aogsettings.cpp
    btnenum.h
    common.h
    formgps.h formgps.cpp
    formgps_classcallbacks.cpp
    formgps_opengl.cpp
    formgps_position.cpp
    formgps_saveopen.cpp
    formgps_sections.cpp
    formgps_settings.cpp
    formgps_sim.cpp
    formgps_udpcomm.cpp
    formgps_ui.cpp
    formgps_ui_boundary.cpp
    formgps_ui_field.cpp
    formgps_ui_tracks.cpp
    formgps_ui_vehicle.cpp
    formheadache.h formheadache.cpp
    formheadland.h formheadland.cpp
    glutils.h glutils.cpp
    interfaceproperty.h
    properties.h
    qmlsectionbuttons.h qmlsectionbuttons.cpp
    qmlsettings.h qmlsettings.cpp
    qmlsettings_addkeys.cpp
    qmlutil.h
    testlists.cpp
    testnmea.cpp
    classes/cabcurve.h classes/cabcurve.cpp
    classes/cabline.h classes/cabline.cpp
    classes/cahrs.h classes/cahrs.cpp
    classes/cboundary.h classes/cboundary.cpp
    classes/cboundarylist.h classes/cboundarylist.cpp
    classes/ccamera.h classes/ccamera.cpp
    classes/ccontour.h classes/ccontour.cpp
    classes/cdubins.h classes/cdubins.cpp
    classes/cfence.cpp
    classes/cfielddata.h classes/cfielddata.cpp
    classes/cflag.h classes/cflag.cpp
    classes/cguidance.h classes/cguidance.cpp
    classes/chead.cpp
    classes/cheadline.h classes/cheadline.cpp
    classes/cmodulecomm.h classes/cmodulecomm.cpp
    classes/cnmea.h classes/cnmea.cpp
    classes/cpatches.h classes/cpatches.cpp
    classes/cpgn.h classes/cpgn.cpp
    classes/crecordedpath.h classes/crecordedpath.cpp
    classes/csection.h classes/csection.cpp
    classes/csim.h classes/csim.cpp
    classes/ctool.h classes/ctool.cpp
    classes/ctrack.h classes/ctrack.cpp
    classes/ctram.h classes/ctram.cpp
    classes/cturn.cpp
    classes/cturnlines.cpp
    classes/cvehicle.h classes/cvehicle.cpp
    classes/cworldgrid.h classes/cworldgrid.cpp
    classes/cyouturn.h classes/cyouturn.cpp
    classes/glm.h
    classes/vec2.h classes/vec2.cpp
    classes/vec3.h classes/vec3.cpp
    classes/vec4.h classes/vec4.cpp
    classes/vecfix2fix.h
)

# Display the qrc file in the project tree without compiling it
if(LOCAL_QML)
    add_custom_target(AOGQML SOURCES agopengps.qrc)
endif()


option(LOCAL_QML "Load QML from disc instead of compiling them in.")

if(LOCAL_QML)
	add_compile_definitions(LOCAL_QML)
endif()
# Where to look for header files
target_include_directories(QtAgOpenGPS PRIVATE
    .
    classes
)

# Libraries to link
target_link_libraries(QtAgOpenGPS PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGL
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::Network
	Qt6::Multimedia
)

# Compile defines
target_compile_definitions(QtAgOpenGPS PRIVATE
    QT_DEPRECATED_WARNINGS
)

set_target_properties(QtAgOpenGPS PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)


# TODO .pro leftovers:
#DISTFILES += \
#    CMakeLists.txt \
#    TODO.txt \
#    README.md \
#    LICENSE

add_subdirectory(QtAgIO)
