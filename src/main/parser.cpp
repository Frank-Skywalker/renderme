#include "parser.hpp"

#include<core/log.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
namespace renderme
{
	auto parse_obj(std::string const& path)->void
	{
		Assimp::Importer importer;

		auto aiscene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
		);

		if ((aiscene == nullptr) ||
			(aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
			(aiscene->mRootNode == nullptr)
			) {
			log(Status::fatal, importer.GetErrorString());
		}


	}

	auto parse_from_file(std::string const& path)->Scene
	{


		return Scene{};
	}

}