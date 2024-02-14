#include "cboundary.h"
#include "aogproperty.h"

CBoundary::CBoundary(QObject *parent) : QObject(parent)
{
    turnSelected = 0;
}

void CBoundary::loadSettings() {
    isSectionControlledByHeadland = property_setHeadland_isSectionControlled;
}
