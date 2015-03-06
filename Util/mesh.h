#ifndef MESH_H
#define MESH_H

#include "types.h"
#include "texture.h"
#include "program.h"
#include "animation.h"
#include <glm/glm.hpp>

class Mesh
{
    public:
        enum Shape { TRIANGLE = 0, LINE = 1  };
        glm::mat4 modelMatrix;

    private:
        GLID m_vertexArray { 0 };
        std::vector<glm::vec3 > m_vertexes;
        std::vector<glm::vec3 > m_normals;
        std::vector<glm::vec3 > m_colours;
        std::vector<glm::vec2 > m_uvs;
        std::vector<Texture > m_textures;
        std::vector<Animation > m_animations;
        int m_animIndex { -1 };

        enum { UV = 0, COLOUR = 1  } m_type;
        Shape m_shape;

    private:
        void bindData();

    public:
        Mesh();
        Mesh(Mesh &&other);
        Mesh(std::vector<glm::vec3 > vertex, std::vector<glm::vec3 > normals, std::vector<glm::vec2 > uvs);
        Mesh(std::vector<glm::vec3 > vertex, std::vector<glm::vec3 > colours, Shape shape = TRIANGLE);
        ~Mesh();

        void addTexture(Texture texture);
        void addAnimation(Animation &&animation);
        void setAnimation(int id);
        void render(Program &program);


        Mesh &operator = (Mesh &&other);

        Mesh(const Mesh&) = delete;
        Mesh &operator = (const Mesh&) = delete;
};

#endif // MESH_H
