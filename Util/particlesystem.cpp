#include "particlesystem.h"
#include "clock.h"
#include "gldebugger.h"

#include <GL/glew.h>

//struct Particle
//{
//		glm::vec3 position;
//		glm::vec3 velocity;
//		glm::vec3 color;
//		float lifeTime;
//		float size;
//		float type;
//		float reuse;
//};

//constexpr int max_num_particles { 100000 };

//float grandf(float min, float max)
//{
//	float val = float((rand()%(RAND_MAX)) + 1.f)/float(RAND_MAX);
//	return min+max*val;
//}

//ParticleSystem::ParticleSystem()
//{
//	GLCLEAR();
//	m_programs[0].createShader("shaders/particles_update.vert", VERTEX);
//	GLCHECK();
//	m_programs[0].createShader("shaders/particles_update.geom", GEOMETRY);
//	GLCHECK();
//	m_programs[0].createProgram();
//	GLCHECK();
//	const char *varyings[7]
//	{
//		"vPositionOut",
//		"vVelocityOut",
//		"vColorOut",
//		"fLifeTimeOut",
//		"fSizeOut",
//		"iTypeOut",
//		"reuseOut"
//	};

//	glTransformFeedbackVaryings(m_programs[0].program(), 7, varyings, GL_INTERLEAVED_ATTRIBS);
//	GLCHECK();

//	m_programs[0].linkProgram();
//	GLCHECK();
//	m_programs[0].resolveUniforms();
//	GLCHECK();
//	m_programs[1].createShader("shaders/particles_render.vert", VERTEX);
//	GLCHECK();
//	m_programs[1].createShader("shaders/particles_render.geom", GEOMETRY);
//	GLCHECK();
//	m_programs[1].createShader("shaders/particles_render.frag", FRAGMENT);
//	GLCHECK();
//	m_programs[1].createProgram();
//	GLCHECK();
//	m_programs[1].linkProgram();
//	GLCHECK();
//	m_programs[1].resolveUniforms();
//	GLCHECK();

//	glGenBuffers(2, m_buffers);
//	GLCHECK();

//	glGenTransformFeedbacks(1, &m_transformBuffer);
//	GLCHECK();
//	glGenQueries(1, &m_query);
//	GLCHECK();
//	glGenVertexArrays(2, m_vertexArrayID);
//	GLCHECK();

////	Particle p;
////	p.type = 0.0f;
////	p.reuse = 0.0f;


//	for(int i = 0; i < 2; ++i)
//	{
//		glBindVertexArray(m_vertexArrayID[i]);
//		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * max_num_particles, nullptr, GL_DYNAMIC_DRAW);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &p);

//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)0); // Position
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12); // Velocity
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (const GLvoid*)24); // Color
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
//		glEnableVertexAttribArray(5);
//		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)44); // Type
//		glEnableVertexAttribArray(6);
//		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)48); // Reuse
//	}
//	m_bufferCursor = 0;
//	m_particleCount = 1;
//	m_startTime = Clock::getDuration();

//	m_random = { 4096 };

//}

//ParticleSystem::~ParticleSystem()
//{

//}

//int ParticleSystem::count() const
//{
//	return m_particleCount;
//}

//void ParticleSystem::update()
//{
//	GLCLEAR();
//	float timePassed = (Clock::getDuration() - m_startTime);
//	m_startTime = Clock::getDuration();

//	m_programs[0].use();
//	GLCHECK();
//	m_programs[0].setUniform("ttime", m_startTime);
//	GLCHECK();
//	m_programs[0].setUniform("fTimePassed", timePassed);
//	GLCHECK();
//	m_programs[0].setUniform("vGenPosition", m_genPosition);
//	GLCHECK();
//	m_programs[0].setUniform("vGenVelocityMin", m_minGenVelocity);
//	GLCHECK();
//	m_programs[0].setUniform("vGenVelocityRange", m_maxGenVelocity);
//	GLCHECK();
//	m_programs[0].setUniform("vGenColor", m_genColor);
//	GLCHECK();
//	m_programs[0].setUniform("vGenGravityVector", m_gravityGen);
//	GLCHECK();
//	m_programs[0].setUniform("fGenLifeMin", m_minGenLife);
//	GLCHECK();
//	m_programs[0].setUniform("fGenLifeRange", m_maxGenLife);
//	GLCHECK();
//	m_programs[0].setUniform("fGenSize", m_genSize);
//	GLCHECK();
//	m_programs[0].setUniform("fDim", m_radius);
//	GLCHECK();
//	m_programs[0].setUniform("iNumToGenerate", m_genCount);
//	GLCHECK();
//	m_elapsedTime += timePassed;

//	glm::vec3 vRandomSeed = glm::vec3(grandf(-1.0f,1.0f), grandf(0.f,.1f), grandf(-1.0f,1.0f));
//	m_programs[0].setUniform("vRandomSeed", vRandomSeed);
//	GLCHECK();

//	m_random.bind(0);

//	//    if(m_elapsedTime > m_nextGenTime)
//	//    {

//	//        m_programs[0].setUniform("iNumToGenerate", m_genCount);
//	//        GLCHECK();
//	//        m_elapsedTime -= m_nextGenTime;

//	//        glm::vec3 vRandomSeed = glm::vec3(grandf(-1.0f,1.0f), grandf(0.f,.1f), grandf(-1.0f,1.0f));
//	//        m_programs[0].setUniform("vRandomSeed", vRandomSeed);
//	//        GLCHECK();
//	//    }


//	glEnable(GL_RASTERIZER_DISCARD);
//	GLCHECK();
//	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformBuffer);
//	GLCHECK();
//	glBindVertexArray(m_vertexArrayID[m_bufferCursor]);
//	GLCHECK();
//	glEnableVertexAttribArray(1); // Re-enable velocity
//	GLCHECK();
//	glEnableVertexAttribArray(6); // Re-enable velocity
//	GLCHECK();
//	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffers[1-m_bufferCursor]);
//	GLCHECK();
//	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);
//	GLCHECK();
//	glBeginTransformFeedback(GL_POINTS);
//	GLCHECK();
//	glDrawArrays(GL_POINTS, 0, m_particleCount);
//	GLCHECK();
//	glEndTransformFeedback();
//	GLCHECK();
//	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
//	GLCHECK();
//	glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_particleCount);
//	GLCHECK();
//	m_bufferCursor = 1-m_bufferCursor;

//	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
//	GLCHECK();

//	m_texture.unbind(0);
//}

//void ParticleSystem::render(Camera &camera)
//{
//	GLCLEAR();
//	glDisable(GL_CULL_FACE);
//	GLCHECK();
//	glEnable(GL_BLEND);
//	GLCHECK();
//	glBlendFunc(m_blendKey, m_blendFunc);
//	GLCHECK();
//	glDepthMask(0);
//	glDisable(GL_RASTERIZER_DISCARD);
//	GLCHECK();
//	m_programs[1].use();
//	GLCHECK();
//	glBindVertexArray(m_vertexArrayID[m_bufferCursor]);
//	GLCHECK();
//	camera.update(m_programs[1]);
//	GLCHECK();
//	m_programs[1].setSampers(1);
//	GLCHECK();
//	m_texture.bind(0);
//	GLCHECK();
//	glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering
//	GLCHECK();
//	glDisableVertexAttribArray(6);
//	GLCHECK();
//	glDrawArrays(GL_POINTS, 0, m_particleCount);
//	GLCHECK();
//	glDepthMask(1);
//	glDisable(GL_BLEND);
//	GLCHECK();
//	glEnable(GL_CULL_FACE);
//	GLCHECK();
//}

//void ParticleSystem::setTexture(Texture t)
//{
//	m_texture = std::move(t);
//}

//void ParticleSystem::setBlendFunc(int key, int func)
//{
//	m_blendKey = key;
//	m_blendFunc = func;
//}

//void ParticleSystem::setProperties(glm::vec3 position, glm::vec3 minVelocity, glm::vec3 maxValocity, glm::vec3 gravity, glm::vec3 colour, float minLife, float maxLife, float size, float radius, float period, int count)
//{
//	m_genPosition = std::move(position);
//	m_minGenVelocity = std::move(minVelocity);
//	m_maxGenVelocity = std::move(maxValocity);
//	m_gravityGen = std::move(gravity);
//	m_genColor = std::move(colour);
//	m_minGenLife = minLife;
//	m_maxGenLife = maxLife;
//	m_genSize = size;
//	m_radius = radius;
//	m_nextGenTime = period;
//	m_genCount = count;
//}



//void ParticleSystemBase::init(std::initializer_list<const char *> varyings)
//{

//}


void ParticleSystem::init_programs(size_t field_count, const char **field_names)
{
	m_programs[0].createProgram();
	glTransformFeedbackVaryings(m_programs[0].program(), field_count, field_names, GL_INTERLEAVED_ATTRIBS);
	m_programs[0].linkProgram();
	m_programs[0].resolveUniforms();

	m_programs[1].createProgram();
	m_programs[1].linkProgram();
	m_programs[1].resolveUniforms();
}

void ParticleSystem::init_buffers(size_t particle_size, size_t field_count, field_prop *field_deatils, size_t total_particle_count, size_t init_particle_count, void *init_buffer)
{
	glGenBuffers(2, m_buffers);
	glGenTransformFeedbacks(1, &m_transformBuffer);
	glGenQueries(1, &m_query);
	glGenVertexArrays(2, m_vertexArrayID);
	for(int i = 0; i < 2; ++i)
	{
		glBindVertexArray(m_vertexArrayID[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, total_particle_count * particle_size, nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, init_particle_count * particle_size, init_buffer);

		for(size_t i = 0; i < field_count; ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
								  field_deatils[i].count,
								  field_deatils[i].is_floating_point ? GL_FLOAT : GL_INT, GL_FALSE,
								  particle_size,
								  (const GLvoid*)field_deatils[0].offset);

		}
	}
	m_bufferCursor = 0;
	m_particleCount = init_particle_count;

}

void ParticleSystem::setUpdateShaders(const Shader &update_vert, const Shader &update_geom)
{
	m_programs[0].createShader(update_vert);
	m_programs[0].createShader(update_geom);
}

void ParticleSystem::setRenderShaders(const Shader &render_vert, const Shader &render_geom, const Shader &render_frag)
{
	m_programs[1].createShader(render_vert);
	m_programs[1].createShader(render_geom);
	m_programs[1].createShader(render_frag);
}

void ParticleSystem::addTexture(Texture t)
{
	m_textures.push_back(std::move(t));
}

void ParticleSystem::setBlendFunc(int key, int func)
{
	m_blendKey = key;
	m_blendFunc = func;
}

void ParticleSystem::update(set_uniforms_func func)
{
		static GLSID frame_time_uniform = m_programs[0].resolveUniform("frame_time");
		static GLSID current_time_uniform = m_programs[0].resolveUniform("current_time");

		m_programs[0].use();

		if(frame_time_uniform >= 0 || current_time_uniform >= 0)
		{
			static float start_time = Clock::getDuration();
			float frame_duration = Clock::getDuration() - start_time;
			start_time = Clock::getDuration();
			if(frame_time_uniform >= 0)
			{
				m_programs[0].setUniform(frame_time_uniform, frame_duration);
			}
			if(current_time_uniform >= 0)
			{
				m_programs[0].setUniform(current_time_uniform, start_time);
			}
		}
		if(func)
		{
			func(m_programs[0]);
		}

		glEnable(GL_RASTERIZER_DISCARD);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformBuffer);

		glBindVertexArray(m_vertexArrayID[m_bufferCursor]);

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffers[1-m_bufferCursor]);

		glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);

		glBeginTransformFeedback(GL_POINTS);

		glDrawArrays(GL_POINTS, 0, m_particleCount);
		glEndTransformFeedback();

		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

		glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_particleCount);

		m_bufferCursor = 1-m_bufferCursor;

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void ParticleSystem::render(Camera &c, set_uniforms_func func)
{

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(m_blendKey, m_blendFunc);
		glDepthMask(0);
		glDisable(GL_RASTERIZER_DISCARD);

		m_programs[1].use();
		glBindVertexArray(m_vertexArrayID[m_bufferCursor]);
		c.update(m_programs[1]);
		if(func)
		{
			func(m_programs[1]);
		}
		m_programs[1].setSampers(m_textures.size());
		for(size_t i = 0; i < m_textures.size(); ++i)
		{
			m_textures[i].bind(i);
		}

		glDrawArrays(GL_POINTS, 0, m_particleCount);

		glDepthMask(1);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
}
