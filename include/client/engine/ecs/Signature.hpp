#pragma once

#include <bitset>

#define MAX_COMPONENTS 32

namespace ecs {
    class Signature {
    public:
        Signature();
    protected:
    private:
        std::bitset<MAX_COMPONENTS> m_Signature;
    };
}
