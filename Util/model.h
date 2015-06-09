#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "mesh.h"
#include "colourid.h"

class Model
{
	private:
		glm::mat4 m_modelMatrix;

		std::vector<Model> m_children;
		std::vector<Mesh> m_meshes;

		std::vector<Animation > m_animations;
		int m_animIndex { -1 };

		ColourID m_id { 0 };
		bool m_selectable { true };
		bool m_selected { false };

	public:
		Model();
		~Model();
		Model(Model &&other);
		Model(const Model &other);
		Model(std::vector<Mesh > meshes);

		void addChild(Model &&child);
		void translate(glm::vec3 axis);
		void rotate(float radiants, glm::vec3 axis);
		void scale(glm::vec3 axis);
		void setSelectable(bool isSelectable = false);
		void select(bool selected = true);
		bool selected() const;
		int meshCount() const;
		Mesh *mesh(int index);
		void addAnimation(Animation animation);
		void setAnimation(int id);
		int animationCount() const;
		Animation *animation(int index);


		void render(Program &program, const glm::mat4 &parent_transform = glm::mat4(1.f));

		Model &operator = (Model &&other);
		Model &operator = (const Model &other);

		bool operator == (int id) const;

};

#endif // MODEL_H
