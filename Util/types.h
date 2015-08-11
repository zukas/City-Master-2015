#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef DEBUG_BUILD
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif


typedef unsigned GLID;
typedef int GLSID;
typedef unsigned char byte;

constexpr float PI { 3.1415926535897932384626433832795f };
constexpr float PI2 { PI * 2.0 };

struct vec3
{
		float x;
		float y;
		float z;
		vec3 operator * (float scaler) { return { x * scaler, y * scaler, z * scaler }; }
};

struct vec2
{
		float x;
		float y;
};

struct colour
{
		float r { 0.f };
		float g { 0.f };
		float b { 0.f };
		colour () = default;
		colour(unsigned char red, unsigned char green, unsigned char blue) :
			r(red/255.f),
			g(green/255.f),
			b(blue/255.f)
		{
		}
		colour(float red, float green, float blue) :
			r(red),
			g(green),
			b(blue)
		{
		}
};

struct colour_vertex
{
		vec3 p;
		colour c;
};

struct uv_vertex
{
		vec3 p;
		vec3 n;
		vec2 uv;
};

typedef std::vector<uv_vertex> uv_vertex_array;
typedef std::vector<class Texture> texture_array;

struct transform
{
		glm::quat rot {};
		glm::vec3 loc {};
		glm::vec3 scale { 1.f };
		operator glm::mat4 () const
		{
			return glm::scale(glm::translate(glm::mat4_cast(rot), loc), scale);
		}
};

struct shape
{
		std::vector<uv_vertex> data;
		std::vector<GLID> indexes;
};

struct Error
{
        std::string message;
        enum
        {
            NONE,

            FAILED_INIT_GLFW,
            FAILED_INIT_GLEW,

            FAILED_COMPILE_FRAGMENT_SHADER,
            FAILED_COMPILE_VERTEX_SHADER,
            FAILED_LINK_PROGRAM,

            NOT_SUPPORTED_GL3_3,

            FILE_NOT_FOUND
        } code { NONE };
};




#endif // TYPES_H
