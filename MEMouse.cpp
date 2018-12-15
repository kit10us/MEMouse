// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include "MEMouse.h"
#include <memouse/Mouse.h>
#include <me/game/Game.h>

void DeleterMouse( memouse::Mouse * mouse )
{
	delete mouse;
}

extern "C" __declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element );
__declspec(dllexport) bool MELoader( me::game::IGame * gameInstance, const qxml::Element * element )
{
	gameInstance->GetInputManager()->AddInputDevice( me::input::IInputDevice::ptr( new memouse::Mouse( gameInstance ), DeleterMouse ) );

	return true;
}

