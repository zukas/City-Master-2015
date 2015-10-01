#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/program.h"
#include "Util/camera.h"
#include "Util/text.h"
#include "Util/controlinput.h"
#include "Util/gbuffer.h"
#include "Util/tempbuffer.h"
#include "Util/smaa.h"
#include "solarsystem.h"

struct point_light_t {
	glm::vec3 position{0.f};
	glm::vec3 colour{0.f};
	float attenuation_constant{0.f};
	float attenuation_linear{0.f};
	float attenuation_exp{0.f};
	float ambient_intensity{0.f};
	float diffuse_intensity{0.f};
};

struct light_pass_program_t {
		uint32_t program_id;
		uint32_t projection_id;
		uint32_t view_id;
		uint32_t model_id;
		uint32_t pix_size_id;
		uint32_t render_type_id;
        uint32_t camera_position_id;

};

class ScreenRender {
	private:
		uint32_t m_mesh_id;

	public:
		ScreenRender() = default;
		~ScreenRender();
		void init();
		void render();
};

class glProgram {
	static constexpr const char *windowTitle{"Testing helppers"};
	static constexpr int windowWidth{1024};
	static constexpr int windowHeight{576};
	static constexpr float halfWindowWidth{windowWidth / 2.f};
	static constexpr float halfWindowHeight{windowHeight / 2.f};

  private:
	class GLFWwindow *m_window{nullptr};

	solar_program_t m_geom_pass_program;
	light_pass_program_t m_light_pass_program;
	solar_program_t m_normal_render;
	SolarSystem m_solar_system;
	GBuffer m_gbuffer;
//	TempBuffer m_temp;
	SMAA m_smaa;
	Camera m_camera;
	ScreenRender m_screen;
	point_light_t m_light;
	Text m_text;
	ControlInput m_input;
    int render_type { 0 };
	float m_frameRate{60.f};
	char m_frameRateBuffer[128];

  public:
	glProgram();
	void exec();

  private:
	void render();
	void handleSelection(double x, double y);
	void handle_input(const control &ctl);
	void geom_pass();
	void light_pass();
	void normal_pass();

};

#endif // GLPROGRAM_H
