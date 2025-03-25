#include "AnimatorComponent.h"

void AnimatorComponent::ShutDown()
{
}

void AnimatorComponent::Setup(std::shared_ptr<SpriteComponent> spriteComponent)
{
	m_spriteComponent = spriteComponent;
}

void AnimatorComponent::Update(float deltaTime)
{
	if (m_currentAnimation == nullptr || m_currentAnimation->m_frameCount == 1)
	{
		return;
	}

	m_animationTimer += deltaTime;
	if (m_animationTimer >= m_currentAnimation->m_frequencySeconds)
	{
		m_currentSpriteIndex++;
		m_currentSpriteIndex %= m_currentAnimation->m_texturePaths.size();
		m_spriteComponent->SetTexture(m_currentAnimation->m_texturePaths[m_currentSpriteIndex]);
		m_animationTimer = 0;
	}
}

void AnimatorComponent::AddAnimation(const std::string& name, const std::vector<std::string>& texturePaths, float frequencySeconds)
{
	Animation animation;
	animation.m_name = name;
	animation.m_texturePaths = texturePaths;
	animation.m_frequencySeconds = frequencySeconds;
	animation.m_frameCount = texturePaths.size();
	m_nameToAnimation[name] = animation;
}

void AnimatorComponent::SetAnimation(const std::string& name)
{
	Animation* newAnimation = &m_nameToAnimation[name];
	if (m_currentAnimation == newAnimation)
	{
		return;
	}

	m_currentAnimation = newAnimation;
	m_animationTimer = 0;
	m_currentSpriteIndex = 0;
	m_spriteComponent->SetTexture(m_currentAnimation->m_texturePaths[m_currentSpriteIndex]);
}
