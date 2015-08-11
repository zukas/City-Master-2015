#include "loader.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"
#include "assimp/Logger.hpp"

#include "Util/mesh.h"
#include "Util/utils.h"

#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <cmath>

Model processNode(const aiScene *scene, aiNode *node, glm::mat4 parentTransform);
Mesh processMesh(const aiScene *scene, aiMesh *mesh);

glm::mat4 convertMatrix(const aiMatrix4x4 &m);
glm::mat4 convertMatrix(const glm::mat4 &m);
glm::mat4 createScaleMatrix(float x, float y, float z);
glm::mat4 createTranslateMatrix(float x, float y, float z);

Model load(const std::string &file)
{
//    std::vector<Mesh > meshes;
    Assimp::Importer i;
    const aiScene *scene = i.ReadFile(file, aiProcess_CalcTangentSpace |
                                      aiProcess_Triangulate |
                                      aiProcess_GenSmoothNormals |
                                      aiProcess_FlipUVs |
                                      aiProcess_JoinIdenticalVertices |
                                      aiProcess_SortByPType);
	Model m;
    if(scene)
    {
		m = processNode(scene, scene->mRootNode, glm::mat4(1.f));
    }
    i.FreeScene();

	return m;
}


Model processNode(const aiScene *scene, aiNode *node, glm::mat4 parentTransform)
{
	std::vector<Mesh> meshes(node->mNumMeshes);
    for(unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        Mesh mesh = processMesh(scene, scene->mMeshes[node->mMeshes[i]]);
		meshes[i] = std::move(mesh);
    }
	Model m { std::move(meshes) };

//	mesh = attachAnimation(scene, node, std::move(mesh), std::move(parentTransform));
    for(unsigned i = 0; i <node->mNumChildren; ++i)
    {
		m.addChild(processNode(scene, node->mChildren[i], convertMatrix(node->mTransformation) * parentTransform));
    }
	return m;
}

Mesh processMesh(const aiScene *scene, aiMesh *mesh)
{
    int faceCount = mesh->mNumFaces;
	std::vector<uv_vertex > points(faceCount * 3);


    for(int n = 0; n < faceCount; ++n)
    {
        const aiFace& face = mesh->mFaces[n];
        for(int m = 0; m < 3; ++m)
        {
			vec3 vert { mesh->mVertices[face.mIndices[m]].x, mesh->mVertices[face.mIndices[m]].y, mesh->mVertices[face.mIndices[m]].z };
			vec3 norm { 1.f, 1.f, 1.f };
            if(mesh->HasNormals())
            {
                norm = { mesh->mNormals[face.mIndices[m]].x, mesh->mNormals[face.mIndices[m]].y, mesh->mNormals[face.mIndices[m]].z };
            }
			vec2 UV { mesh->mTextureCoords[0][face.mIndices[m]].x, mesh->mTextureCoords[0][face.mIndices[m]].y };

            points[n * 3 + m] = { std::move(vert), std::move(norm), std::move(UV) };
        }
    }


    Mesh res { std::move(points) };

    if(scene->mNumMaterials > mesh->mMaterialIndex)
    {
        aiMaterial *m = scene->mMaterials[mesh->mMaterialIndex];
        aiString path;
        m->GetTexture(aiTextureType_DIFFUSE, 0, &path);
        std::string _path = std::string("textures/") + path.C_Str();
        std::fstream file(_path);
        if(file)
        {
            file.close();
            Texture t(GL_TEXTURE_2D, _path);
            res.addTexture(std::move(t));
        }
        else
        {
            Texture t(GL_TEXTURE_2D, std::string("textures/failed.png"));
            res.addTexture(std::move(t));
        }
    }
    else
    {
        Texture t(GL_TEXTURE_2D, std::string("textures/failed.png"));
        res.addTexture(std::move(t));
    }

    return res;
}


glm::mat4 convertMatrix(const aiMatrix4x4 &m)
{
    return  {   m.a1, m.b1, m.c1, m.d1,
                m.a2, m.b2, m.c2, m.d2,
                m.a3, m.b3, m.c3, m.d3,
                m.a4, m.b4, m.c4, m.d4,
    };
}

glm::mat4 convertMatrix(const glm::mat4 &m)
{
	return  {   m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]
	};
}


glm::mat4 createScaleMatrix(float x, float y, float z)
{
    return {    x, 0.f, 0.f, 0.f,
                0.f, y, 0.f, 0.f,
                0.f, 0.f, z, 0.f,
                0.f, 0.f, 0.f, 1.f
    };
}

glm::mat4 createTranslateMatrix(float x, float y, float z)
{
    return {    1.f, 0.f, 0.f, x,
                0.f, 1.f, 0.f, y,
                0.f, 0.f, 1.f, z,
                0.f, 0.f, 0.f, 1.f
    };
}
