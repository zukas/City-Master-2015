#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "mesh.h"
#include "colourid.h"

class Model
{
    private:
        std::vector<Mesh> m_meshes;
        ColourID m_id { 0 };
        bool m_selectable { true };
        bool m_selected { false };

    public:
        Model();
        ~Model();
        Model(Model &&other);
        Model(std::vector<Mesh > meshes);

        void translate(glm::vec3 axis);
        void rotate(float radiants, glm::vec3 axis);
        void scale(glm::vec3 axis);
        void setAnimation(int id);
        void setSelectable(bool isSelectable = false);
        void select(bool selected = true);
        bool selected() const;

        void render(Program &program);

        Model &operator = (Model &&other);

        bool operator == (int id) const;

        Model(const Model&) = delete;
        Model &operator = (const Model&) = delete;
};

#endif // MODEL_H
