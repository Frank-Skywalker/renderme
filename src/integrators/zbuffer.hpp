#pragma once
#include <core/integrator.hpp>
#include <core/film.hpp>

#include<map>;

namespace renderme
{
	//Classified using ymax
	struct Classified_Polygon
	{
		glm::vec4 equation;
		unsigned int index;
		unsigned int yspan;
		glm::vec3 color;
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
		glm::vec4 equation;
		unsigned int index;
		unsigned int yspan;
		glm::vec3 color;
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

		ZBuffer_Integrator();

		auto gl_draw(Camera const* camera, Scene const& scene) -> void;
		auto render(Camera const* camera, Scene const& scene, Film* film)  -> void;
		auto imgui_config() ->void;

	private:
		auto build_data_structures(Camera const* camera, Scene const& scene)->void;
		auto perform_zbuffer(Film* film)->void;
		auto clean_data_structures()->void;


		unsigned int polygon_id;
	};

}