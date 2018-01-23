#include "LoadMesh.h"
#include <fstream>
#include <algorithm>

#include <GL/glew.h>
#include "assimp/Importer.hpp"
#include "assimp/PostProcess.h"
#include <numeric>
#include "../../../Gui demo/include/glm/detail/type_vec4.hpp"
#include <iostream>
#include <list>
#include <queue>

// Create an instance of the Importer class
Assimp::Importer importer;
// scale factor for the model to fit in the window

// Buffer array
unsigned int* faceArray;
float* verticeArray;
float* textureCoordArray;
float* normalArray;

// Arrays indexes
unsigned int faceIndex;
unsigned int verticesIndex;
unsigned int textureCoordIndex;
unsigned int normalIndex;

void GetBoundingBox(const aiMesh* mesh, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;

	for (unsigned int t = 0; t < mesh->mNumVertices; ++t)
	{
		aiVector3D tmp = mesh->mVertices[t];

		min->x = std::min(min->x, tmp.x);
		min->y = std::min(min->y, tmp.y);
		min->z = std::min(min->z, tmp.z);

		max->x = std::max(max->x, tmp.x);
		max->y = std::max(max->y, tmp.y);
		max->z = std::max(max->z, tmp.z);
	}
}


void GetBoundingBoxForNode(const aiScene* scene, const aiNode* nd, aiVector3D* min, aiVector3D* max)
{
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n)
	{
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t)
		{
			aiVector3D tmp = mesh->mVertices[t];

			min->x = std::min(min->x, tmp.x);
			min->y = std::min(min->y, tmp.y);
			min->z = std::min(min->z, tmp.z);

			max->x = std::max(max->x, tmp.x);
			max->y = std::max(max->y, tmp.y);
			max->z = std::max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n)
	{
		GetBoundingBoxForNode(scene, nd->mChildren[n], min, max);
	}
}


void GetBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max)
{
	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	GetBoundingBoxForNode(scene, scene->mRootNode, min, max);
}

MeshData LoadMesh(const std::string& pFile)
{
	MeshData mesh;

	//check if file exists
	std::ifstream fin(pFile.c_str());
	if (!fin.fail())
	{
		fin.close();
	}
	else
	{
		printf("Couldn't open file: %s\n", pFile.c_str());
		printf("%s\n", importer.GetErrorString());
		return mesh;
	}

	mesh.mScene = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality);
	//|aiProcess_FlipWindingOrder);
	//| aiProcess_PreTransformVertices);

	// If the import failed, report it
	if (!mesh.mScene)
	{
		printf("%s\n", importer.GetErrorString());
		return mesh;
	}

	// Now we can access the file's contents.
	printf("Import of scene %s succeeded.", pFile.c_str());

	GetBoundingBox(mesh.mScene, &mesh.mBbMin, &mesh.mBbMax);
	aiVector3D diff = mesh.mBbMax - mesh.mBbMin;
	float w = std::max(diff.x, std::max(diff.y, diff.z));

	mesh.mScaleFactor = 1.0f / w;

	BufferIndexedVerts(mesh);
	//mesh.mNumIndices = mesh.mScene->mMeshes[0]->mNumFaces * 3;
	return mesh;
}

void BufferIndexedVerts(MeshData& meshdata)
{
	if (meshdata.mVao != -1)
	{
		glDeleteVertexArrays(1, &meshdata.mVao);
	}

	if (meshdata.mIndexBuffer != -1)
	{
		glDeleteBuffers(1, &meshdata.mIndexBuffer);
	}

	if (meshdata.mVboVerts != -1)
	{
		glDeleteBuffers(1, &meshdata.mVboVerts);
	}

	if (meshdata.mVboTexCoords != -1)
	{
		glDeleteBuffers(1, &meshdata.mVboTexCoords);
	}

	if (meshdata.mVboNormals != -1)
	{
		glDeleteBuffers(1, &meshdata.mVboNormals);
	}

	//shader attrib locations
	int pos_loc = -1;
	int tex_coord_loc = -1;
	int normal_loc = -1;

	GLint program = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	pos_loc = glGetAttribLocation(program, "pos_attrib");
	tex_coord_loc = glGetAttribLocation(program, "tex_coord_attrib");
	normal_loc = glGetAttribLocation(program, "normal_attrib");

	glGenVertexArrays(1, &meshdata.mVao);
	glBindVertexArray(meshdata.mVao);

	// Load sub meshes
	auto mesh_num = meshdata.mScene->mNumMeshes;
	unsigned int face_num = 0, vertices_num = 0;
	for (unsigned int n = 0; n < mesh_num; ++n)
	{
		face_num += meshdata.mScene->mMeshes[n]->mNumFaces;
		vertices_num += meshdata.mScene->mMeshes[n]->mNumVertices;
	}

	//	const aiMesh* mesh = meshdata.mScene->mMeshes[0]; 
	//	unsigned int numFaces = mesh->mNumFaces;

	meshdata.mNumIndices = face_num * 3;
	faceArray = static_cast<unsigned int *>(malloc(sizeof(unsigned int) * face_num * 3));
	verticeArray = static_cast<float*>(malloc(sizeof(float) * vertices_num * 3));
	textureCoordArray = static_cast<float*>(malloc(sizeof(float) * vertices_num * 2));
	normalArray = static_cast<float*>(malloc(sizeof(float) * vertices_num * 3));
	faceIndex = 0;
	verticesIndex = 0;
	textureCoordIndex = 0;
	normalIndex=0;

	ProcessNode(meshdata,meshdata.mScene->mRootNode);

	//Buffer indices
	glGenBuffers(1, &meshdata.mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshdata.mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * face_num * 3, faceArray, GL_STATIC_DRAW);
	free(faceArray);

	//Buffer vertices
	glGenBuffers(1, &meshdata.mVboVerts);
	glBindBuffer(GL_ARRAY_BUFFER, meshdata.mVboVerts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices_num, verticeArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, 0, 0, nullptr);
	free(verticeArray);

	// buffer for vertex texture coordinates
	glGenBuffers(1, &meshdata.mVboTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, meshdata.mVboTexCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertices_num, textureCoordArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(tex_coord_loc);
	glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, 0, 0, nullptr);
	free(textureCoordArray);


	//buffer normals
	glGenBuffers(1, &meshdata.mVboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, meshdata.mVboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices_num, normalArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(normal_loc);
	glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, nullptr);
	free(normalArray);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ProcessMesh(aiMesh* current_mesh, aiNode* currentNode)
{
	// Face Indices
	auto f_num = current_mesh->mNumFaces;
	auto faces = current_mesh->mFaces;
	std::for_each(faces, faces + f_num, [&](const aiFace& f)
	{
		memcpy(&faceArray[faceIndex], f.mIndices, 3 * sizeof(unsigned int));
		faceIndex += 3;
	});

	// Vertices, textures, normals
	auto v_num = current_mesh->mNumVertices;

	if (current_mesh->HasPositions())
	{
		std::cout << "Current node: " << currentNode->mName.C_Str() << std::endl;
		auto vertices = current_mesh->mVertices;
		//memcpy(&verticeArray[verticesIndex], vertices, 3 * sizeof(unsigned int) * v_num);
		std::for_each(vertices, vertices + v_num, [&](aiVector3D& v)
		{
			auto tmpN = currentNode;
			std::vector<aiMatrix4x4> ms;
			while (tmpN->mParent != nullptr)
			{
				ms.push_back(tmpN->mTransformation);
				tmpN = tmpN->mParent;
			}
			aiMatrix4x4 fm = *ms.rbegin(); // final matrix
			for (auto m = ms.rbegin() + 1; m != ms.rend(); ++m)
			{
				fm = *m * fm;
			}

			//aiVector3D translate(fm.a4, fm.b4, fm.c4);
			//auto newV = v - translate;
			auto newV = v;
			verticeArray[verticesIndex++] = newV.x;
			verticeArray[verticesIndex++] = newV.y;
			verticeArray[verticesIndex++] = newV.z;
			std::cout << "Current node: " << currentNode->mName.C_Str() << "\t" << newV.x << " " << newV.y << " " << newV.z <<
				std::endl;
		});
		//verticesIndex += 3 * v_num;
	}

	if (current_mesh->HasTextureCoords(0))
	{
		for (unsigned int k = 0; k < current_mesh->mNumVertices; ++k)
		{
			textureCoordArray[textureCoordIndex + k * 2] = current_mesh->mTextureCoords[0][k].x;
			textureCoordArray[textureCoordIndex + k * 2 + 1] = current_mesh->mTextureCoords[0][k].y;
		}
		textureCoordIndex += 2 * current_mesh->mNumVertices;
	}

	if (current_mesh->HasNormals())
	{
		for (unsigned int k = 0; k < current_mesh->mNumVertices; ++k)
		{
			normalArray[normalIndex + k * 3] = current_mesh->mNormals[k].x;
			normalArray[normalIndex + k * 3 + 1] = current_mesh->mNormals[k].y;
			normalArray[normalIndex + k * 3 + 2] = current_mesh->mNormals[k].z;
		}
		normalIndex += 3 * current_mesh->mNumVertices;
	}
}

void ProcessNode(MeshData& meshdata, aiNode* node)
{
	// BFS to traverse the tree
	// Use two queues to do BFS
	std::queue<aiNode*> node_q;
	node_q.push(node);

	while (node_q.size() != 0)
	{
		auto last = node_q.front();
		node_q.pop();
		// Process last
		for (unsigned int i = 0; i < last->mNumMeshes; ++i)
		{
			ProcessMesh(meshdata.mScene->mMeshes[last->mMeshes[i]], last);
		}

		// Add children nodes
		std::for_each(last->mChildren, last->mChildren + last->mNumChildren, [&](aiNode* n)
		{
			node_q.push(n);
		});
	}
}
