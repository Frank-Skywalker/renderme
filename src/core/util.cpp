#include "util.hpp"
namespace renderme
{
	auto combine_hash(std::size_t lhs, std::size_t rhs) -> std::size_t
	{
		return lhs ^ (rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2));
	}

	auto Hasher::hash_hash(std::size_t rhs) -> Hasher&
	{
		_value = combine_hash(_value, rhs);
		return *this;
	}

	auto Hasher::value() const noexcept->std::size_t
	{
		return _value;
	}
}