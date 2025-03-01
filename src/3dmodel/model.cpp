#include "3dmodel/model.h"

#include "3dmodel/mesh.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

Mesh create_mesh(const aiScene* scene, const aiMesh* mesh);
Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal);

Model::Model(const char* path) {
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
	                                                   aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	const aiNode* root = scene->mRootNode;
	this->meshes.reserve(root->mNumMeshes);
	for (unsigned i = 0; i < root->mNumMeshes; ++i) {
		unsigned mesh_idx = root->mMeshes[i];
		this->meshes.emplace_back(create_mesh(scene, scene->mMeshes[mesh_idx]));
	}
	std::cout << "lights: " << scene->HasLights() << " " << scene->mNumLights << std::endl;
	std::cout << "cameras: " << scene->HasCameras() << " " << scene->mNumCameras << std::endl;
	std::cout << "materials: " << scene->HasMaterials() << " " << scene->mNumMaterials << std::endl;
	std::cout << "meshes: " << scene->HasMeshes() << " " << scene->mNumMeshes << std::endl;
	std::cout << "animations: " << scene->HasAnimations() << " " << scene->mNumAnimations << std::endl;
}

Mesh create_mesh(const aiScene* scene, const aiMesh* mesh) {
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		vertices.emplace_back(create_vertex(mesh->mVertices[i], mesh->mNormals[i]));
	}
	std::vector<GLuint> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace& face = mesh->mFaces[i];
		if (face.mNumIndices != 3) {
			// not a triangle
			continue;
		}
		for (int i = 0; i < 3; ++i) {
			unsigned int index = face.mIndices[index];
			indices.push_back(index);
		}
	}
	indices.shrink_to_fit();
	return Mesh(std::move(vertices), std::move(indices));
}

Vertex create_vertex(const aiVector3D& position, const aiVector3D& normal) {
	return Vertex(glm::vec3(position.x, position.y, position.z), glm::vec3(normal.x, normal.y, normal.z));
}

void Model::render(Shader& shader, const Camera& camera) const {
	for (const Mesh& mesh : this->meshes) {
		mesh.render(shader, camera);
	}
}
