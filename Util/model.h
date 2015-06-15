#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "mesh.h"
#include "colourid.h"


typedef void* model_data;
typedef glm::mat4 (*model_matrix_preproc_func)(const transform &t, model_data data);
typedef glm::mat4 (*model_matrix_postproc_func)(const glm::mat4 &t, model_data data);

glm::mat4 __basic_model_matrix_func(const transform &t, model_data);

class Model
{
	private:
		transform m_modelMatrix {};
		model_matrix_preproc_func m_preprocFunc { __basic_model_matrix_func };
		model_matrix_postproc_func m_postprocFunc { nullptr };
		model_data m_data { nullptr };

		std::vector<Model> m_children;
		std::vector<Mesh> m_meshes;

		std::vector<Animation > m_animations;
		int m_animIndex { -1 };

		ColourID m_id { 0 };
		bool m_selectable { true };
		bool m_selected { false };

	public:
		Model() = default;
		~Model();
		Model(Model &&other);
		Model(const Model &other);
		Model(std::vector<Mesh > meshes);

		void addChild(Model &&child);
		void setLocation(const glm::vec3 &loc);
		void setRotation(const glm::quat &rot);
		void setScale(const glm::vec3 &scale);
		void setModelPreProcFunction(model_matrix_preproc_func func);
		void setModelPostProcFunction(model_matrix_postproc_func func);
		void setModelData(model_data data);
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
//		void render(Program &program, const Transformation &parent_transform, const glm::mat4 &parent_matrix = glm::mat4(1.f));

		Model &operator = (Model &&other);
		Model &operator = (const Model &other);

		bool operator == (int id) const;

};

#endif // MODEL_H
