#include "../Entity/Entity.h"

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "../EntityComponents/SpriteComponent.h"

struct Animation
{
	std::string m_name;
	std::vector<std::string> m_texturePaths;
	float m_frequencySeconds;
	int m_frameCount;
};

class AnimatorComponent : public EntityComponent
{
public:
	void ShutDown() override;

	void Setup(std::shared_ptr<SpriteComponent> spriteComponent);
	void Update(float deltaTime) override;
	void AddAnimation(const std::string& name, const std::vector<std::string>& texturePaths, float frequencySeconds);
	void SetAnimation(const std::string& name);

private:
	std::shared_ptr<SpriteComponent> m_spriteComponent = nullptr;

	Animation* m_currentAnimation = nullptr;
	std::map<std::string, Animation> m_nameToAnimation;

	int m_currentSpriteIndex = 0;
	float m_animationTimer = 0;
};