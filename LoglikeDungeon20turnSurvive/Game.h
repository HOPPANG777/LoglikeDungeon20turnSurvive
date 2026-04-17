#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <random>
#include <set>
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Entity/Enemy/Monster.h"
#include "Systems/Input/Input.h"
#include "Utilities/Random.h"
#include "Type/Tile.h"

class Game
{
private:
    Map _map;
    Player _player;
    Input _input;

    int _turn = 1;
    bool _running = true;

    std::deque<std::string> _log; // 최근 5줄

    std::vector<Monster> _monsters; // 몬스터 관리 컨테이너

    std::set<Position> _monsterPosition;

public:
    Game();

public:
    void Init();
    void Run();

    void AddLog(const std::string& msg);

private:
    void Render() const;
    void PlayerStatRender() const;

    void HandlePlayerAction(Action a);
    void TryMovePlayer(int dx, int dy);

public:
    void MonsterSpawn(const int count);
    Monster* FindMonsterAt(int x, int y);
    int FindMonsterIndexAt(Position pos);

public:
    void Combat(Entity& attacker, Entity& defender);

private:
    static void ClearScreen();
    static char TileToChar(Tile t);
};

