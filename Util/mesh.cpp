#include "mesh.h"
#include "clock.h"
#include "gldebugger.h"
#include "profiler.h"
#include <GL/glew.h>


void Mesh::bindData()
{
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	GLID buffer[2];
	glGenBuffers(m_indexes.empty() ? 1 : 2, buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(uv_vertex), &m_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				sizeof(uv_vertex),                  // stride
				0           // array buffer offset
				);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
				1,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				sizeof(uv_vertex),                  // stride
				reinterpret_cast<void* >(sizeof(vec3))            // array buffer offset
				);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
				2,                  // attribute
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				sizeof(uv_vertex),                  // stride
				reinterpret_cast<void* >(sizeof(vec3)  * 2)            // array buffer offset
				);
	if(!m_indexes.empty())
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer[1] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * sizeof(GLID), &m_indexes[0], GL_STATIC_DRAW );

	}
	glBindVertexArray(0);
}

void Mesh::cleanUp()
{
	glDeleteVertexArrays(1, &m_vertexArray);
	m_vertexArray = 0;
}

Mesh::Mesh()
{

}

Mesh::Mesh(Mesh &&other) :
	m_vertexArray(other.m_vertexArray),
	m_data(std::move(other.m_data)),
	m_indexes(std::move(other.m_indexes)),
	m_textures(std::move(other.m_textures))
{

	other.m_vertexArray = 0;
}

Mesh::Mesh(const Mesh &other) :
	m_data(other.m_data),
	m_indexes(other.m_indexes),
	m_textures(other.m_textures)
{
	bindData();
}

Mesh::Mesh(std::vector<uv_vertex> &&data) :
	m_data(std::move(data))
{
	bindData();
}

Mesh::Mesh(shape &&s) :
	m_data(std::move(s.data)),
	m_indexes(std::move(s.indexes))
{
	bindData();
}

Mesh::~Mesh()
{
	cleanUp();
}

void Mesh::addTexture(Texture texture)
{
	m_textures.push_back(std::move(texture));
}

int Mesh::textureCount() const
{
	return m_textures.size();
}


void Mesh::render(Program &program)
{
    PROF;
	glBindVertexArray(m_vertexArray);

	if(program.type() != Selection)
	{
		unsigned text_c =  m_textures.size();
		program.setSampers(text_c);
		for(unsigned i = 0; i < text_c; ++i)
		{
			m_textures[i].bind(i);
		}
	}

	if(m_indexes.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, m_data.size());
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, nullptr);
	}


	if(program.type() != Selection)
	{
		unsigned text_c =  m_textures.size();
		program.setSampers(text_c);
		for(unsigned i = 0; i < text_c; ++i)
		{
			m_textures[i].unbind(i);
		}
	}

	glBindVertexArray(0);

}


Mesh &Mesh::operator = (Mesh &&other)
{
	cleanUp();

	m_vertexArray = other.m_vertexArray;
	m_data = std::move(other.m_data);
	m_indexes = std::move(other.m_indexes);
	m_textures = std::move(other.m_textures);

	other.m_vertexArray = 0;

	return *this;
}

Mesh &Mesh::operator = (const Mesh &other)
{
	cleanUp();

	m_data = other.m_data;
	m_indexes = other.m_indexes;
	m_textures = other.m_textures;

	bindData();

	return *this;
}
