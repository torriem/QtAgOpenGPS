I'll put my branch notes/changes here.

Zones page UI done. Need to clean up once the backend is present.

Added customized SpinBox es with warning, and another one with single decimal point...Have a little work left on that one.


These are dead ends I ran into...

Need to set steerconfigwindow/line editor to their own windows, so we can move them around on the screen, instead of being anchored to one spot

config pages need to close the submenu if parent is clicked... I would like to write one function to check...

I also haven't figured out how to solve the "ReferenceError:etc." warning

need to rework the vehicle/field/line picker.
right now they're coded to /home/[my user]/Documents/QtAgOpenGPS/etc., so they will only work on my computer.
using ~/Documents/QtAgOpenGPS/etc does not work

Need to change the AB Line/curve(later) editors. 
Right now they are linked to the AB Line button.
So you turn off the AB Line when you open the window to pick which line you want. 
They need to open if the AB Line is active, and the line button is clicked
