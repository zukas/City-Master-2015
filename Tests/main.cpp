#include "glprogram.h"
#include "colourid.h"

#include "glm/glm.hpp"

int main()
{


    glProgram p;
    p.exec();

//    float size = 250.f;

//    glm::vec3 vertexes [24] =
//    {
//        // Front face
//        glm::vec3(size, size, size), glm::vec3(size, -size, size), glm::vec3(-size, size, size), glm::vec3(-size, -size, size),
//        // Back face
//        glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, size, -size), glm::vec3(size, -size, -size),
//        // Left face
//        glm::vec3(-size, size, size), glm::vec3(-size, -size, size), glm::vec3(-size, size, -size), glm::vec3(-size, -size, -size),
//        // Right face
//        glm::vec3(size, size, -size), glm::vec3(size, -size, -size), glm::vec3(size, size, size), glm::vec3(size, -size, size),
//        // Top face
//        glm::vec3(-size, size, -size), glm::vec3(size, size, -size), glm::vec3(-size, size, size), glm::vec3(size, size, size),
//        // Bottom face
//        glm::vec3(size, -size, -size), glm::vec3(-size, -size, -size), glm::vec3(size, -size, size), glm::vec3(-size, -size, size),
//    };

//    glm::vec3 normals[6] =
//    {
//        glm::vec3(0.0f, 0.0f, -1.0f),
//        glm::vec3(0.0f, 0.0f, 1.0f),
//        glm::vec3(1.0f, 0.0f, 0.0f),
//        glm::vec3(-1.0f, 0.0f, 0.0f),
//        glm::vec3(0.0f, -1.0f, 0.0f),
//        glm::vec3(0.0f, 1.0f, 0.0f)
//    };

//    glm::vec2 uvs[4] =
//    {
//        glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
//    };

//    std::string out_v;
//    std::string out_n;
//    std::string out_u;


//    for(int  i = 0; i < 24; ++i)
//    {

//        glm::vec3 v_tmp = vertexes[i];
//        char v_buf[128] { 0 };
//        int v_size = std::sprintf(v_buf, "%s, %s, %s,\n", v_tmp.x > 0 ? "size" : "-size" , v_tmp.y > 0 ? "size" : "-size", v_tmp.z > 0 ? "size" : "-size");
//        out_v += std::string(v_buf, v_size);

//        glm::vec3 n_tmp = normals[i / 4];
//        char n_buf[128] { 0 };
//        int n_size = std::sprintf(n_buf, "%.2ff, %.2ff, %.2ff,\n", n_tmp.x, n_tmp.y, n_tmp.z);
//        out_n += std::string(n_buf, n_size);


//        glm::vec2 u_tmp = uvs[i % 4];
//        char u_buf[128] { 0 };
//        int u_size = std::sprintf(u_buf, "%.2ff, %.2ff,\n", u_tmp.x, u_tmp.y);
//        out_u += std::string(u_buf, u_size);

//    }
//    printf("%s\n\n%s\n\n%s", out_v.c_str(), out_n.c_str(), out_u.c_str());

    return 0;
}
