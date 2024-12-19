#include <limits>

namespace glm {
    static const double DOUBLE_MAX = std::numeric_limits<double>::max();
    static const double DOUBLE_MIN = std::numeric_limits<double>::min();
    static const double FLOAT_MAX = std::numeric_limits<float>::max();
    static const double FLOAT_MIN = std::numeric_limits<float>::min();

    static const double BYTE_MAX = std::numeric_limits<uchar>::max();
    static const double SHORT_MAX = std::numeric_limits<short>::max();
    static const double USHORT_MAX = std::numeric_limits<ushort>::max();
}
