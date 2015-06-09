#include "axis.h"
#include <GL/glew.h>

constexpr float size { 3000.f };

constexpr float axisBuffer []
{
	-size, 0.0f, 0.0f,
	1.f, 0.f, 0.f,

	size, 0.0f, 0.0f,
	1.f, 0.f, 0.f,

	0.0f, -size, 0.0f,
	0.f, 1.f, 0.f,

	0.0f,  size, 0.0f,
	0.f, 1.f, 0.f,

	0.0f, 0.0f, -size,
	0.f, 0.f, 1.f,

	0.0f, 0.0f,  size,
	0.f, 0.f, 1.f
};

constexpr auto vs = VS(
						layout(location = 0) in vec3 inPosition;
						layout(location = 1) in vec3 inColour;

						uniform mat4 viewMatrix;
						uniform mat4 projectionMatrix;

						out vec3 colour;

						void main()
						{
							colour = inColour;
							mat4 MVP = (projectionMatrix * viewMatrix);
							gl_Position =  MVP * vec4(inPosition, 1.0);
						}
);

constexpr auto fs = FS(
						in vec3 colour;
						out vec4 outColour;

						void main()
						{
							outColour = vec4(colour, 1.0);
						}
);

Axis::Axis()
{
}

Axis::~Axis()
{
	glDeleteVertexArrays(1, &m_vertexArray);
	m_vertexArray = 0;
}

void Axis::init()
{
	if(m_vertexArray == 0)
	{
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		GLID buffer;
		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axisBuffer), axisBuffer, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
					0,                  // attribute
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					sizeof(float) * 6,                  // stride
					0           // array buffer offset
					);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
					1,                  // attribute
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					sizeof(float) * 6,                  // stride
					reinterpret_cast<void* >(sizeof(float) * 3)            // array buffer offset
					);

		glBindVertexArray(0);

		m_program.createShader(vs);
		m_program.createShader(fs);
		m_program.createProgram();
		m_program.linkProgram();
		m_program.resolveUniforms();
	}
}

void Axis::render(Camera &camera)
{
	if(m_vertexArray)
	{
		m_program.use();
		camera.update(m_program);
		glBindVertexArray(m_vertexArray);
		glDrawArrays(GL_LINES, 0, sizeof(axisBuffer));
		glBindVertexArray(0);
	}
}
