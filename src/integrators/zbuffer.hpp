#pragma once
#include <core/integrator.hpp>
#include <core/film.hpp>

#include<map>;

namespace renderme
{
	//Classified using ymax
	struct Classified_Polygon
	{
		Float a, b, c, d;
		unsigned int index;
		unsigned int yspan;
		Vector3f color;
	};


	using Classified_Polygon_Table = std::map<unsigned int, std::vector<Classified_Polygon>>;

	//Classified using ymax
	struct Classified_Edge
	{
		Float ymax_x;
		Float dx;           //Direction: from ymax to ymin
		unsigned int yspan;
		unsigned int polygon_index;
	};

	using Classified_Edge_Table = std::map<unsigned int, std::vector<Classified_Edge>>;

	
	struct Active_Polygon
	{
		Float a, b, c, d;
		unsigned int index;
		unsigned int yspan;
		Vector3f color;
	};

	using Active_Polygon_Table = std::vector<Active_Polygon>;
	
	struct Active_Edge_Pair
	{
		Float x_left;
		Float dx_left;
		Float yspan_left;


		Float x_right;
		Float dx_right;
		Float yspan_right;

		Float z_left;
		Float dzx;
		Float dzy;
		unsigned int polygon_index;
	};

	using Active_Edge_Pair_Table = std::vector<Active_Edge_Pair>;


	struct ZBuffer_Integrator final : Integrator
	{

		ZBuffer_Integrator(Film* film);

		auto gl_draw(Scene const& scene) const noexcept -> void;
		auto render(Scene const& scene) const noexcept -> void;
		auto imgui_config() ->void;

	private:


		Film* film;
	};

}