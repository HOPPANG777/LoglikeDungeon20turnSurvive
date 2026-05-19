#include "GameRenderer.h"
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Entity/Enemy/Enemy.h"
#include "Entity/Enemy/Monster.h"
#include "Systems/Log/Log.h"
#include "Type/Position.h"
#include <iostream>

using namespace std;

// 콘솔 화면 지우기
void GameRenderer::ClearScreen()
{
	cout << "\x1B[2J\x1B[H";
}

// 타일 enum을 출력 문자로 변환
char GameRenderer::TileToChar(Tile tile)
{
	switch (tile)
	{
	case Tile::TILE_WALL:  return '#';
	case Tile::TILE_ITEM:  return 'I';
	case Tile::TILE_EMPTY: return '.';
	default:               return '?';
	}
}

// 플레이어 스탯 줄 출력
void GameRenderer::RenderPlayerStats(const Player& player, int turn) const
{
	cout << "Turn: " << turn << " / " << kSurvivalTurns << "\n";
	cout << "HP: " << player.GetHP() << "/" << player.GetMaxHP()
		<< "  ATK: " << player.GetATK()
		<< "  DEF: " << player.GetDEF()
		<< "  Potions: " << player.GetPotionCount()
		<< "\n\n";
}

// 맵·로그·조작 안내 출력
void GameRenderer::Render(const Map& map, const Player& player, const Enemy& enemies,
	const Log& log, int turn) const
{
	ClearScreen();

	RenderPlayerStats(player, turn);

	const Position playerPos = player.GetPosition();

	for (int y = 0; y < Map::height; ++y)
	{
		for (int x = 0; x < Map::width; ++x)
		{
			Position here{ x, y };

			if (playerPos == here)
			{
				cout << 'P';
			}
			else if (enemies.HasMonsterAt(here))
			{
				const Monster* monster = enemies.FindMonsterAt(here);
				cout << (monster && monster->IsBoss() ? 'B' : 'M');
			}
			else
			{
				cout << TileToChar(map.GetTile(x, y));
			}
		}
		cout << "\n";
	}

	log.Render();

	cout << "\nControls: W/A/S/D move, E interact, H potion, Q/ESC quit\n";
}

// 승패 종료 화면 출력
void GameRenderer::RenderEndScreen(GameState state, const string& endMessage,
	int turn, const Player& player) const
{
	ClearScreen();
	cout << "\n========================================\n";

	if (state == GameState::GAME_STATE_WON)
		cout << "  Victory!\n";
	else
		cout << "  Failed\n";

	cout << "========================================\n\n";
	cout << endMessage << "\n\n";
	cout << "Final Turn: " << turn << "\n";
	cout << "HP: " << player.GetHP() << "/" << player.GetMaxHP() << "\n\n";
	cout << "Enter key pressed the end...\n";
}
