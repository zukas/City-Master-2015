#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"
#include "texture.h"
#include <glm/glm.hpp>

class Object
{
    public:
        static constexpr int bufferCount { 3 };

    private:
        glm::mat4 m_modelMatrix;
        GLID m_vertexArray;
        GLID m_buffers[bufferCount];
        std::vector<glm::vec3 > m_vertexes;
        std::vector<glm::vec3 > m_normals;
        std::vector<glm::vec2 > m_uvs;


    public:
        Object(std::vector<glm::vec3 > &&vertex = {}, std::vector<glm::vec3 > &&normals = {}, std::vector<glm::vec2 > &&uvs = {});

        void render();

        Object(const Object&) = delete;
        Object &operator = (const Object&) = delete;
};

#endif // OBJECT_H
