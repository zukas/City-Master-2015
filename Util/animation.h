#ifndef ANIMATION_H
#define ANIMATION_H
#include "types.h"
#include "program.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


struct AnimationKey
{
		glm::quat rotation {};
		glm::vec3 translation { 0.f };
		glm::vec3 scale { 1.f };

		float time { 0.f };
		AnimationKey(){}
		AnimationKey(glm::quat _rotation, glm::vec3 _translation, glm::vec3 _scale, double _time) :
			rotation(std::move(_rotation)),
			translation(std::move(_translation)),
			scale(std::move(_scale)),
			time(_time) {}

		bool operator < (double delta) const { return time < delta; }
};

enum class AnimationStatus
{
	NONE,
	PREPARE,
	RUNNING,
	FINISHED
};

enum class AnimationSequence
{
	SINGLE_SHOT,
	REPEAT
};

enum class AnimationType
{
	KEY_FRAME,
	FUNCTION
};


typedef glm::mat4 (*animation_func)(const glm::mat4 &model, const glm::mat4 &parent, float current, float duration);

class Animation
{
	private:
		std::vector<AnimationKey > m_transform;
		glm::mat4 m_base_transform;
		glm::mat4 m_applied_transform;

		animation_func m_function;

		float m_start { 0.f };
		float m_duration { 0.f };
		AnimationStatus m_status { AnimationStatus::NONE };
		AnimationSequence m_sequence{ AnimationSequence::SINGLE_SHOT };
		AnimationType m_type { AnimationType::KEY_FRAME };

		Animation *m_next { nullptr };

	private:
		glm::mat4 keyframe_transform(const glm::mat4 &model, const glm::mat4 &parent, float current, float duration) const;

	public:
		Animation(std::vector<AnimationKey > &&transform, glm::mat4 base_transform, float duration);
		Animation(animation_func func, float duration);
		void setSequence(AnimationSequence sequence);
		void prepare();
		void clear();
		AnimationStatus status() const;
		void setNext(Animation * next);
		glm::mat4 transform(const glm::mat4 &model, const glm::mat4 &parent);
};

#endif // ANIMATION_H
