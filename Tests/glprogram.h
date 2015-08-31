#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "Util/program.h"
#include "Util/camera.h"
#include "Util/lamp.h"
#include "Util/model.h"
#include "Util/axis.h"
#include "Util/text.h"
#include "Util/mouse.h"
#include "Util/keyboard.h"
#include "Util/skybox.h"
#include "Util/particlesystem.h"
#include "saturnrings.h"
#include "solarsystem.h"

class glProgram
{
        static constexpr const char * windowTitle { "Testing helppers" };
        static constexpr int windowWidth { 1280 };
        static constexpr int windowHeight { 720 };
        static constexpr float halfWindowWidth { windowWidth / 2.f };
        static constexpr float halfWindowHeight { windowHeight / 2.f };

    private:
//        uint32_t m_objectProgram;
//        Program m_objectSelectionProgram;
        Camera m_camera;

		SolarSystem system;
        solar_program_t program;

//		SaturnRings rings;
        Text m_text;
        Lamp m_lamp;
        SkyBox m_box;
        Mouse m_mouse;
        Keyboard m_keyboard;
        Axis m_axis;
        std::vector<Model > m_models;
        class GLFWwindow *m_window { nullptr };
		float m_maxFrameRate { 0.f };
		float m_minFrameRate { 6000.f };

    public:
        glProgram();
        void exec();

    private:
        void render();
        void handleSelection(double x, double y);
};

#endif // GLPROGRAM_H
