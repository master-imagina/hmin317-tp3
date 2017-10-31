#include "shaderparam.h"


bool operator==(const ShaderParam &lhs, const ShaderParam &rhs)
{
    if (lhs.name != rhs.name) {
        return false;
    }

    if (lhs.value != rhs.value) {
        return false;
    }

    return true;
}
