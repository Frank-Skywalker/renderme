#pragma once
namespace renderme
{

	struct Only_Movable
	{
		Only_Movable() = default;
		~Only_Movable() = default;

		Only_Movable(Only_Movable const&) = delete;
		auto operator=(Only_Movable const&)->Only_Movable & = delete;

		Only_Movable(Only_Movable&&) = default;
		auto operator=(Only_Movable&&)->Only_Movable & = default;


	};

	struct Non_Transferable
	{
		Non_Transferable() = default;
		~Non_Transferable() = default;

		Non_Transferable(Non_Transferable const&) = delete;
		auto operator=(Non_Transferable const&)->Non_Transferable & = delete;

		Non_Transferable(Non_Transferable&&) = delete;
		auto operator=(Non_Transferable&&)->Non_Transferable & = delete;
	};


	template<class T>
	struct Singleton : Non_Transferable
	{
		static auto instance()->T&;
	};

}

#include "util.inl"