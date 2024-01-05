#include "aogproperty.h"
#include "propdefs.h"

//AOGSettings settings;

AOGProperty test_property("test/test", false);
//QtAOG-specific setting properties
AOGProperty property_environment_last_name("environment/name",QString(""));


bool test(void) {
    bool t;

    test_property.value().toUInt();
    t = test_property;

    test_property=5;
    return t;
}

