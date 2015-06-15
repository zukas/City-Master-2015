#include "model.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "gldebugger.h"


glm::mat4 __basic_model_matrix_func(const transform &t, model_data)
{
	return t;
}

Model::~Model()
{
	ColourID::releaseID(m_id);
}

Model::Model(Model &&other) :
	m_modelMatrix(std::move(other.m_modelMatrix)),
	m_preprocFunc(other.m_preprocFunc),
	m_postprocFunc(other.m_postprocFunc),
	m_data(other.m_data),
	m_children(std::move(other.m_children)),
	m_meshes(std::move(other.m_meshes)),
	m_animations(std::move(other.m_animations)),
	m_id(other.m_id),
	m_selectable(other.m_selectable),
	m_selected(other.m_selected)
{
	other.m_preprocFunc = __basic_model_matrix_func;
	other.m_postprocFunc = nullptr;
	other.m_data = nullptr;
	other.m_id = 0;
	other.m_selected = false;
}

Model::Model(const Model &other) :
	m_modelMatrix(other.m_modelMatrix),
	m_preprocFunc(other.m_preprocFunc),
	m_postprocFunc(other.m_postprocFunc),
	m_data(other.m_data),
	m_children(other.m_children),
	m_meshes(other.m_meshes),
	m_animations(other.m_animations),
	m_id(ColourID::aquireID()),
	m_selectable(other.m_selectable),
	m_selected(false)
{
}

Model::Model(std::vector<Mesh> meshes) :
	m_meshes(std::move(meshes)),
	m_id(ColourID::aquireID())
{
}

void Model::addChild(Model &&child)
{
	m_children.push_back(std::forward<Model>(child));
}

void Model::setLocation(const glm::vec3 &loc)
{
	m_modelMatrix.loc = loc;// = glm::translate(m_modelMatrix, axis);
}

void Model::setRotation(const glm::quat &rot)
{
	m_modelMatrix.rot = rot;//  = glm::rotate(m_modelMatrix, radiants, axis);
}

void Model::setScale(const glm::vec3 &scale)
{
	m_modelMatrix.scale = scale;// = glm::scale(m_modelMatrix, axis);
}

void Model::setModelPreProcFunction(model_matrix_preproc_func func)
{
	m_preprocFunc = func;
}

void Model::setModelPostProcFunction(model_matrix_postproc_func func)
{
	m_postprocFunc = func;
}

void Model::setModelData(model_data data)
{
	m_data = data;
}

void Model::setSelectable(bool isSelectable)
{
	m_selectable = isSelectable;
}

void Model::select(bool selected)
{
	m_selected = selected;
}

bool Model::selected() const
{
	return m_selected;
}

int Model::meshCount() const
{
	return m_meshes.size();
}

Mesh *Model::mesh(int index)
{
	return index >= 0 && index < meshCount() ? &m_meshes[index] : nullptr;
}

void Model::addAnimation(Animation animation)
{
	m_animations.push_back(std::move(animation));
}

void Model::setAnimation(int id)
{
	m_animIndex = id >= 0 && id < (int)m_animations.size() ? id : -1;
	if(m_animIndex != -1)
	{
		m_animations[m_animIndex].prepare();
	}
	for(auto &m : m_children)
	{
		m.setAnimation(id);
	}
}

int Model::animationCount() const
{
	return m_animations.size();
}

Animation *Model::animation(int index)
{
	return index >= 0 && index < animationCount() ? &m_animations[index] : nullptr;
}

void Model::render(Program &program, const glm::mat4 &parent_transform)
{
	if(program.type() == Selection)
	{
		if(!m_selectable) return;
		program.setObjectID(m_id);
	}
	else
	{
		program.setSelected(m_selected);
	}

	glm::mat4 tmp = m_preprocFunc(m_modelMatrix, m_data) ;// glm::translate(m_modelMatrix, glm::vec3(parent_transform[3]));
//	glm::mat4 transform = tmp;

//	if(m_animIndex != -1 && (int)m_animations.size() > m_animIndex)
//	{
//		tmp = m_animations[m_animIndex].transform(tmp, parent_transform);
//	}

//	tmp = parent_transform * tmp;

//	tmp = glm::translate(glm::translate(tmp, -glm::vec3(tmp[3])), glm::vec3(transform[3]));

	for(auto &m : m_children)
	{
		m.render(program, parent_transform * tmp);
	}

	if(m_postprocFunc)
	{
		tmp = m_postprocFunc(tmp, m_data);
	}
	tmp = parent_transform * tmp;

//	tmp *= parent_transform;
//	tmp = glm::translate(tmp, glm::vec3(parent_transform[3]));

	program.setModelMatrix(tmp);

	for(auto &m : m_meshes)
	{
		m.render(program);
	}

}

//void Model::render(Program &program, const Transformation &parent_transform, const glm::mat4 &parent_matrix)
//{
//	if(program.type() == Selection)
//	{
//		if(!m_selectable) return;
//		program.setObjectID(m_id);
//	}
//	else
//	{
//		program.setSelected(m_selected);
//	}

//	Transformation tmp { glm::vec3(m_modelMatrix[3]), {}, {}, { 1.f, 1.f, 1.f } };

//	if(m_animIndex != -1 && (int)m_animations.size() > m_animIndex)
//	{
//		tmp = m_animations[m_animIndex].transform(tmp, parent_transform);
//	}

////	tmp = glm::translate(glm::translate(tmp, -glm::vec3(tmp[3])), glm::vec3(transform[3]));
//	glm::mat4 model(1.f);

//	glm::mat4 translate_to_parent_space = glm::translate(model, parent_transform.location);
//	glm::mat4 translate_to_model_space = glm::translate(model, -parent_transform.location);
//	glm::mat4 rot_parent_space = glm::eulerAngleYXZ(tmp.parent_space_euler_angle.y, tmp.parent_space_euler_angle.x, tmp.parent_space_euler_angle.z);
//	glm::mat4 rot_model_space = glm::eulerAngleYXZ(tmp.model_space_euler_angle.y, tmp.model_space_euler_angle.x, tmp.model_space_euler_angle.z);
//	glm::mat4 mod = glm::translate(glm::scale(model, tmp.scale), parent_transform.location + tmp.location);

//	model = translate_to_parent_space * rot_parent_space * mod * translate_to_model_space * rot_model_space;


//	for(auto &m : m_children)
//	{
//		m.render(program, tmp, model);
//	}
////	tmp *= parent_transform;
////	tmp = glm::translate(tmp, glm::vec3(parent_transform[3]));



//	program.setModelMatrix(model);

//	for(auto &m : m_meshes)
//	{
//		m.render(program);
//	}

//}

Model &Model::operator = (Model &&other)
{
	m_modelMatrix = std::move(other.m_modelMatrix);
	m_preprocFunc = other.m_preprocFunc;
	m_postprocFunc = other.m_postprocFunc;
	m_data = other.m_data;
	m_children = std::move(other.m_children);
	m_meshes = std::move(other.m_meshes);
	m_animations = std::move(other.m_animations);
	m_id = other.m_id;
	m_selectable = other.m_selectable;
	m_selected = other.m_selected;

	other.m_preprocFunc = __basic_model_matrix_func;
	other.m_postprocFunc = nullptr;
	other.m_data = nullptr;
	other.m_id = 0;
	other.m_selected = false;
	return *this;
}

Model &Model::operator = (const Model &other)
{
	ColourID::releaseID(m_id);

	m_modelMatrix = other.m_modelMatrix;
	m_preprocFunc = other.m_preprocFunc;
	m_postprocFunc = other.m_postprocFunc;
	m_data = other.m_data;
	m_children = other.m_children;
	m_meshes = other.m_meshes;
	m_animations = other.m_animations;
	m_id = ColourID::aquireID();
	m_selectable = other.m_selectable;
	m_selected = false;

	return *this;
}

bool Model::operator == (int id) const
{
	return m_id == id;
}

