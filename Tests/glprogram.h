#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "program.h"
#include "camera.h"
#include "lamp.h"
#include "model.h"
#include "axis.h"

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
        Camera m_camera;
        Lamp m_lamp;
        Axis m_axis;
        std::vector<Model > m_models;
        class GLFWwindow *m_window { nullptr };

    public:
        glProgram();
        void exec();

    private:
        void render();
        void renderSelection();
        void mouseTrack();
};

#endif // GLPROGRAM_H
