#include "saturnrings.h"

auto u_vs = VS(
	layout (location = 0) in vec3 in_position;
	layout (location = 1) in float in_radius;

	out vec3 position_vs;
	out float radius_vs;

	void main()
	{
		position_vs = in_position;
		radius_vs = in_radius;
	}
);
//	 position_vs[0];

auto u_gs = GS(
	layout(points) in;
	layout(points) out;
	layout(max_vertices = 30) out;

	in vec3 position_vs[];
	in float radius_vs[];

	out vec3 position;
	out float radius;

	uniform float current_time;

	#define PI 3.14159
	#define PI2 PI * 2.0

	void main()
	{
		position = position_vs[0];
		radius = radius_vs[0];
		EmitVertex();
		EndPrimitive();

		for(int i = 1; i <= 24; ++i)
		{
			float theta = (PI2 * 24 / float(i)) * (current_time / 100000.0);
			float x = radius * cos(theta);
			float y = radius * sin(theta);
			position = vec3(x, 0.0, y);
			radius = radius + i;
			EmitVertex();
			EndPrimitive();
		}
	}
);

auto r_vs = VS(
	layout (location = 0) in vec3 in_position;
	layout (location = 1) in float in_radius;

	out vec3 position_vs;
	out float radius_vs;

	void main()
	{
		gl_Position = vec4(in_position, 1.0);
		position_vs = in_position;
		radius_vs = in_radius;
	}
);

auto r_gs =
	"#version 330\n"
	"layout(points) in;"
	"layout(triangle_strip) out;"
	"layout(max_vertices = 4) out;"

	"in vec3 position_vs[];"

	"smooth out vec2 uv_gs;"
	"smooth out vec4 colour_gs;"
	"uniform mat4 viewMatrix;"
	"uniform mat4 projectionMatrix;"
	"uniform mat4 modelMatrix;"

	"uniform vec3 vQuad1;"
	"uniform vec3 vQuad2;"
	"uniform vec3 colour;"
	"uniform float size;"

	"void main()"
	"{"
		"vec3 pos = gl_in[0].gl_Position.xyz;"
		"mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;"

		"colour_gs = vec4(colour, 1.0);"

		"vec3 pos_gs = pos + ((-vQuad1-vQuad2) * size);"
		"uv_gs = vec2(0.0, 0.0);"
		"gl_Position = MVP * vec4(pos_gs, 1.0);"
		"EmitVertex();"

		"pos_gs = pos + ((-vQuad1+vQuad2) * size);"
		"uv_gs = vec2(0.0, 1.0);"
		"gl_Position = MVP * vec4(pos_gs, 1.0);"
		"EmitVertex();"

		"pos_gs = pos + ((vQuad1-vQuad2) * size);"
		"uv_gs = vec2(1.0, 0.0);"
		"gl_Position = MVP * vec4(pos_gs, 1.0);"
		"EmitVertex();"

		"pos_gs = pos + ((vQuad1+vQuad2) * size);"
		"uv_gs = vec2(1.0, 1.0);"
		"gl_Position = MVP * vec4(pos_gs, 1.0);"
		"EmitVertex();"

		"EndPrimitive();"
	"}"_gs;

auto r_fs =
	"#version 330\n"
	"uniform sampler2D tsampler[1];"
	"smooth in vec2 uv_gs;"
	"smooth in vec4 colour_gs;"

	"out vec4 FragColor;"

	"void main()"
	"{"
		"vec4 tcolour = texture2D(tsampler[0], uv_gs);"
		"FragColor = tcolour * colour_gs;"
		"float alpha = 0.0;"
		""
		"for(float i = 0.001; i < 1.0; i+=i)"
		"{"
			"if(FragColor.r > i && FragColor.g > i && FragColor.b > i)"
			"{"
				"alpha = i;"
			"}"
		"}"
		"if(alpha < 0.1)"
		"{"
			"discard;"
		"}"
		"FragColor.a *= alpha;"
	"}"_fs;

PARTICLE_FIELD(position, float[3]);
PARTICLE_FIELD(radius, float);

using particle = Particle<position_field_t, radius_field_t>::particle_t;
typedef std::vector<particle> particles;

void update_uniforms(UniformManager &)
{

}

void render_uniforms(UniformManager &m)
{
	m.setUniform("modelMatrix", glm::translate(glm::mat4(1.f), { 10.f, 0.f, 0.f}));
	m.setUniform("colour", { 0.95f, 0.85f, 0.85f });
	m.setUniform("size", 2.f);
}

SaturnRings::SaturnRings()
{

}

void SaturnRings::init()
{
	m_system.setUpdateShaders(u_vs, u_gs);
	m_system.setRenderShaders(r_vs, r_gs, r_fs);
//	m_system.addTexture({GL_TEXTURE_2D,  });
	m_system.init(15000, particles{ { position_field_t { 5.f, 5.f, 5.f }, radius_field_t { 5.f } } });

}

void SaturnRings::render(Camera &c)
{
	m_system.update(update_uniforms);
	m_system.render(c, render_uniforms);
}

