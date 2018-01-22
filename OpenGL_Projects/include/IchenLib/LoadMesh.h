#ifndef __LOADMESH_H__
#define __LOADMESH_H__

#include <string>
#include <GL/glew.h>
#include "assimp/Scene.h"
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


struct MeshData
{
   unsigned int mVao;
   unsigned int mVboVerts;
   unsigned int mVboNormals;
   unsigned int mVboTexCoords;
   unsigned int mIndexBuffer;
   float mScaleFactor; //TODO replace with bounding box
   unsigned int mNumIndices;
   //const aiScene* mScene;
   aiVector3D mBbMin, mBbMax;
   MeshData::MeshData(): mVao(-1), mVboVerts(-1), mVboNormals(-1), mVboTexCoords(-1), mIndexBuffer(-1), mScaleFactor(0.0f), mNumIndices(0) {}
      
};

MeshData BufferIndexedVerts(aiMesh* meshdata);
void GetBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max);
void GetBoundingBox(const aiMesh* mesh, aiVector3D* min, aiVector3D* max);

class Model
{
public:
	std::vector<MeshData> meshes; // All the meshes including sub-mesh
	float mScaleFactor;
	aiVector3D mBbMin, mBbMax;

	Model() {};
	Model(const std::string& path) { loadModel(path); }

	// Draw using current shader
	void Draw();
	void Reload(const std::string& path);
private:
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	MeshData processMesh(aiMesh* mesh);
};

#endif