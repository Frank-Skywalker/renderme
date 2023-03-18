#include "material.hpp"

namespace renderme
{
	Material::Material(std::string name) :name{ std::move(name) } {};
}