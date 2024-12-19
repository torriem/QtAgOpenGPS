QtAgOpenGPS
===========
Ag Precision Mapping and Section Control Software

Note!
==========
I haven't compiled for Windows in awhile. If this branch ever won't compile on Windows, contact me, or have a go at it yourself!

Dev Notes
----------
Developer how-to has been moved to the [wiki](https://github.com/torriem/qtagopengps/wiki)

What is QtAgOpenGPS?
--------------------
QtAgOpenGPS is a direct port of Brian Tischler's [AgOpenGPS](https://github.com/farmerbriantee/AgOpenGPS), which was originally
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

Copyright
---------
Much of the code is copied straight from the AOG C Sharp sources and is
therefore copyright Brian Tischler.  Everything else is copyright 
Michael Torrie (torriem@gmail.com) and Muhktimar (tamirscn@gmail.com).

License
-------
AOG was originally licensed under the GPLv3, so this port was also 
licensed under the GPLv3.  AOG has since been relicensed to the MIT
license, which is still compatible with the GPLv3, so this project
remains GPLv3 for now.

Requirements
------------
QtAOG requires Qt 6.0 or newer to build, on any Qt-supported platform
that supports OpenGL ES 2 or newer, or DirectX on Windows.
It also requires cmake 3.22 or newer.

Why this Port?
--------------
This port is mainly for my own entertainment, to allow me to run AOG
on Linux, including SBCs like the Raspberry Pi.  But I think the 
most desirable target will be Android some day.

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

Earlier I changed the case of methods to be more standard C++. I've
started to undo that and make the method calls as close to AOG's
original names as possible.

Most variables, functions, and methods, retain the AOG names, unless
architectural differences require moving code into different sections.
For example, the OpenGL code runs in a different thread than the main
GUI loop, the logic to set UI state has been pulled out of the function
that does the actual drawing.  Also since QtQuick itself uses OpenGL
heavily, there's no point in having an "intializeGL" routine; rather
each time we draw the frame we have to set all the variables including
the model view and perspective matrices.  Of course in OpenGL ES we
must manage those matrices ourselves anyway.

QtAgIO Notes
-------------
QtAgIO only works with UDP. The GUI is only half done.
Note that the loopback
ports are changed to 17770 and 15550, instead of 17777 and 15555 like AOG,
so we can run with AOG on the same device. If you change back to 17777, and 
15555, (in the settings.ini), you can run QtAOG with Brian's AgIO, and vice versa.

The network flow is not all correct yet. There is no "try to reconnect" code
right now. If you didn't connect the first time, you have to restart QtAgIO.

Status of the Port
------------------
As of Jan 4, 2023, the backend code is now tracking pretty closely to the
progress being made on AgOpenGPS/isoxml branch, at least as of Dec 20,
2023.

UI is still mostly non-present, but works with the built-in simulator.
For testing purposes, a job and field is automatically started, and a 
demo AB line is defined at 5 degrees.  You should be able to copy 
Boundary.txt, Sections.txt, ABLines.txt, etc from AgOpenGPS into the
QtAgOpenGS/Fields/TestField folder and work with previously-saved
data.

Coverage works, boundaries work, u-turn works, automatic u-turn works (but
has no button to enable it).  AB Line following works. Headland mode works.

Bugs and TODOs
--------------
- GL font drawing has issues, but only with the AB Line number display.  
  A lot of UI stuff currently drawn with GL should be drawn with QML
  widgets instead.
- Dashed lines are not possible in OpenGL ES, but I think a shader script
  can do it.  Also there's no easy way to do thick lines in opengl ES either.
- Hook in the GUI that David Wedel is contributing.

----------------
