#pragma once

#include <iostream>
#include <string>

enum class Action
{
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Interact,
    Quit
};

class Input
{
public:
	Input() = default;
	~Input() = default;

public:
    Action ReadInput();

};

