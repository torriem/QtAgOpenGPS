#include "toolmenufunc.h"
#include "qmlutil.h"

void toolMenufunc::miscfunc()
{
qml_root = this->rootObject();
btndelAppliedArea = qmlItem(qml_root, "delAppliedArea");
}
