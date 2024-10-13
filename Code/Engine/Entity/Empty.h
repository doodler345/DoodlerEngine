#pragma once
#include "Entity.h"

class Empty : public Entity
{
	// Geerbt über Entity
	void EntityInit() override;
	void DestroyDerived() override;
};

