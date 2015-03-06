#ifndef ANIMATION_H
#define ANIMATION_H
#include "types.h"
#include "program.h"

#include <glm/glm.hpp>


struct AnimationKey
{
    glm::mat4 transformation;
    double time;
    AnimationKey();
};

enum AnimationStatus
{
    NONE,
    PREPARE,
    FIRST_FRAME,
    RUNNING,
    END_FRAME,
    FINISHED
};

enum AnimationSequence
{
    SINGLE_SHOT,
    REPEAT
};

class Animation
{
    private:
        std::vector<AnimationKey > m_transform;
        double m_start { 0 };
        AnimationStatus m_status { NONE };
        AnimationSequence m_sequence{ SINGLE_SHOT };

    public:
        Animation(std::vector<AnimationKey > &&transform);
        void setSequence(AnimationSequence sequence);
        void prepare();
        void clear();
        AnimationStatus status() const;
        glm::mat4 transform();
};

#endif // ANIMATION_H
