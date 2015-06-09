#include "model.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "gldebugger.h"

Model::Model() :
	m_modelMatrix(1.f)
{
}

Model::~Model()
{
	ColourID::releaseID(m_id);
}

Model::Model(Model &&other) :
	m_modelMatrix(std::move(other.m_modelMatrix)),
	m_children(std::move(other.m_children)),
	m_meshes(std::move(other.m_meshes)),
	m_animations(std::move(other.m_animations)),
	m_id(other.m_id),
	m_selectable(other.m_selectable),
	m_selected(other.m_selected)
{
	other.m_id = 0;
	other.m_selected = false;
}

Model::Model(const Model &other) :
	m_modelMatrix(other.m_modelMatrix),
	m_children(other.m_children),
	m_meshes(other.m_meshes),
	m_animations(other.m_animations),
	m_id(ColourID::aquireID()),
	m_selectable(other.m_selectable),
	m_selected(false)
{
}

Model::Model(std::vector<Mesh> meshes) :
	m_modelMatrix(1.f),
	m_meshes(std::move(meshes)),
	m_id(ColourID::aquireID())
{
}

void Model::addChild(Model &&child)
{
	m_children.push_back(std::forward<Model>(child));
}

void Model::translate(glm::vec3 axis)
{
	m_modelMatrix = glm::translate(m_modelMatrix, axis);
}

void Model::rotate(float radiants, glm::vec3 axis)
{
	m_modelMatrix = glm::rotate(m_modelMatrix, radiants, axis);
}

void Model::scale(glm::vec3 axis)
{
	m_modelMatrix = glm::scale(m_modelMatrix, axis);
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

	glm::mat4 tmp = m_modelMatrix * parent_transform;// glm::translate(m_modelMatrix, glm::vec3(parent_transform[3]));
//	glm::mat4 transform = tmp;

	if(m_animIndex != -1 && (int)m_animations.size() > m_animIndex)
	{
		tmp = m_animations[m_animIndex].transform(tmp, parent_transform);
	}

//	tmp = glm::translate(glm::translate(tmp, -glm::vec3(tmp[3])), glm::vec3(transform[3]));

	for(auto &m : m_children)
	{
		m.render(program, tmp);
	}
//	tmp *= parent_transform;
//	tmp = glm::translate(tmp, glm::vec3(parent_transform[3]));

	program.setModelMatrix(tmp);

	for(auto &m : m_meshes)
	{
		m.render(program);
	}

}

Model &Model::operator = (Model &&other)
{
	m_modelMatrix = std::move(other.m_modelMatrix);
	m_children = std::move(other.m_children);
	m_meshes = std::move(other.m_meshes);
	m_animations = std::move(other.m_animations);
	m_id = other.m_id;
	m_selectable = other.m_selectable;
	m_selected = other.m_selected;

	other.m_id = 0;
	other.m_selected = false;
	return *this;
}

Model &Model::operator = (const Model &other)
{
	ColourID::releaseID(m_id);

	m_modelMatrix = other.m_modelMatrix;
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

