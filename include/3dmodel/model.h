#pragma once

#include "3dmodel/mesh.h"
#include "util/macro.h"

#include <assimp/Importer.hpp>
#include <vector>

class Model {
public:
	Model(const char* path);
	NO_COPY(Model);
	NO_MOVE(Model);

private:
	Assimp::Importer importer;
	std::vector<Mesh> meshes;
};
