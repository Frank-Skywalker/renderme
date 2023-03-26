#pragma once
#include<glm/glm.hpp>
#include <cstddef>
#include<functional>

// custom specialization of std::hash can be injected in namespace std
template<class T>
struct std::hash<glm::tvec2<T>>
{
    std::size_t operator()(glm::tvec2<T> const& v) const noexcept
    {
        std::size_t h1 = std::hash<T>{}(v.x);
        std::size_t h2 = std::hash<T>{}(v.y);
        return h2 ^ (h1 << 1); // or use boost::hash_combine
    }
};

template<class T>
struct std::hash<glm::tvec3<T>>
{
    std::size_t operator()(glm::tvec3<T> const& v) const noexcept
    {
        std::size_t h1 = std::hash<T>{}(v.x);
        std::size_t h2 = std::hash<T>{}(v.y);
        std::size_t h3 = std::hash<T>{}(v.z);
        return h3 ^ ((h2 ^ (h1 << 1)) << 1); // or use boost::hash_combine
    }
};