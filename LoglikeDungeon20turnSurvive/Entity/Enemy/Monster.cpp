#include "Monster.h"
#include <cmath>

ChaseStep Monster::CalcChaseStep(Position playerPos) const
{
	ChaseStep result{};
	Position cur = _position;

	int dx = playerPos.x - cur.x;
	int dy = playerPos.y - cur.y;

	// 이미 같은 칸이면 이동하지 않음
	if (dx == 0 && dy == 0) return result;

	// x/y 차이가 큰 축으로 1칸 추적 (맨해튼)
	if (std::abs(dx) >= std::abs(dy))
	{
		result.stepX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
	}
	else
	{
		result.stepY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
	}

	result.shouldMove = true;
	return result;
}



void Monster::ApplyMove(int stepX, int stepY)
{
	_position.x += stepX;
	_position.y += stepY;
}

