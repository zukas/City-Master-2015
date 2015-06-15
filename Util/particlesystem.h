#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "program.h"
#include "camera.h"
#include "texture.h"

class ParticleSystem
{
    private:
        Program m_programs[2];
        GLID m_vertexArrayID[2] { 0 };
        GLID m_buffers[2] { 0 };
        GLID m_transformBuffer { 0 };
        GLID m_query { 0 };

		Texture m_texture { };
		Texture m_random { };

        int m_bufferCursor { 0 };
        int m_particleCount { 0 };
        int m_genCount { 0 };

        float m_startTime { 0.f };
        float m_elapsedTime { 0.f };
        float m_nextGenTime { 0.f };
        float m_minGenLife { 0.f };
        float m_maxGenLife { 0.f };
        float m_genSize { 0.f };
		float m_radius { 0.f };

        glm::vec3 m_genPosition;
        glm::vec3 m_minGenVelocity;
        glm::vec3 m_maxGenVelocity;
        glm::vec3 m_gravityGen;
        glm::vec3 m_genColor;

        int m_blendKey { GL_ONE };
        int m_blendFunc { GL_ONE };


    public:
        ParticleSystem();
        ~ParticleSystem();

        int count() const;
        void update();
        void render(Camera &camera);
        void setTexture(Texture t);
        void setBlendFunc(int key, int func);
		void setProperties(glm::vec3 position, glm::vec3 minVelocity, glm::vec3 maxValocity, glm::vec3 gravity, glm::vec3 colour, float minLife, float maxLife, float size, float radius, float period, int count);
};

#endif // PARTICLESYSTEM_H
