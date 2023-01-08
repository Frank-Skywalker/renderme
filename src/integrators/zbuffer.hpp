#pragma once
#include <core/integrator.hpp>
#include <core/film.hpp>

#include<map>;

namespace renderme
{

	struct Polygon final
	{
		unsigned int id;
		glm::vec4 equation;
		int dy;
		glm::vec3 color;
	};
	//Sorted using ymax
	using Polygon_Table = std::map<int, std::vector<Polygon>>;

	struct Edge final
	{
		unsigned int polygon_id;
		int dy;
		Float x;              // Uppder x
		Float dxdy;           // Direction: from ymax to ymin
	};
	//Sorted using ymax
	using Edge_Table = std::map<int, std::vector<Edge>>;


	//struct Active_Polygon final
	//{
	//	unsigned int id;
	//	glm::vec4 equation;
	//	int dy;
	//	glm::vec3 color;
	//};

	using Active_Polygon_Table = std::vector<Polygon*>;

	//struct Active_Edge_Pair final
	//{
	//	unsigned int polygon_id;
	//	Float dzdx;
	//	Float dzdy;

	//	Float z_left;

	//	Float x_left;
	//	Float dxdy_left;
	//	int dy_left;

	//	Float x_right;
	//	Float dxdy_right;
	//	int dy_right;
	//};

	struct Active_Edge_Pair final
	{
		Float dzdx;
		Float dzdy;
		Float z_left;

		Edge* left;
		Edge* right;
	};

	using Active_Edge_Pair_Table = std::vector<Active_Edge_Pair>;

	struct ZBuffer_Integrator final : Integrator
	{

		ZBuffer_Integrator();

		auto gl_draw(Camera const* camera, Scene const& scene) -> void;
		auto render(Camera const* camera, Scene const& scene, Film* film)  -> void;
		auto imgui_config() ->void;

	private:
		//Build the polygon table and edge table
		auto build_data_structures(Camera const* camera, Scene const& scene, Film* film)->void;
		auto perform_zbuffer(Film* film)->void;
		auto clean_data_structures()->void;


		unsigned int polygon_id;

		Polygon_Table polygon_table;
		Edge_Table edge_table;

		//Active_Polygon_Table active_polygon_table;
		//Active_Edge_Pair_Table active_edge_pair_table;

	};

}