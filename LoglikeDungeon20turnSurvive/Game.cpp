
#include "Game.h"
#include <algorithm>
#include <cmath>

using namespace std;

Game::Game()
	: _player("Player", 30, 4, 1)
{
}

void Game::Init()
{
	_map.MapInit();

	// 플레이어 시작 위치 -> 벽이 아닌 곳
	_player.SetPosition(1, 1);

	// 게임 상태 초기화
	_monsters.clear();
	_monsterPosition.clear();
	_items.clear();
	_bossSpawned = false;
	_wonByBoss = false;
	_state = GameState::GAME_STATE_PLAYING;
	_turn = 1;
	_running = true;
	_endMessage.clear();

	// 몬스터 시작 위치 -> 벽이 아닌 곳
	MonsterSpawn(3);

	// 아이템 3개 배치
	ItemSpawn(3);

	_log.clear();
	for (const auto& m : _monsters)
		AddLog("Monster Spawn : " + to_string(m.GetPosition().x) + ", " + to_string(m.GetPosition().y));
	AddLog("Game Start!");
}

void Game::Run()
{
	// 출력 → 입력 → 몬스터 → 턴 증가 → 승패
	while (_running && _state == GameState::GAME_STATE_PLAYING)
	{
		// 맵 + 상태 출력
		Render();

		cout << "\n> ";
		cout.flush();

		// 플레이어 입력 처리
		Action a = _input.ReadInput();

		// 종료는 턴을 소모하지 않음
		if (a == Action::ACTION_QUIT)
		{
			_running = false;
			AddLog("Quit.");
			break;
		}

		HandlePlayerAction(a);

		// 보스 처치 등으로 이미 승리한 경우
		if (_state != GameState::GAME_STATE_PLAYING)
			break;

		// 플레이어 사망 시 패배
		if (_player.IsDead())
		{
			SetDefeat();
			break;
		}

		// 몬스터 이동 + 접촉 전투
		UpdateMonsters();
		RemoveDeadMonsters();

		if (_player.IsDead())
		{
			SetDefeat();
			break;
		}

		// 턴 증가, 20턴 보스 등장
		EndTurn();

		// 승패 판정
		if (CheckEndCondition())
			break;
	}

	// 종료 화면
	if (_state == GameState::GAME_STATE_WON || _state == GameState::GAME_STATE_LOST)
	{
		RenderEndScreen();
		string dummy;
		getline(cin, dummy);
	}
}

void Game::HandlePlayerAction(Action a)
{
	switch (a)
	{
	case Action::ACTION_MOVE_UP:    TryMovePlayer(0, -1); break;
	case Action::ACTION_MOVE_DOWN:  TryMovePlayer(0, 1); break;
	case Action::ACTION_MOVE_LEFT:  TryMovePlayer(-1, 0); break;
	case Action::ACTION_MOVE_RIGHT: TryMovePlayer(1, 0); break;

	case Action::ACTION_INTERACT:
		// E -> 같은 칸 아이템 줍기, 없으면 포션 사용
		if (!TryPickupItem())
			TryUsePotion();
		break;

	case Action::ACTION_USE_POTION:
		TryUsePotion();
		break;

	case Action::ACTION_NONE:
	default:
		AddLog("잘못된 입력입니다.");
		break;
	}
}

void Game::TryMovePlayer(int dx, int dy)
{
	Position cur = _player.GetPosition();
	Position next{ cur.x + dx, cur.y + dy };

	// 이동하려는 칸에 몬스터가 있으면 전투 (Game 판정)
	int idx = FindMonsterIndexAt(next);

	if (idx != -1)
	{
		CombatWithMonster(idx);

		if (_player.IsDead())
		{
			SetDefeat();
			return;
		}

		if (idx < static_cast<int>(_monsters.size()) && _monsters[idx].IsDead())
			OnMonsterKilled(idx);

		return;
	}

	switch (_player.TryMove(_map, dx, dy))
	{
	case PlayerMoveResult::BlockedOutside:
		AddLog("Blocked: outside map.");
		break;
	case PlayerMoveResult::BlockedWall:
		AddLog("Blocked: wall.");
		break;
	case PlayerMoveResult::Moved:
		AddLog("Player moved.");
		break;
	}
}

void Game::PerformExchangeCombat(Entity& attacker, Entity& defender)
{
	attacker.Attack(defender);
	AddLog(defender.GetName() + "가 " + to_string(attacker.GetATK()) + "만큼 데미지를 입었습니다.");

	if (!defender.IsDead())
	{
		defender.Attack(attacker);
		AddLog(attacker.GetName() + "가 " + to_string(defender.GetATK()) + "만큼 데미지를 입었습니다.");
	}
}

void Game::CombatWithMonster(int monsterIndex)
{
	// 한 턴에 1회 교환 -> 둘 중 하나가 죽을 때까지 반복
	while (!_player.IsDead() && monsterIndex < static_cast<int>(_monsters.size())
		&& !_monsters[monsterIndex].IsDead())
	{
		PerformExchangeCombat(_player, _monsters[monsterIndex]);
	}
}

void Game::OnMonsterKilled(int monsterIndex)
{
	if (monsterIndex < 0 || monsterIndex >= static_cast<int>(_monsters.size()))
		return;

	AddLog(_monsters[monsterIndex].GetName() + "가 사망했습니다.");

	Position deadPos = _monsters[monsterIndex].GetPosition();

	if (_monsters[monsterIndex].IsBoss())
		SetVictory(true);

	_monsterPosition.erase(deadPos);
	_monsters.erase(_monsters.begin() + monsterIndex);
}

void Game::UpdateMonsters()
{
	for (size_t i = 0; i < _monsters.size(); ++i)
	{
		if (_monsters[i].IsDead())
			continue;

		TryMoveMonster(_monsters[i]);

		if (_player.IsDead())
			return;

		Position mPos = _monsters[i].GetPosition();
		Position pPos = _player.GetPosition();

		if (mPos == pPos)
		{
			AddLog(_monsters[i].GetName() + "의 공격!");
			CombatWithMonster(static_cast<int>(i));
		}
	}

	SyncMonsterPositions();
}

void Game::TryMoveMonster(Monster& monster)
{
	ChaseStep step = monster.CalcChaseStep(_player.GetPosition());
	if (!step.shouldMove)
		return;

	Position cur = monster.GetPosition();
	Position next{ cur.x + step.stepX, cur.y + step.stepY };

	if (!_map.IsInside(next))
		return;

	if (_map.GetTile(next) == Tile::TILE_WALL)
		return;

	if (next == _player.GetPosition())
	{
		_monsterPosition.erase(cur);
		monster.ApplyMove(step.stepX, step.stepY);
		_monsterPosition.insert(monster.GetPosition());
		return;
	}

	int selfIdx = -1;
	for (size_t i = 0; i < _monsters.size(); ++i)
	{
		if (&_monsters[i] == &monster)
		{
			selfIdx = static_cast<int>(i);
			break;
		}
	}

	if (IsOccupiedByMonster(next, selfIdx))
		return;

	_monsterPosition.erase(cur);
	monster.ApplyMove(step.stepX, step.stepY);
	_monsterPosition.insert(monster.GetPosition());
}

bool Game::IsOccupiedByMonster(Position pos, int skipIndex) const
{
	for (size_t i = 0; i < _monsters.size(); ++i)
	{
		if (static_cast<int>(i) == skipIndex) continue;

		if (!_monsters[i].IsDead() && _monsters[i].GetPosition() == pos) return true;
	}
	return false;
}

void Game::SyncMonsterPositions()
{
	_monsterPosition.clear();
	for (const auto& m : _monsters)
	{
		if (!m.IsDead()) _monsterPosition.insert(m.GetPosition());
	}
}

void Game::RemoveDeadMonsters()
{
	for (int i = static_cast<int>(_monsters.size()) - 1; i >= 0; --i)
	{
		if (_monsters[i].IsDead())
		{
			if (_monsters[i].IsBoss()) SetVictory(true);

			_monsterPosition.erase(_monsters[i].GetPosition());
			_monsters.erase(_monsters.begin() + i);
		}
	}
}

void Game::EndTurn()
{
	++_turn;

	if (_turn >= kSurvivalTurns && !_bossSpawned) SpawnBoss();
}

void Game::SpawnBoss()
{
	if (_bossSpawned)
		return;

	for (int attempt = 0; attempt < 200; ++attempt)
	{
		int x = Random::Range(0, Map::width - 1);
		int y = Random::Range(0, Map::height - 1);
		Position pos{ x, y };

		if (_map.GetTile(pos) == Tile::TILE_WALL) continue;

		Position p = _player.GetPosition();
		if (pos == p) continue;

		if (IsOccupiedByMonster(pos)) continue;

		if (HasItemAt(pos)) continue;

		Monster boss("Boss", 25, 3, 2, true);
		boss.SetPosition(x, y);
		_monsters.push_back(boss);
		_monsterPosition.insert(pos);
		_bossSpawned = true;
		AddLog("보스가 등장했습니다!");
		return;
	}

	AddLog("보스 스폰 실패 (빈 칸 없음).");
}

void Game::ItemSpawn(int count)
{
	int placed = 0;
	int attempts = 0;

	while (placed < count && attempts < 500)
	{
		++attempts;
		int x = Random::Range(0, Map::width - 1);
		int y = Random::Range(0, Map::height - 1);
		Position pos{ x, y };

		if (_map.GetTile(pos) != Tile::TILE_EMPTY) continue;

		if (_player.GetPosition() == pos) continue;

		if (HasItemAt(pos)) continue;

		_items.push_back(pos);
		_map.SetTile(pos, Tile::TILE_ITEM);
		++placed;
	}
}

bool Game::HasItemAt(Position pos) const
{
	return _map.GetTile(pos) == Tile::TILE_ITEM;
}

void Game::RemoveItemAt(Position pos)
{
	_map.SetTile(pos, Tile::TILE_EMPTY);
	_items.erase(remove(_items.begin(), _items.end(), pos), _items.end());
}

bool Game::TryPickupItem()
{
	Position p = _player.GetPosition();

	if (!HasItemAt(p))
	{
		AddLog("줍을 아이템이 없습니다.");
		return false;
	}

	if (!_player.TryPickupItem(_map))
		return false;

	_items.erase(remove(_items.begin(), _items.end(), p), _items.end());
	AddLog("포션을 획득했습니다.");
	return true;
}

void Game::TryUsePotion()
{
	if (_player.TryUsePotion())
		AddLog("포션을 사용했습니다. HP 회복.");
	else if (_player.GetPotionCount() <= 0)
		AddLog("포션이 없습니다.");
	else
		AddLog("현재 체력이 최대 체력과 동일합니다.");
}

bool Game::CheckEndCondition()
{
	if (_player.IsDead())
	{
		SetDefeat();
		return true;
	}

	if (_state == GameState::GAME_STATE_WON)
		return true;

	if (_turn >= kSurvivalTurns && !_player.IsDead())
	{
		SetVictory(false);
		return true;
	}

	return _state != GameState::GAME_STATE_PLAYING;
}

void Game::SetVictory(bool byBoss)
{
	_state = GameState::GAME_STATE_WON;
	_wonByBoss = byBoss;
	_running = false;

	if (byBoss)
		_endMessage = "승리! 보스를 처치했습니다.";
	else
		_endMessage = "승리! 20턴을 생존했습니다.";

	AddLog(_endMessage);
}

void Game::SetDefeat()
{
	_state = GameState::GAME_STATE_LOST;
	_running = false;
	_endMessage = "패배! HP가 0이 되었습니다.";
	AddLog(_endMessage);
}

void Game::MonsterSpawn(const int count)
{
	for (int i = 0; i < count; i++)
	{
		for (int attempt = 0; attempt < 200; ++attempt)
		{
			int x = Random::Range(0, Map::width - 1);
			int y = Random::Range(0, Map::height - 1);

			Position pos{ x, y };

			Tile tile = _map.GetTile(x, y);

			if (tile == Tile::TILE_WALL) continue;

			int playerX = _player.GetPosition().x;
			int playerY = _player.GetPosition().y;

			if (x == playerX && y == playerY) continue;

			if (IsOccupiedByMonster(pos)) continue;

			if (HasItemAt(pos)) continue;

			Monster monster("monster" + to_string(i), 8, 1, 1);
			monster.SetPosition(x, y);

			_monsters.push_back(monster);
			_monsterPosition.insert(pos);
			break;
		}
	}
}

Monster* Game::FindMonsterAt(int x, int y)
{
	for (auto& m : _monsters)
	{
		if (m.GetPosition().x == x && m.GetPosition().y == y)
		{
			return &m;
		}
	}

	return nullptr;
}

int Game::FindMonsterIndexAt(Position pos)
{
	for (int i = 0; i < static_cast<int>(_monsters.size()); i++)
	{
		if (_monsters[i].GetPosition() == pos)
		{
			return i;
		}
	}

	return -1;
}

void Game::Render() const
{
	ClearScreen();

	PlayerStatRender();

	const Position p = _player.GetPosition();

	for (int y = 0; y < Map::height; ++y)
	{
		for (int x = 0; x < Map::width; ++x)
		{
			Position here{ x, y };

			if (p.x == x && p.y == y)
			{
				cout << 'P';
			}
			else if (_monsterPosition.contains(here))
			{
				const Monster* m = nullptr;
				for (const auto& mon : _monsters)
				{
					if (!mon.IsDead() && mon.GetPosition() == here)
					{
						m = &mon;
						break;
					}
				}
				cout << (m && m->IsBoss() ? 'B' : 'M');
			}
			else
			{
				cout << TileToChar(_map.GetTile(x, y));
			}
		}
		cout << "\n";
	}

	cout << "\n[Log]\n";
	for (const auto& line : _log)
		cout << "- " << line << "\n";

	cout << "\nControls: W/A/S/D move, E interact, H potion, Q/ESC quit\n";
}

void Game::PlayerStatRender() const
{
	cout << "Turn: " << _turn << " / " << kSurvivalTurns << "\n";
	cout << "HP: " << _player.GetHP() << "/" << _player.GetMaxHP()
		<< "  ATK: " << _player.GetATK()
		<< "  DEF: " << _player.GetDEF()
		<< "  Potions: " << _player.GetPotionCount()
		<< "\n\n";
}

void Game::RenderEndScreen() const
{
	ClearScreen();
	cout << "\n========================================\n";
	if (_state == GameState::GAME_STATE_WON)
		cout << "  승리\n";
	else
		cout << "  패배\n";
	cout << "========================================\n\n";
	cout << _endMessage << "\n\n";
	cout << "최종 턴: " << _turn << "\n";
	cout << "HP: " << _player.GetHP() << "/" << _player.GetMaxHP() << "\n\n";
	cout << "Enter 키를 누르면 종료합니다...\n";
}

void Game::AddLog(const string& msg)
{
	_log.push_back(msg);
	while (_log.size() > 5)
		_log.pop_front();
}

void Game::ClearScreen()
{
	cout << "\x1B[2J\x1B[H";
}

char Game::TileToChar(Tile t)
{
	switch (t)
	{
	case Tile::TILE_WALL:  return '#';
	case Tile::TILE_ITEM:  return 'I';
	case Tile::TILE_EMPTY: return '.';
	default:          return '?';
	}
}
