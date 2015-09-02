//#ifndef PARTICLESYSTEM_H
//#define PARTICLESYSTEM_H

//#include "program.h"
//#include "camera.h"
//#include <stddef.h>

//template <typename _T, size_t _N> struct array {
//    _T values[_N];
//    const _T &operator[](int idx) const { return values[idx]; }
//    constexpr size_t size() const { return _N; }
//    operator _T *() { return values; }
//};

//struct field_prop {
//    std::uintptr_t offset;
//    size_t count;
//    bool is_floating_point;
//};

//template <typename... Fields> struct Particle : Fields... {
//    using particle_t = Particle;
//    using filed_names_t = array<const char *, sizeof...(Fields)>;
//    using filed_details_t = array<field_prop, sizeof...(Fields)>;
//    Particle() = default;
//    Particle(const Particle &) = default;

//    template <typename... T>
//    constexpr Particle(T &&... x)
//        : Fields{static_cast<T &&>(x)}... {
//        static_assert(sizeof...(T) == field_count(),
//                      "Particle argument count mismatch");
//    }

//    static constexpr size_t field_count() { return sizeof...(Fields); }

//    static constexpr filed_names_t field_names() {
//        return {(Fields::name())...};
//    }

//    static constexpr filed_details_t field_details() {
//        return {
//            (field_prop{reinterpret_cast<std::uintptr_t>(
//                            static_cast<Particle *>(nullptr)->Fields::value()),
//                        Fields::count(), Fields::is_floating_point()})...};
//    }
//};

//template <typename T> struct field_detail_t {
//    static_assert(std::is_arithmetic<T>::value,
//                  "Particle types must be of integral types");
//    typedef T type_t;
//    static constexpr size_t count{1};
//    static constexpr bool is_floating_point{std::is_floating_point<T>::value};
//};

//template <typename T, size_t N> struct field_detail_t<T[N]> {
//    static_assert(std::is_arithmetic<T>::value,
//                  "Particle types must be of integral types");
//    static_assert(N == 2 || N == 3, "Array length must be greater then zero");
//    typedef T type_t;
//    static constexpr size_t count{N};
//    static constexpr bool is_floating_point{std::is_floating_point<T>::value};
//};

//#define PARTICLE_FIELD(type_name, type_id)                                     \
//    struct type_name##_field_t {                                               \
//        using field_detail = field_detail_t<type_id>;                          \
//        field_detail::type_t type_name[field_detail::count];                   \
//        template <typename __T,                                                \
//                  typename = typename std::enable_if<                          \
//                      field_detail::count == 1 && sizeof(__T)>::type>          \
//        type_name##_field_t(__T a)                                             \
//            : type_name{a} {}                                                  \
//        template <typename __T,                                                \
//                  typename = typename std::enable_if<                          \
//                      field_detail::count == 2 && sizeof(__T)>::type>          \
//        type_name##_field_t(__T a, __T b)                                      \
//            : type_name{a, b} {}                                               \
//        template <typename __T,                                                \
//                  typename = typename std::enable_if<                          \
//                      field_detail::count == 3 && sizeof(__T)>::type>          \
//        type_name##_field_t(__T a, __T b, __T c)                               \
//            : type_name{a, b, c} {}                                            \
//        static constexpr const char *name() { return #type_name; };            \
//        field_detail::type_t *value() { return type_name; };                   \
//        static constexpr size_t count() { return field_detail::count; };       \
//        static constexpr bool is_floating_point() {                            \
//            return field_detail::is_floating_point;                            \
//        };                                                                     \
//    }

//// typedef void (*set_uniforms_func)(UniformManager&);

//class ParticleSystem {
//  private:
//    Program m_programs[2];
//    GLID m_vertexArrayID[2]{0};
//    GLID m_buffers[2]{0};
//    GLID m_transformBuffer{0};
//    GLID m_query{0};

//    int m_bufferCursor{0};
//    int m_particleCount{0};
//    float m_startTime{0.f};
//    int m_blendKey{GL_SRC_ALPHA};
//    int m_blendFunc{GL_ONE_MINUS_SRC_ALPHA};

//    std::vector<Texture> m_textures;

//  private:
//    void init_programs(size_t field_count, const char **field_names);

//    void init_buffers(size_t particle_size, size_t field_count,
//                      field_prop *field_deatils, size_t total_particle_count,
//                      size_t init_particle_count, void *init_buffer);

//  public:
//    ParticleSystem() = default;

//    void setUpdateShaders(const Shader &update_vert, const Shader &update_geom);
//    void setRenderShaders(const Shader &render_vert, const Shader &render_geom,
//                          const Shader &render_frag);
//    void addTexture(Texture t);
//    void setBlendFunc(int key, int func);

//    template <typename... Fields>
//    void init(size_t max_particles,
//              std::vector<Particle<Fields...>> starting_set) {

//        init_programs(Particle<Fields...>::field_count(),
//                      Particle<Fields...>::field_names());
//        init_buffers(sizeof(Particle<Fields...>),
//                     Particle<Fields...>::field_count(),
//                     Particle<Fields...>::field_details(), max_particles,
//                     starting_set.size(), &starting_set[0]);
//    }

//    //		void update(set_uniforms_func func = nullptr);
//    //		void render(Camera &c, set_uniforms_func func = nullptr);
//};

//#endif // PARTICLESYSTEM_H
