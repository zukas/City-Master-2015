#ifndef MESH_H
#define MESH_H

#include "types.h"
#include "texture.h"
#include "program.h"
#include "animation.h"
#include <glm/glm.hpp>
#include <memory>

class Mesh
{
    public:
        enum Shape { TRIANGLE = 0, LINE = 1  };
        enum Type { UV = 0, COLOUR = 1  };

    private:
        GLID m_vertexArray { 0 };
    public:
        glm::mat4 modelMatrix;
    private:
        std::vector<Texture > m_textures;
        std::vector<Animation > m_animations;
        int m_animIndex { -1 };
        int m_size { 0 };
        Shape m_shape;
        Type m_type;
        std::shared_ptr<struct Private > d_ptr { nullptr };

    private:
        void bindData();
        void cleanUp();

    public:
        Mesh();
        Mesh(Mesh &&other);
        Mesh(const Mesh &other);
        Mesh(std::vector<glm::vec3 > vertex, std::vector<glm::vec3 > normals, std::vector<glm::vec2 > uvs);
        Mesh(std::vector<glm::vec3 > vertex, std::vector<glm::vec3 > colours, Shape shape = TRIANGLE);
        ~Mesh();

        void addTexture(Texture texture);
        void addAnimation(Animation &&animation);
        void setAnimation(int id);
        void render(Program &program);


        Mesh &operator = (Mesh &&other);
        Mesh &operator = (const Mesh &other);

};

#endif // MESH_H
