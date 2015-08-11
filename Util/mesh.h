#ifndef MESH_H
#define MESH_H

#include "types.h"
#include "texture.h"
#include "program.h"

#include <glm/glm.hpp>
#include <memory>

class Mesh
{
    private:
        GLID m_vertexArray { 0 };

    private:
		std::vector<uv_vertex> m_data;
		std::vector<GLID> m_indexes;
		std::vector<Texture > m_textures;
    private:
        void bindData();
        void cleanUp();

    public:
        Mesh();
        Mesh(Mesh &&other);
        Mesh(const Mesh &other);
		Mesh(std::vector<uv_vertex> &&data);
		Mesh(shape &&s);
        ~Mesh();

		void addTexture(Texture texture);
		int textureCount() const;
//        void addAnimation(Animation &&animation);
//        void setAnimation(int id);
//		int animationCount() const;
//		Animation *animation(int index);

		void render(Program &program);



        Mesh &operator = (Mesh &&other);
        Mesh &operator = (const Mesh &other);

};

#endif // MESH_H
