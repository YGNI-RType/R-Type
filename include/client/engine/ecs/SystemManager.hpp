#pragma once

#include "System.hpp"

namespace ecs {
    class SystemManager {
        public:
            template <class ...Components, typename Function>
            void addSystem(Function &&f);
        private:
    };
}
