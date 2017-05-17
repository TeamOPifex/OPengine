#include "./Human/include/Input/OPgamePadSystem.h"

OPgamePadSystem OPGAMEPADS = {
	OPgamePad(0),
	OPgamePad(1),
	OPgamePad(2),
	OPgamePad(3),
	OPgamePad(4),
	OPgamePad(5),
	OPgamePad(6),
	OPgamePad(7)
};

OPgamePad* OPgamePadSystem::Get(OPgamePadIndex index) {
	return &gamePads[(ui32)index];
}

void OPgamePadSystem::Update(OPtimer* timer)
{
	for (OPint i = (ui32)OPgamePadIndex::_MAX; i--; )
	{
		gamePads[i].Update(timer);
	}
}

void OPgamePadSystem::Reset()
{
	for (OPint i = (ui32)OPgamePadIndex::_MAX; i--; )
	{
		gamePads[i].Reset();
	}
}

void OPgamePadSystem::SetDeadzones(OPfloat deadzone)
{
	for (OPint i = (ui32)OPgamePadIndex::_MAX; i--; )
	{
		gamePads[i].SetDeadzone(deadzone);
	}
}