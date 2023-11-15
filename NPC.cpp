#include "NPC.h"

NPC::NPC()
{
	AddComponent(AI_ = shared_ptr<AIMoveComponent>(new AIMoveComponent(this)));

}