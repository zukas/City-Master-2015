#include "program.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>

RefCount Program::g_counter;

void Program::cleanUp()
{
    if(m_programID != 0 && g_counter - m_programID == 0)
    {
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
}

Program::Program() :
    m_programID(0)
{
}

Program::Program(const std::string &vertex, const std::string &fragment, ProgramType type) :
    m_programID(0),
    m_type(type)
{
    std::string vertexCode;
    std::ifstream vertexFile(vertex);
    if(vertexFile.is_open()){
        std::string Line = "";
        while(getline(vertexFile, Line))
            vertexCode += "\n" + Line;
        vertexFile.close();
    }else{
        m_error.code = Error::FILE_NOT_FOUND;
        m_error.message = std::string("Cannot open: ").append(vertex);
        return;
    }

    std::string fragmentCode;
    std::ifstream fragmentFile(fragment);
    if(fragmentFile.is_open()){
        std::string Line = "";
        while(getline(fragmentFile, Line))
            fragmentCode += "\n" + Line;
        fragmentFile.close();
    }else{
        m_error.code = Error::FILE_NOT_FOUND;
        m_error.message = std::string("Cannot open: ").append(fragment);
        return;
    }


    printf("Vertex code\n%s\n\n", vertexCode.c_str());

    printf("Fragment code\n%s\n\n", fragmentCode.c_str());

    fflush(stdout);

    GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint res = GL_FALSE;
    int logLength;

    const char* dataPtr = vertexCode.c_str();
    glShaderSource(vertexID, 1, &dataPtr , nullptr);
    glCompileShader(vertexID);

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &logLength);
    if ( logLength > 0 ){
        m_error.code = Error::FAILED_COMPILE_VERTEX_SHADER;
        m_error.message.resize(logLength+1);
        glGetShaderInfoLog(fragmentID, logLength, nullptr, &m_error.message[0]);
        printf("Vertex shader compile error: %s\n",m_error.message.c_str());
    }

    dataPtr = fragmentCode.c_str();
    glShaderSource(fragmentID, 1, &dataPtr , nullptr);
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &logLength);
    if ( logLength > 0 ){
        m_error.code = Error::FAILED_COMPILE_VERTEX_SHADER;
        m_error.message.resize(logLength+1);
        glGetShaderInfoLog(fragmentID, logLength, nullptr, &m_error.message[0]);
        printf("Fragment shader compile error: %s\n",m_error.message.c_str());
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexID);
    glAttachShader(m_programID, fragmentID);
    glLinkProgram(m_programID);

    glGetProgramiv(m_programID, GL_LINK_STATUS, &res);
    glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
    if ( logLength > 0 ){
        m_error.code = Error::FAILED_LINK_PROGRAM;
        m_error.message.resize(logLength+1);
        glGetShaderInfoLog(fragmentID, logLength, nullptr, &m_error.message[0]);
        printf("Program link error: %s\n",m_error.message.c_str());
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    g_counter+m_programID;

    {
        int total = -1;
        glGetProgramiv( m_programID, GL_ACTIVE_UNIFORMS, &total );
        printf("Number of uniforms: %d\n", total);
        for(int i=0; i<total; ++i)  {
            int name_len=-1, num=-1;
            GLenum type = GL_ZERO;
            char name[100];
            glGetActiveUniform( m_programID, GLuint(i), sizeof(name)-1,
                                &name_len, &num, &type, name );
            name[name_len] = 0;
            GLuint location = glGetUniformLocation( m_programID, name );
            printf("Uniform: %s, Localtion: %i\n", name, location);
        }
        fflush(stdout);
    }



    m_ids.glsl_model_matrix = glGetUniformLocation(m_programID, glsl_model_matrix);
    m_ids.glsl_view_matrix = glGetUniformLocation(m_programID, glsl_view_matrix);
    m_ids.glsl_projection_matrix = glGetUniformLocation(m_programID, glsl_projection_matrix);
    m_ids.glsl_camera_position = glGetUniformLocation(m_programID, glsl_camera_position);
    m_ids.glsl_light_position = glGetUniformLocation(m_programID, glsl_light_position);
    m_ids.glsl_light_strength = glGetUniformLocation(m_programID, glsl_light_strength);

    m_ids.glsl_sampler[0] = glGetUniformLocation(m_programID, glsl_sampler[0]);
    m_ids.glsl_sampler[1] = glGetUniformLocation(m_programID, glsl_sampler[1]);
    m_ids.glsl_sampler[2] = glGetUniformLocation(m_programID, glsl_sampler[2]);
    m_ids.glsl_sampler[3] = glGetUniformLocation(m_programID, glsl_sampler[3]);
    m_ids.glsl_sampler[4] = glGetUniformLocation(m_programID, glsl_sampler[4]);

    m_ids.glsl_texture_count = glGetUniformLocation(m_programID, glsl_texture_count);
    m_ids.glsl_object_id = glGetUniformLocation(m_programID, glsl_object_id);
    m_ids.glsl_object_selected = glGetUniformLocation(m_programID, glsl_object_selected);
    m_ids.glsl_colour = glGetUniformLocation(m_programID, glsl_colour);


}

Program::Program(Program &&other) :
    m_programID(other.m_programID),
    m_type(other.m_type),
    m_ids(other.m_ids)
{
    other.m_programID = 0;
}

Program::Program(const Program &other) :
    m_programID(other.m_programID),
    m_type(other.m_type),
    m_ids(other.m_ids)
{
    g_counter+m_programID;
}

Program::~Program()
{
    cleanUp();
}

void Program::use()
{
    glUseProgram(m_programID);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_viewport.x = viewport[0];
    m_viewport.y = viewport[1];
    m_viewport.w = viewport[2];
    m_viewport.h = viewport[3];
}

ProgramType Program::type() const
{
    return m_type;
}

void Program::setModelMatrix(const glm::mat4 &mat)
{
    glUniformMatrix4fv(m_ids.glsl_model_matrix, 1, GL_FALSE, &mat[0][0]);
}

void Program::setViewMatrix(const glm::mat4 &mat)
{
    glUniformMatrix4fv(m_ids.glsl_view_matrix, 1, GL_FALSE, &mat[0][0]);
}

void Program::setProjectionMatrix(const glm::mat4 &mat)
{
    glUniformMatrix4fv(m_ids.glsl_projection_matrix, 1, GL_FALSE, &mat[0][0]);
}

void Program::setCameraPosition(const glm::vec3 &vec)
{
    glUniform3fv(m_ids.glsl_camera_position, 1, &vec[0]);
}

void Program::setLigthPosition(const glm::vec3 &vec)
{
    glUniform3fv(m_ids.glsl_light_position, 1, &vec[0]);
}

void Program::setLightStrength(float value)
{
    glUniform1f(m_ids.glsl_light_strength, value);
}

void Program::setSampers(GLID value)
{

    for(unsigned i = 0; i < value; ++i)
    {
        glUniform1ui(m_ids.glsl_sampler[i], i);
    }

    glUniform1i(m_ids.glsl_texture_count, value);
}

void Program::setObjectID(const glm::vec4 &vec)
{
    glUniform4fv(m_ids.glsl_object_id, 1, &vec[0]);
}

void Program::setSelected(bool value)
{
    glUniform1i(m_ids.glsl_object_selected, value);
}

void Program::setColour(const glm::vec4 &vec)
{
    glUniform4fv(m_ids.glsl_colour, 1, &vec[0]);
}

void Program::setUniform(const std::string &name, bool value)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform1i(id, value);
}

void Program::setUniform(const std::string &name, GLID value)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform1ui(id, value);
}

void Program::setUniform(const std::string &name, GLSID value)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform1i(id, value);
}

void Program::setUniform(const std::string &name, float value)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform1f(id, value);
}

void Program::setUniform(const std::string &name, const glm::vec2 &vec)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform2fv(id, 1, &vec[0]);
}

void Program::setUniform(const std::string &name, const glm::vec3 &vec)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform3fv(id, 1, &vec[0]);
}

void Program::setUniform(const std::string &name, const glm::vec4 &vec)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniform4fv(id, 1, &vec[0]);
}

void Program::setUniform(const std::string &name, const glm::mat4 &mat)
{
    GLSID id = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0][0]);
}

Viewport Program::getViewport() const
{
    return m_viewport;
}

Error Program::getError() const
{
    return m_error;
}


Program &Program::operator =(Program &&other)
{
    cleanUp();
    m_programID = other.m_programID;
    m_type = other.m_type;
    m_ids = other.m_ids;
    other.m_programID = 0;
    return *this;
}

Program &Program::operator = (const Program &other)
{
    cleanUp();
    m_programID = other.m_programID;
    m_type = other.m_type;
    m_ids = other.m_ids;
    g_counter+m_programID;
    return *this;
}

