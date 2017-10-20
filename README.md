QtAgOpenGPS
===========
Ag Precision Mapping and Section Control Software

What is QtAgOpenGPS?
--------------------
QtAgOpenGPS is a direct port of Brian Tischler's [AgOpenGPS]
(https://github.com/farmerbriantee/AgOpenGPS), which was originally
written in C#.  This port aims to follow AgOpenGPS closely, and not
introduce any new algorithms or significant architecture modifications,
except as required to work with Qt and C++.  Currently it's not quite
up to the latest commits on Brian's C# AOG, but nearly so.

Quoting the README.me for AgOpenGPS:

"This project is for my personal use only, and has no commercial value
whatsoever. This software is not for sale, is incomplete, is in
development to show concepts only and is mostly non functional. Any
use of this software is not recommended and is intended for simulation
only.

This software reads NMEA strings for the purpose of recording and
mapping position information for Agricultural use. Also it has up to 8
Section Control to control implements application of product preventing
over-application.

Also ouputs angle delta and distance from reference line for AB line
and Contour guidance."

Eventually the other utilities in AOG will be ported, including the
NMEA simulator.

This application is distributed here in source code form only. I will
post demo binaries outside of this tree somewhere.

License
-------
Since QtAOG is a direct port of AOG, this project is bound by the same
license as AOG, which is the GPLv3, and much of the code is still
copyrighted by the creator of AOG, farmerbriantee.

Requirements
------------
QtAOG requires Qt 5.8 or newer to build, on any Qt-supported platform
that has full OpenGL 4. It currently will not run on OpenGL ES, though
that will come in the future.  Although graphics cards on Windows often
have full OpenGL 4 support, Qt itself only supports OpenGL ES on
Windows. However calling OpenGL calls directly works and that is what
I'm going to do for now. Once OpenGL ES support is in, QtAOG will use
Qt wrappers for all opengl calls.

Why this Port?
--------------
This port is mainly for my own entertainment, to allow me to run AOG
on Linux. When the OpenGL calls are converted to OpenGL ES, QtAOG
should run on ARM-based computers and tablets also.  As the main
interface is built using QtQuick components, a port to Android is
possible as well.

Notes on the Port
-----------------
This port is as close to a 1:1 transliteration of the C# code as
possible, using Qt to drive the GUI, and C++ and Qt together to replace
the C# GUI components.  Being such a direct translation, the code has
a very C# feel to it, even in C++.  There are lots of classes that are
only instantiated once, and the formgps.h is very large, and the
coupling between the various classes is extremely tight.  In fact there
are a lot of forward references to the main FormGPS class.  Since
formgps.h is required by just about class in the project, changes to
formgps.h require a rebuild of every single object file.

Some symbols like function names have been changed to lower case letter
first, following the convention Qt uses, and is often common in C++.
Capital first letters are reserved for class names.  However most
variables, functions, and methods, retain the AOG names, unless
architectural differences require moving code into different sections.
For example, the OpenGL code runs in a different thread than the main
GUI loop, the logic to set UI state has been pulled out of the function
that does the actual drawing.  Also since QtQuick itself uses OpenGL
heavily, there's no point in having an "intializeGL" routine; rather
each time we draw the frame we have to set all the variables including
the model view and perspective matrices.  Of course in OpenGL ES we
must manage those matrices ourselves anyway.

Status of the Port
------------------
As of Oct 19, 2017, all of the core logic of AOG has been ported. This
includes coverage tracking, section control lookahead, field boundaries,
AB Lines, Contour tracking, the works.  However there are currently no
interfaces with the serial ports, so no way of actually controlling
sections or steering yet. Also very little of the UI has been hooked
up. Most buttons do nothing.  The app will take NMEA data over UDP, and
for testing purposes, it runs with an open job so you can drive around
and turn on sections.

Configuration is currently done entirely through a config file that
QtAOG creats automatically on first run. In Linux this is
$HOME/.config/AgOpenGPS/QtAgOpenGPS.conf.

The next stage is to hook up the UI buttons and implement field job
control.  Also the settings UI needs to be created.
