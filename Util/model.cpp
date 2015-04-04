#include "model.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Model::Model()
{
}

Model::~Model()
{
    ColourID::releaseID(m_id);
}

Model::Model(Model &&other) :
    m_meshes(std::move(other.m_meshes)),
    m_id(other.m_id),
    m_selectable(other.m_selectable),
    m_selected(other.m_selected)
{
    other.m_id = 0;
    other.m_selected = false;
}

Model::Model(const Model &other) :
    m_meshes(other.m_meshes),
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

void Model::translate(glm::vec3 axis)
{
    for(auto &m : m_meshes)
    {
        m.modelMatrix = glm::translate(m.modelMatrix, axis);
    }
}

void Model::rotate(float radiants, glm::vec3 axis)
{
    for(auto &m : m_meshes)
    {
        m.modelMatrix = glm::rotate(m.modelMatrix, radiants, axis);
    }
}

void Model::scale(glm::vec3 axis)
{
    for(auto &m : m_meshes)
    {
        m.modelMatrix = glm::scale(m.modelMatrix, axis);
    }
}

void Model::setAnimation(int id)
{
    for(auto &m: m_meshes)
    {
        m.setAnimation(id);
    }
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

void Model::render(Program &program)
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
    for(auto &m : m_meshes)
    {
        m.render(program);
    }
}

Model &Model::operator = (Model &&other)
{
    m_meshes = std::move(other.m_meshes);
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

    m_meshes = other.m_meshes;
    m_id = ColourID::aquireID();
    m_selectable = other.m_selectable;
    m_selected = false;

    return *this;
}

bool Model::operator == (int id) const
{
    return m_id == id;
}

