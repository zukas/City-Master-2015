#include "animation.h"
#include "clock.h"
#include <algorithm>

AnimationKey::AnimationKey() :
    transformation(1.f),
    time(0)
{
}

Animation::Animation(std::vector<AnimationKey> &&transform) :
    m_transform(std::move(transform))
{
    std::sort(std::begin(m_transform), std::end(m_transform), [](const AnimationKey &a, const AnimationKey &b)
    {
        return a.time < b.time;
    });
}

void Animation::setSequence(AnimationSequence sequence)
{
    m_sequence = sequence;
}

void Animation::prepare()
{
    m_status = PREPARE;
}

void Animation::clear()
{
    m_status = NONE;
}

AnimationStatus Animation::status() const
{
    return m_status;
}

glm::mat4 Animation::transform()
{
    glm::mat4 tmp(1.f);
    if(m_status == END_FRAME)
    {
        if(m_sequence == SINGLE_SHOT)
        {
            m_status = FINISHED;
        }
        else
        {
            m_status = PREPARE;
        }
    }


    if(m_status != NONE && m_status != FINISHED)
    {
        if(m_status == PREPARE)
        {
            m_start = Clock::getDuration();
        }

        double time = Clock::getDuration() - m_start;


        if(time < m_transform.front().time)
        {
            tmp = m_transform.front().transformation;
            m_status = FIRST_FRAME;
        }
        else if(time < m_transform.back().time)
        {
            auto it = std::find_if(std::begin(m_transform), std::end(m_transform), [time] (const AnimationKey &key)
            {
                return key.time >= time;
            });

            if(it != std::end(m_transform))
            {
                tmp = it->transformation;
            }
            m_status = RUNNING;
        }
        else
        {
            tmp = m_transform.back().transformation;
            m_status = END_FRAME;
        }
    }

    return tmp;
}



