#pragma once


#include <vector>
#include <optional>

namespace ecs {

    template <typename Component>
    class ComponentSparseArray {
    public:
        ComponentSparseArray() {};

        void setComponent(std::size_t pos, const Component &c) {
            if (pos >= m_data.size() + 1)
                m_data.resize(pos + 1);

            m_data[pos] = c;
            return m_data[pos];
        }
        void setComponent(std::size_t pos, const Component &&c) {
            if (pos >= m_data.size() + 1)
                m_data.resize(pos + 1);

            m_data[pos] = std::move(c);
            return m_data[pos];
        }
        void unsetComponent(std::size_t pos) {
            if (pos >= size())
                return;

            m_data[pos].reset();
        }
    protected:
    private:
        std::vector<std::optional<Component>> m_data;
    };

}
