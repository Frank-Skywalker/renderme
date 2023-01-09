#pragma once
#include <core/integrator.hpp>
#include <core/film.hpp>

#include<map>;
#include<list>;
#include<set>
#include<algorithm>

namespace renderme
{

	struct Edge final
	{
		// Not changed during zbuffer
		int ymax;
		Float dxdl;

		// Changed during zbuffer
		int dy;
		Float x;
		Float z;
	};

	using Active_Edge_List = std::list<Edge*>;

	struct Polygon final
	{
		// Not changed during zbuffer
		glm::vec4 equation;
		Float dzdx;
		Float dzdy;
		glm::vec3 color;
		
		// Changed during zbuffer
		int dy;
		std::vector<Edge> edges;
		Active_Edge_List active_edge_list;

	};

	using Polygon_Table = std::vector<std::vector<Polygon>>;
	using Active_Polygon_List = std::list<Polygon*>;


	struct ZBuffer_Integrator final : Integrator
	{

		ZBuffer_Integrator();

		auto gl_draw(Camera const* camera, Scene const& scene) -> void;
		auto render(Camera const* camera, Scene const& scene, Film* film)  -> void;
		auto imgui_config() ->void;

	private:
		//Build the polygon table and edge table
		auto build_polygon_table(Camera const* camera, Scene const& scene, Film* film)->void;
		auto perform_zbuffer(Film* film)->void;
		auto clean_polygon_table()->void;

		Polygon_Table polygon_table;
	};

}