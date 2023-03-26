#pragma once
#include "util.hpp"
#include<functional>
namespace renderme
{
	template<class T>
	auto Singleton<T>::instance()->T&
	{
		static T _instance;
		return _instance;
	}

	template<class T>
	auto hash_then_combine_hash(std::size_t lhs, T const& rhs) -> std::size_t
	{
		std::hash<T> hasher;
		return combine_hash(lhs, hasher(rhs));
	}

	template<class T>
	auto Hasher::hash(T const& rhs) -> Hasher&
	{
		_value = hash_then_combine_hash(_value, rhs);
		return *this;
	}
}