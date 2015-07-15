#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "program.h"
#include "camera.h"
#include "lamp.h"
#include "model.h"
#include "axis.h"
#include "text.h"
#include "mouse.h"
#include "keyboard.h"
#include "skybox.h"
#include "particlesystem.h"
#include "saturnrings.h"

class glProgram
{
        static constexpr const char * windowTitle { "Testing helppers" };
        static constexpr int windowWidth { 1280 };
        static constexpr int windowHeight { 720 };
        static constexpr float halfWindowWidth { windowWidth / 2.f };
        static constexpr float halfWindowHeight { windowHeight / 2.f };

    private:
        Program m_objectProgram;
        Program m_objectSelectionProgram;
        Program m_axisProgram;
        Program m_textProgram;
        Program m_skyProgram;
        Camera m_camera;

		SaturnRings rings;
        Text m_text;
        Lamp m_lamp;
        SkyBox m_box;
        Mouse m_mouse;
        Keyboard m_keyboard;
        Axis m_axis;
        ParticleSystem * m_fire { nullptr };
        ParticleSystem * m_smoke { nullptr };
        std::vector<Model > m_models;
        class GLFWwindow *m_window { nullptr };
        float m_frameRate { 0 };

    public:
        glProgram();
        void exec();

    private:
        void render();
        void handleSelection(double x, double y);
};

#endif // GLPROGRAM_H
