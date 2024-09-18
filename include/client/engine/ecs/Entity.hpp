#pragma once

#include <cstdlib>

namespace ecs {
    class Entity {
    public:
        Entity(std::size_t id);

    protected:
    private:
        std::size_t m_id;
    };
}
