#ifndef MESH2D_H
#define MESH2D_H

#include "types.h"
#include "texture.h"
#include "program.h"
#include "animation.h"
#include <glm/glm.hpp>
#include <memory>

class Mesh2d
{
    private:
        GLID m_vertexArray { 0 };
        size_t m_dataSize;
        std::vector<GLID> m_indexes;
        std::vector<Texture > m_textures;

        void cleanUp();

    public:
        Mesh2d() = delete;
        Mesh2d(Mesh2d &&other);
        Mesh2d(const Mesh2d &other);
        Mesh2d(size_t dataSize);
        ~Mesh2d();
        void bindDataIndexed(std::vector<vertex_2d> &data);
        void bindDataNotIndexed(std::vector<vertex_2d> &data);
        void addTexture(Texture texture);
        int textureCount() const;

        void renderIndexed(Program &program);
        void renderNotIndexed(Program &program);

        Mesh2d &operator = (Mesh2d &&other);
        Mesh2d &operator = (const Mesh2d &other);

};
#endif // MESH2D_H

