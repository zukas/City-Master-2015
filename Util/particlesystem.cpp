#include "particlesystem.h"
#include "clock.h"

#include <GL/glew.h>

struct Particle
{
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 color;
        float lifeTime;
        float size;
        int type;
        int reuse;
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

    const char *varyings[7]
    {
        "vPositionOut",
        "vVelocityOut",
        "vColorOut",
        "fLifeTimeOut",
        "fSizeOut",
        "iTypeOut",
        "reuseOut"
    };

    glTransformFeedbackVaryings(m_programs[0].program(), 7, varyings, GL_INTERLEAVED_ATTRIBS);

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
    p.reuse = 0;


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
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (const GLvoid*)24); // Color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_INT, GL_TRUE, sizeof(Particle), (const GLvoid*)44); // Type
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, sizeof(Particle), (const GLvoid*)48); // Reuse
    }
    m_bufferCursor = 0;
    m_particleCount = 1;
    m_startTime = Clock::getDuration();

}

ParticleSystem::~ParticleSystem()
{

}

int ParticleSystem::count() const
{
    return m_particleCount;
}

void ParticleSystem::update()
{

    float timePassed = (Clock::getDuration() - m_startTime);
    m_startTime = Clock::getDuration();

    m_programs[0].use();

    m_programs[0].setUniform("fTimePassed", timePassed);

    m_programs[0].setUniform("vGenPosition", m_genPosition);

    m_programs[0].setUniform("vGenVelocityMin", m_minGenVelocity);

    m_programs[0].setUniform("vGenVelocityRange", m_maxGenVelocity);

    m_programs[0].setUniform("vGenColor", m_genColor);

    m_programs[0].setUniform("vGenGravityVector", m_gravityGen);

    m_programs[0].setUniform("fGenLifeMin", m_minGenLife);

    m_programs[0].setUniform("fGenLifeRange", m_maxGenLife);

    m_programs[0].setUniform("fGenSize", m_genSize);
    m_programs[0].setUniform("fDim", 3.f);

    m_programs[0].setUniform("iNumToGenerate", 0);

//    m_elapsedTime += timePassed;

//    if(m_elapsedTime > m_nextGenTime)
//    {

        m_programs[0].setUniform("iNumToGenerate", m_genCount);

//        m_elapsedTime -= m_nextGenTime;

        glm::vec3 vRandomSeed = glm::vec3(grandf(-1.0f,1.0f), grandf(0.f,.1f), grandf(-1.0f,1.0f));
        m_programs[0].setUniform("vRandomSeed", vRandomSeed);

//    }


    glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformBuffer);

    glBindVertexArray(m_vertexArrayID[m_bufferCursor]);

    glEnableVertexAttribArray(1); // Re-enable velocity
    glEnableVertexAttribArray(6); // Re-enable velocity

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

void ParticleSystem::render(Camera &camera)
{
    glEnable(GL_BLEND);

    glBlendFunc(m_blendKey, m_blendFunc);

    glDepthMask(0);

    glDisable(GL_RASTERIZER_DISCARD);

    m_programs[1].use();

    glBindVertexArray(m_vertexArrayID[m_bufferCursor]);

    camera.update(m_programs[1]);

    m_programs[1].setSampers(1);

    m_texture.bind(0);

    glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering
    glDisableVertexAttribArray(6);

    glDrawArrays(GL_POINTS, 0, m_particleCount);

    glDepthMask(1);

    glDisable(GL_BLEND);
}

void ParticleSystem::setTexture(Texture t)
{
    m_texture = std::move(t);
}

void ParticleSystem::setBlendFunc(int key, int func)
{
    m_blendKey = key;
    m_blendFunc = func;
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

