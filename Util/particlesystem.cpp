#include "particlesystem.h"
#include "clock.h"

#include <GL/glew.h>

struct Particle
{
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 volor;
        float lifeTime;
        float size;
        int type;
};

constexpr int max_num_particles { 100000 };

float grandf(float min, float max)
{
    float val = float((rand()%(RAND_MAX)) + 1.f)/float(RAND_MAX);
    return min+max*val;
}

ParticleSystem::ParticleSystem()
{

    m_programs[0].createShader("shaders/particles_update.vert", VERTEX);
    m_programs[0].createShader("shaders/particles_update.geom", GEOMETRY);
    m_programs[0].createProgram();

    const char *varyings[6]
    {
        "vPositionOut",
        "vVelocityOut",
        "vColorOut",
        "fLifeTimeOut",
        "fSizeOut",
        "iTypeOut"
    };

    glTransformFeedbackVaryings(m_programs[0].program(), 6, varyings, GL_INTERLEAVED_ATTRIBS);

    m_programs[0].linkProgram();
    m_programs[0].resolveUniforms();

    m_programs[1].createShader("shaders/particles_render.vert", VERTEX);
    m_programs[1].createShader("shaders/particles_render.geom", GEOMETRY);
    m_programs[1].createShader("shaders/particles_render.frag", FRAGMENT);
    m_programs[1].createProgram();
    m_programs[1].linkProgram();
    m_programs[1].resolveUniforms();

    glGenBuffers(2, m_buffers);

    glGenTransformFeedbacks(1, &m_transformBuffer);
    glGenQueries(1, &m_query);
    glGenVertexArrays(2, m_vertexArrayID);


    Particle p;
    p.type = 0;


    for(int i = 0; i < 2; ++i)
    {
        glBindVertexArray(m_vertexArrayID[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * max_num_particles, nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &p);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)0); // Position
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12); // Velocity
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)24); // Color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), (const GLvoid*)44); // Type
    }
    m_bufferCursor = 0;
    m_particleCount = 1;
    m_startTime = Clock::getDuration();

}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::update()
{
    glGetError();

    float timePassed = (Clock::getDuration() - m_startTime);
    m_startTime = Clock::getDuration();
//    std::printf("duration: %f\n", timePassed);
    m_programs[0].use();

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 1: %d\n", error);
    }

    m_programs[0].setUniform("fTimePassed", timePassed);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 2: %d\n", error);
    }

    m_programs[0].setUniform("vGenPosition", m_genPosition);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 3: %d\n", error);
    }

    m_programs[0].setUniform("vGenVelocityMin", m_minGenVelocity);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 4: %d\n", error);
    }

    m_programs[0].setUniform("vGenVelocityRange", m_maxGenVelocity);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 5: %d\n", error);
    }

    m_programs[0].setUniform("vGenColor", m_genColor);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 6: %d\n", error);
    }

    m_programs[0].setUniform("vGenGravityVector", m_gravityGen);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 7: %d\n", error);
    }

    m_programs[0].setUniform("fGenLifeMin", m_minGenLife);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 8: %d\n", error);
    }

    m_programs[0].setUniform("fGenLifeRange", m_maxGenLife);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 9: %d\n", error);
    }

    m_programs[0].setUniform("fGenSize", m_genSize);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 10: %d\n", error);
    }

    m_programs[0].setUniform("iNumToGenerate", 0);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 11: %d\n", error);
    }


    m_elapsedTime += timePassed;

    if(m_elapsedTime > m_nextGenTime)
    {
        //std::printf("elapsed: %f\n", m_elapsedTime);

        m_programs[0].setUniform("iNumToGenerate", m_genCount);

        std::printf("generating %d particles\n", m_genCount);

        error = glGetError();
        if(error != GL_NO_ERROR)
        {
            std::printf("GL update error 12: %d\n", error);
        }

        m_elapsedTime -= m_nextGenTime;

        glm::vec3 vRandomSeed = glm::vec3(grandf(-2.0f,3.0f), grandf(-2.0f,3.0f), grandf(-2.0f,3.0f));
        m_programs[0].setUniform("vRandomSeed", vRandomSeed);

        error = glGetError();
        if(error != GL_NO_ERROR)
        {
            std::printf("GL update error 13: %d\n", error);
        }

    }


    glEnable(GL_RASTERIZER_DISCARD);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 14: %d\n", error);
    }

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformBuffer);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 15: %d\n", error);
    }

    glBindVertexArray(m_vertexArrayID[m_bufferCursor]);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 16: %d\n", error);
    }

    glEnableVertexAttribArray(1); // Re-enable velocity

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 17: %d\n", error);
    }

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffers[1-m_bufferCursor]);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 18: %d\n", error);
    }

    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 19: %d\n", error);
    }

    glBeginTransformFeedback(GL_POINTS);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 20: %d\n", error);
    }

    glDrawArrays(GL_POINTS, 0, m_particleCount);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 21: %d\n", error);
    }

    glEndTransformFeedback();

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 22: %d\n", error);
    }

    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 23: %d\n", error);
    }

    glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_particleCount);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 24: %d\n", error);
    }

    std::printf("particle count: %d\n", m_particleCount);

    m_bufferCursor = 1-m_bufferCursor;

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL update error 25: %d\n", error);
    }
}

void ParticleSystem::render(Camera &camera)
{
    glGetError();
    glEnable(GL_BLEND);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 1: %d\n", error);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL error: %d\n", error);
    }

    glDepthMask(0);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 2: %d\n", error);
    }

    glDisable(GL_RASTERIZER_DISCARD);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 3: %d\n", error);
    }

    m_programs[1].use();

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 4: %d\n", error);
    }

    glBindVertexArray(m_vertexArrayID[m_bufferCursor]);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 7: %d\n", error);
    }

    camera.update(m_programs[1]);


    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 5: %d\n", error);
    }


    m_programs[1].setSampers(1);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error - setting sampler: %d\n", error);
    }

    m_texture.bind(0);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 6: %d\n", error);
    }

    glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 8: %d\n", error);
    }

    glDrawArrays(GL_POINTS, 0, m_particleCount);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 9: %d\n", error);
    }

    glDepthMask(1);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 10: %d\n", error);
    }

    glDisable(GL_BLEND);

    error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::printf("GL render error 11: %d\n", error);
    }

    std::fflush(stdout);
}

void ParticleSystem::setTexture(Texture t)
{
    m_texture = std::move(t);
}

void ParticleSystem::setProperties(glm::vec3 position, glm::vec3 minVelocity, glm::vec3 maxValocity, glm::vec3 gravity, glm::vec3 colour, float minLife, float maxLife, float size, float period, int count)
{
    m_genPosition = std::move(position);
    m_minGenVelocity = std::move(minVelocity);
    m_maxGenVelocity = std::move(maxValocity);
    m_gravityGen = std::move(gravity);
    m_genColor = std::move(colour);
    m_minGenLife = minLife;
    m_maxGenLife = maxLife;
    m_genSize = size;
    m_nextGenTime = period;
    m_genCount = count;
}

