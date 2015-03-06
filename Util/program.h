#ifndef PROGRAM_H
#define PROGRAM_H

#include "types.h"
#include "refcount.h"
#include <glm/glm.hpp>

enum ProgramType
{
    Normal, Selection
};

class Program
{
    private:
        GLID m_programID;
        ProgramType m_type;
        Error m_error;

        static RefCount g_counter;

    public:
        Program();
        Program(const std::string &vertex, const std::string &fragment, ProgramType type = Normal);
        Program(Program &&other);
        Program(const Program &other);
        ~Program();
        void use();
        ProgramType type() const;
        void setUniform(const std::string &name, bool value);
        void setUniform(const std::string &name, GLID value);
        void setUniform(const std::string &name, GLSID value);
        void setUniform(const std::string &name, float value);
        void setUniform(const std::string &name, const glm::vec2 &vec);
        void setUniform(const std::string &name, const glm::vec3 &vec);
        void setUniform(const std::string &name, const glm::vec4 &vec);
        void setUniform(const std::string &name, const glm::mat4 &mat);

        Error getError() const;
        Program &operator = (Program &&other);
        Program &operator = (const Program &other);
};

#endif // PROGRAM_H
