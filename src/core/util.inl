#pragma once
#include "util.hpp"
namespace renderme::util
{
	template<class T>
	auto Singleton<T>::instance()->T&
	{
		static T _instance;
		return _instance;
	}
}