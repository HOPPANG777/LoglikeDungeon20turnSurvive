#pragma once

struct Position
{
	int x;
	int y;

	bool operator<(const Position& other) const
	{
		if (y != other.y) return y < other.y;

		return x < other.x;
	}

	bool operator==(const Position& other) const
	{
		return x == other.x &&
			   y == other.y;
	}
};




