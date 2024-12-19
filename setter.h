#ifndef SETTER_H
#define SETTER_H

/* Convenience macro for defining inline setters for
 * C++ member variables that are also QML properties.
 * The setter will call the Changed signal to inform QML
 * when C++ makes a change to a variable.  Note that the
 * property must have been defined using the NOTIFY
 * signal in the form varableChanged.
 */

//emit keyword is usually an empty macro, if we use it here it just
//disappears anyway. Ignore QT Creators warning about missing emit
#define SETTER(T, x, sx) inline void sx (T value) { if (x != value) { x = value; x##Changed(); } }

#endif // SETTER_H
