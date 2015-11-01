#include "box2d.h"
#include "Util/types.h"
#include "Util/mesh_2d.h"

#include <GL/glew.h>
#include <GL/gl.h>

Box2d::Box2d() {}

void Box2d::init(uint32_t texture_id, float height, float width) {
    m_height = height;
    m_width = width;

    uv_2d_vertex data[6]{{{0.f, 0.f}, {0.f, 0.f}},
                         {{width, 0.f}, {1.f, 0.f}},
                         {{0.f, height}, {0.f, 1.f}},
                         {{width, height}, {1.f, 1.f}},
                         {{0.f, height}, {0.f, 1.f}},
                         {{width, 0.f}, {1.f, 0.f}}};

}

void Box2d::render(float x, float y) {
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glBindVertexArray(m_vertexArray);

//    //    m_program.use();
//    //    m_program.setSampers(1);

//    //    auto viewport = m_program.getViewport();
//    //    m_program.setProjectionMatrix(
//    //        glm::ortho(0.0f, float(viewport.w), 0.0f, float(viewport.h)));
//    //    m_texture.bind(0);
//    glm::mat4 model =
//        glm::translate(glm::mat4(1.0f), glm::vec3(float(x), float(y), 0.0f));

//    m_program.setModelMatrix(model);
    mesh_2d::render_geometry(m_mesh, 6);

    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}
