#ifndef MESH_H
#define MESH_H

#include "types.h"
#include "texture.h"
#include "program.h"
#include "animation.h"
#include <glm/glm.hpp>
#include <memory>

struct MeshUVPoint
{
    glm::vec3 vertex {};
    glm::vec3 normal {};
    glm::vec2 uv {};

    MeshUVPoint() = default;
    MeshUVPoint(glm::vec3 &&vert, glm::vec3 &&norm, glm::vec2 &&UV) :
        vertex(std::move(vert)),
        normal(std::move(norm)),
        uv(std::move(UV)) {}
};

class Mesh
{
    private:
        GLID m_vertexArray { 0 };
    public:
        glm::mat4 modelMatrix;
    private:
        std::vector<MeshUVPoint> m_data;
        std::vector<Texture > m_textures;
        std::vector<Animation > m_animations;
        int m_animIndex { -1 };
    private:
        void bindData();
        void cleanUp();

    public:
        Mesh();
        Mesh(Mesh &&other);
        Mesh(const Mesh &other);
        Mesh(std::vector<MeshUVPoint> &&data);
        ~Mesh();

        void addTexture(Texture texture);
        void addAnimation(Animation &&animation);
        void setAnimation(int id);
        void render(Program &program);


        Mesh &operator = (Mesh &&other);
        Mesh &operator = (const Mesh &other);

};

#endif // MESH_H
