#pragma once

#include <string>
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Entity/Enemy/Enemy.h"
#include "Item/Item.h"
#include "Systems/Input/Input.h"
#include "Systems/Log/Log.h"
#include "Systems/Spawner/EntitySpawner.h"
#include "Systems/Render/GameRenderer.h"
#include "Type/GameState.h"

// 게임 루프, 턴, 승패
class Game
{
private:
	Map _map;
	Player _player;
	Enemy _enemies;
	Item _items;
	Log _log;
	Input _input;
	EntitySpawner _spawner;
	GameRenderer _renderer;

	int _turn = 1;
	bool _running = true;
	bool _bossSpawned = false;
	bool _wonByBoss = false;
	GameState _state = GameState::GAME_STATE_PLAYING;
	std::string _endMessage;

public:
	Game();
	~Game() = default;

public:
	void Init();
	void Run();

private:
	void HandlePlayerAction(Action action);
	void TryMovePlayer(int dx, int dy);
	void TryUsePotion();

private:
	void EndTurn();
	bool CheckEndCondition();
	void SetVictory(bool byBoss);
	void SetDefeat();

};
