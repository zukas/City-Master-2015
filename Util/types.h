#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef DEBUG_BUILD
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

template <typename __T> struct remove_reference { typedef __T type; };

template <typename __T> struct remove_reference<__T &> { typedef __T type; };

template <typename __T> struct remove_reference<__T &&> { typedef __T type; };

template <typename __T>
constexpr typename remove_reference<__T>::type &&move(__T &&__t) noexcept {
    return static_cast<typename remove_reference<__T>::type &&>(__t);
}

typedef void *(*mem_alloc)(uint32_t);
typedef void *(*mem_free)(void *);

typedef int glint;

class GLID {
  private:
    unsigned value;

  public:
    constexpr GLID() : value(0) {}
    constexpr GLID(unsigned val) : value(val) {}
    GLID(GLID &&other) : value(other.value) { other.value = 0; }
    GLID(const GLID &other) = delete;

    GLID &&move() { return ::move(*this); }

    GLID &operator=(GLID &&other) {
        value = other.value;
        other.value = 0;
        return *this;
    }
    GLID &operator=(const GLID &other) = delete;
    operator unsigned &() { return value; }
    constexpr operator unsigned() const { return value; }
    unsigned *operator&() { return &value; }
};

typedef unsigned char byte;

constexpr float PI{3.1415926535897932384626433832795f};
constexpr float PI2{PI * 2.0};

struct vec3 {
    float x;
    float y;
    float z;
    vec3 operator*(float scaler) {
        return {x * scaler, y * scaler, z * scaler};
    }
    bool operator == (const vec3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct vec2 {
    float x;
    float y;
    bool operator == (const vec2 &other) const {
        return x == other.x && y == other.y;
    }
};

struct rect {
	float x;
	float y;
	float w;
	float h;
};

class colour {
  public:
	byte r;
	byte g;
	byte b;
	byte a;
	colour() : r(0), g(0), b(0), a(255) {}
	colour(byte r_, byte g_, byte b_, byte a_ = 255)
		: r(r_), g(g_), b(b_), a(a_) {}
	operator glm::vec4() const {
		return glm::vec4(float(r) / 255.f, float(g) / 255.f, float(b) / 255.f,
						 float(a) / 255.f);
	}
};

struct colour_vertex {
    vec3 p;
	vec3 c;
};

struct uv_vertex {
    vec3 p;
    vec3 n;
    vec2 uv;
    bool operator == (const uv_vertex &other) const {
        return p == other.p && n == other.n && uv == other.uv;
    }

};

typedef std::vector<uv_vertex> uv_vertex_array;
typedef std::vector<class Texture> texture_array;

struct transform {
    glm::quat rot{};
    glm::vec3 loc{};
    glm::vec3 scale{1.f};
    operator glm::mat4() const {
        return glm::scale(glm::translate(glm::mat4_cast(rot), loc), scale);
    }
};

struct shape {
    std::vector<uv_vertex> data;
    std::vector<uint32_t> indexes;
};

#endif // TYPES_H
