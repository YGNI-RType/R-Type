#pragma once

#include <cstdlib>

namespace ecs {
    typedef std::size_t Entity;
}

// namespace ecs {
//     class Entity {
//     public:
//         Entity(std::size_t id);

//         operator std::size_t();
//         bool operator==(const std::size_t size) const;
//         bool operator!=(const std::size_t size) const;

//     protected:
//     private:
//         std::size_t m_id;
//     };
// }
