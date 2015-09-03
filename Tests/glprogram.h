#ifndef GLPROGRAM_H
#define GLPROGRAM_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/program.h"
#include "Util/camera.h"
#include "Util/lamp.h"
#include "Util/text.h"
#include "Util/controlinput.h"
#include "solarsystem.h"

class glProgram
{
        static constexpr const char * windowTitle { "Testing helppers" };
        static constexpr int windowWidth { 1280 };
        static constexpr int windowHeight { 720 };
        static constexpr float halfWindowWidth { windowWidth / 2.f };
        static constexpr float halfWindowHeight { windowHeight / 2.f };

    private:
        solar_program_t program;
        SolarSystem system;
        Camera m_camera;
        Text m_text;
        Lamp m_lamp;
        ControlInput m_input;

        class GLFWwindow *m_window { nullptr };
        float m_frameRate { 0.f };

    public:
        glProgram();
        void exec();

    private:
        void render();
        void handleSelection(double x, double y);
        void handle_input(const control &ctl);
};

#endif // GLPROGRAM_H
