#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <random>
#include <set>
#include <vector>
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Entity/Enemy/Monster.h"
#include "Systems/Input/Input.h"
#include "Utilities/Random.h"
#include "Type/Tile.h"
#include "Type/GameState.h"

class Game
{
private:
	// 20≈œ ¡¶«—
	static constexpr int kSurvivalTurns = 20;

	Map _map;
	Player _player;
	Input _input;

	int _turn = 1;
	bool _running = true;
	GameState _state = GameState::GAME_STATE_PLAYING;
	std::string _endMessage;

	std::deque<std::string> _log;

	std::vector<Monster> _monsters;
	std::set<Position> _monsterPosition;
	std::vector<Position> _items;

	bool _bossSpawned = false;
	bool _wonByBoss = false;

public:
	Game();

public:
	void Init();
	void Run();

	void AddLog(const std::string& msg);

private:
	void Render() const;
	void PlayerStatRender() const;
	void RenderEndScreen() const;

	void HandlePlayerAction(Action a);
	void TryMovePlayer(int dx, int dy);

	void UpdateMonsters();
	void TryMoveMonster(Monster& monster);
	void RemoveDeadMonsters();
	void SyncMonsterPositions();

	void SpawnBoss();
	void ItemSpawn(int count);
	bool HasItemAt(Position pos) const;
	void RemoveItemAt(Position pos);
	bool TryPickupItem();
	void TryUsePotion();

	void EndTurn();
	bool CheckEndCondition();
	void SetVictory(bool byBoss);
	void SetDefeat();

	void PerformExchangeCombat(Entity& attacker, Entity& defender);
	void CombatWithMonster(int monsterIndex);
	void OnMonsterKilled(int monsterIndex);

public:
	void MonsterSpawn(const int count);
	Monster* FindMonsterAt(int x, int y);
	int FindMonsterIndexAt(Position pos);

	bool IsOccupiedByMonster(Position pos, int skipIndex = -1) const;

private:
	static void ClearScreen();
	static char TileToChar(Tile t);

};

