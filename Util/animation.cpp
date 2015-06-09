#include "animation.h"
#include "clock.h"
#include "gldebugger.h"
#include "utils.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>


glm::mat4 Animation::keyframe_transform(const glm::mat4 &model, const glm::mat4 &parent, float current, float duration) const
{
	glm::mat4 tmp = model;
	for(int i = 0, l = m_transform.size(), last = l - 1; i < l; ++i)
	{
		if(!(m_transform[i] < current))
		{
			if(i == 0)
			{
				tmp *= transform_mat4(m_transform[0].rotation, m_transform[0].translation, m_transform[0].scale, m_applied_transform);
			}
			else
			{
				auto dur = m_transform[i].time - m_transform[i - 1].time;
				auto curr = current - m_transform[i - 1].time;
				float interp = curr / dur;

				auto rot = glm::slerp(m_transform[i - 1].rotation, m_transform[i].rotation, interp);
				auto trans = m_transform[i - 1].translation + (m_transform[i].translation - m_transform[i - 1].translation) * interp;
				auto scale = m_transform[i - 1].scale + (m_transform[i].scale - m_transform[i - 1].scale) * interp;
				tmp *= transform_mat4(rot, trans, scale, m_applied_transform);
			}
			break;
		}
		else if(i == last)
		{
			tmp *= transform_mat4(m_transform[last].rotation, m_transform[last].translation, m_transform[last].scale, m_applied_transform);
		}
	}
	return tmp;
}

Animation::Animation(std::vector<AnimationKey> &&transform, glm::mat4 base_transform, float duration) :
	m_transform(std::move(transform)),
	m_base_transform(std::move(base_transform)),
	m_applied_transform(m_base_transform),
	m_duration(duration),
	m_type(AnimationType::KEY_FRAME)
{
	std::sort(std::begin(m_transform), std::end(m_transform), [](const AnimationKey &a, const AnimationKey &b)
	{
		return a.time < b.time;
	});
}

Animation::Animation(animation_func func, float duration) :
	m_function(func),
	m_duration(duration),
	m_type(AnimationType::FUNCTION)
{
}

void Animation::setSequence(AnimationSequence sequence)
{
	m_sequence = sequence;
}

void Animation::prepare()
{
	m_status = AnimationStatus::PREPARE;
	m_applied_transform = m_base_transform;
	LOG("%s","Animation PREPARE");
	if(m_next)
	{
		m_next->prepare();
	}
}

void Animation::clear()
{
	m_status = AnimationStatus::NONE;
	m_applied_transform = m_base_transform;
	LOG("%s", "Animation CLEAR");
	if(m_next)
	{
		m_next->clear();
	}
}

AnimationStatus Animation::status() const
{
	return m_status;
}

void Animation::setNext(Animation *next)
{
	m_next = next;
}

glm::mat4 Animation::transform(const glm::mat4 &model, const glm::mat4 &parent)
{
	glm::mat4 tmp;

	if(m_status != AnimationStatus::NONE && m_status != AnimationStatus::FINISHED)
	{
		if(m_status == AnimationStatus::PREPARE)
		{
			m_start = Clock::getDuration();
			m_status = AnimationStatus::RUNNING;
		}

		float time = Clock::getDuration() - m_start;
		if(time > m_duration)
		{
			if(m_sequence == AnimationSequence::SINGLE_SHOT)
			{
				m_status = AnimationStatus::FINISHED;
			}
			else
			{
				m_start += m_duration;
				time -= m_duration;
			}
		}

		if(m_status != AnimationStatus::FINISHED)
		{
			if(m_type == AnimationType::KEY_FRAME)
			{
				tmp = keyframe_transform(model, parent, time, m_duration);
			}
			else
			{
				tmp = m_function(model, parent, time, m_duration);
			}
		}

		if(m_next)
		{
			tmp = m_next->transform(tmp, parent);
		}
	}

	return tmp;
}



