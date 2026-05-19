#pragma once

#include <string>
#include "Type/GameState.h"
#include "Type/Tile.h"

class Map;
class Player;
class Enemy;
class Log;

// 맵·스탯·종료 화면 출력
class GameRenderer
{
public:
	static constexpr int kSurvivalTurns = 20;

public:
	GameRenderer() = default;
	~GameRenderer() = default;

public:
	void Render(const Map& map, const Player& player, const Enemy& enemies,
		const Log& log, int turn) const;

public:
	void RenderPlayerStats(const Player& player, int turn) const;

public:
	void RenderEndScreen(GameState state, const std::string& endMessage,
		int turn, const Player& player) const;

private:
	static void ClearScreen();

private:
	static char TileToChar(Tile tile);

};
