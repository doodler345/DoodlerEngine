#pragma once

#include "Entity.h"

class InteractableUIEntity : public Entity
{
public:
	virtual void SetInteractable(bool isInteractable) = 0;
	bool IsInteractable() const { return m_isInteractable; }

protected:
	bool m_isInteractable = true;
};
