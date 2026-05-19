#pragma once

#include <iostream>
#include <string>

enum class Action
{
	ACTION_NONE,
	ACTION_MOVE_UP,
	ACTION_MOVE_DOWN,
	ACTION_MOVE_LEFT,
	ACTION_MOVE_RIGHT,
	ACTION_INTERACT,	// E: 아이템 줍기 / 포션 사용
	ACTION_USE_POTION,	// H: 포션 사용
	ACTION_QUIT
};

class Input
{
public:
	Input() = default;
	~Input() = default;

public:
	Action ReadInput();
};
