#include "EntityComponent.h"

EntityComponent::EntityComponent()
{
	m_id = s_componentIDCounter++;
}

EntityComponent::~EntityComponent()
{
	//DebugPrint("EntityComponent " + m_name + " destroyed", TextColor::Yellow, DebugChannel::EntityComponent, __FILE__, __LINE__);
}

int EntityComponent::s_componentIDCounter = 0;

