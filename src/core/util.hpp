#pragma once
#include <cstddef>

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


	auto combine_hash(std::size_t lhs, std::size_t rhs) -> std::size_t;
	
	template<class T>
	auto hash_then_combine_hash(std::size_t lhs, T const& rhs) -> std::size_t;

	struct Hasher
	{
		auto hash_hash(std::size_t rhs) -> Hasher&;

		template<class T>
		auto hash(T const& rhs) -> Hasher&;

		auto value() const noexcept->std::size_t;
	private:

		std::size_t _value;
	};

}

#include "util.inl"