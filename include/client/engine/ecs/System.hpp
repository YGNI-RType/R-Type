#pragma once

#include <set>

#include "Entity.hpp"
#include "Signature.hpp"

namespace ecs {
    class System {
        private:
            std::bitset<MAX_COMPONENTS> m_signature;
            std::set<Entity> m_subbedEntities;
    };
}
