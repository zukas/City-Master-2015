#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <glm/glm.hpp>

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
        return glm::vec4{float(r) / 255.f, float(g) / 255.f, float(b) / 255.f,
                         float(a) / 255.f};
    }
};

struct colour_vertex {
    glm::vec3 p;
    glm::vec3 c;
};

struct uv_vertex {
    glm::vec3 p;
    glm::vec3 n;
    glm::vec2 uv;
    bool operator==(const uv_vertex &other) const {
        return p == other.p && n == other.n && uv == other.uv;
    }
};

#endif // TYPES_H
