#include "aogproperty.h"
#include "propdefs.h"

AOGSettings settings;

AOGProperty test_property("test/test", false);

bool test(void) {
    bool t;

    test_property.value().toUInt();
    t = test_property;

    test_property=5;
    return t;
}

