#pragma once
#include "Entity.h"

class Empty : public Entity
{
	// Geerbt �ber Entity
	void EntityInit() override;
	void DestroyDerived() override;
};

