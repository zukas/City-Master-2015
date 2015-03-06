#ifndef COLOURID_H
#define COLOURID_H

#include <glm/glm.hpp>
#include <vector>

class ColourID
{
    private:
        static std::vector<int > g_colourID;
        int m_value { 0 };
        int m_red { 0 };
        int m_green { 0 };
        int m_blue { 0 };
        glm::vec3 m_colour {};

    public:
        static constexpr int max_red { 255 };
        static constexpr int max_green { 255 << 8 };
        static constexpr int max_blue { 255 << 16 };

        ColourID(int id);
        ColourID(int in_red, int in_green, int in_blue);

        int value() const;
        glm::vec3 colour() const;
        int red() const;
        int green() const;
        int blue() const;

        operator glm::vec3 () const;
        operator int () const;


        static ColourID aquireID();
        static void releaseID(int id);
};

#endif // COLOURID_H
