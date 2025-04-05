#pragma once

#include <string>

class Entity;

class EntityComponent
{
public:
	EntityComponent();
	~EntityComponent();

	virtual void Update(float deltaTime) {};
	virtual void ShutDown() = 0;

	Entity* GetOwner() { return m_ownerEntity; }
	std::string m_name;
	int m_id = -1;

	bool m_allowMultiple = false;

	static int s_componentIDCounter;
protected:
	Entity* m_ownerEntity = nullptr;
};



#include "../Engine.h"
#include "../RenderSystem.h"

class DrawableEntityComponent : public EntityComponent
{
public:
	virtual void SetVisibility(bool value) = 0;
	virtual void SetRenderLayer(int layer) = 0;

protected:
	RenderSystem* m_renderSystem = nullptr;
	bool m_isVisible = true;
	int m_renderLayer = 0;
};