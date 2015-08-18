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
};

struct vec2 {
    float x;
    float y;
};

struct colour {
    float r{0.f};
    float g{0.f};
    float b{0.f};
    colour() = default;
    colour(unsigned char red, unsigned char green, unsigned char blue)
        : r(red / 255.f), g(green / 255.f), b(blue / 255.f) {}
    colour(float red, float green, float blue) : r(red), g(green), b(blue) {}
};

struct colour_vertex {
    vec3 p;
    colour c;
};

struct uv_vertex {
    vec3 p;
    vec3 n;
    vec2 uv;
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
