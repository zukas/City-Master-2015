#include "skybox.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


constexpr float size { 1.f };


constexpr float vertexs[108]
{
	 size, -size, -size,  size, -size,  size, size,  size,  size, size,  size,  size, size,  size, -size, size, -size, -size,
	-size, -size,  size, -size, -size, -size,-size,  size, -size,-size,  size, -size,-size,  size,  size,-size, -size,  size,
	-size,  size, -size,  size,  size, -size, size,  size,  size, size,  size,  size,-size,  size,  size,-size,  size, -size,
	-size, -size,  size,  size, -size,  size, size, -size, -size, size, -size, -size,-size, -size, -size,-size, -size,  size,
	 size, -size,  size, -size, -size,  size,-size,  size,  size,-size,  size,  size, size,  size,  size, size, -size,  size,
	-size, -size, -size,  size, -size, -size, size,  size, -size, size,  size, -size,-size,  size, -size,-size, -size, -size
};


constexpr float t = (1 + sqrt(5.0))/2.0;
constexpr float s = sqrt(1 + t*t);

//constexpr float coords
//{
//	t, 1, 0
//};



constexpr auto vs = VS(
						layout(location = 0) in vec3 inPosition;
						uniform mat4 modelMatrix;
						uniform mat4 viewMatrix;
						uniform mat4 projectionMatrix;

						smooth out vec3 UV;

						void main()
						{
							mat4 inverseProjection = inverse(projectionMatrix);
							mat3 inverseModelview = transpose(mat3(modelMatrix * viewMatrix));
							vec4 unprojected = inverseProjection * vec4(inPosition, 1.0);

							UV = inverseModelview * unprojected.xyz;

							gl_Position = vec4(inPosition, 1.0);
						}
);

constexpr auto fs = FS(
						uniform samplerCube tsampler[1];
						smooth in vec3 UV;
						out vec4 outColour;

						void main()
						{
							outColour = texture(tsampler[0], UV);
						}
);

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(const std::string &right, const std::string &left, const std::string &bottom, const std::string &top, const std::string &front, const std::string &back) :
	m_texture(right, left, bottom, top, front, back)
{

	m_texture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_texture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_texture.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);
	GLID buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs) ,vertexs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
				0,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				nullptr           // array buffer offset
				);


	glBindVertexArray(0);

	m_program.createShader(vs);
	m_program.createShader(fs);
	m_program.createProgram();
	m_program.linkProgram();
	m_program.resolveUniforms();

	m_model = glm::scale(glm::mat4(1.f), glm::vec3(2000.f, 2000.f, 2000.f));
}

SkyBox::~SkyBox()
{

}

void SkyBox::render(Camera &camera)
{
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	glCullFace(GL_FRONT);
	glDepthMask (GL_FALSE);
	m_program.use();
	glBindVertexArray(m_vertexArray);
	camera.update(m_program);
	m_program.setModelMatrix(m_model);
	m_program.setSampers(1);
	m_texture.bind(0);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexs));
	glDepthMask (GL_TRUE);
	glCullFace(OldCullFaceMode);
}

