
#include "Game.h"

using namespace std;

Game::Game()
	: _player("Player", 30, 4, 1, 5)
{
}

void Game::Init()
{
	_map.MapInit();

	// 플레이어 시작 위치: 벽이 아닌 곳
	_player.SetPosition(1, 1);

	// 몬스터 시작 위치: 벽이 아닌 곳
	MonsterSpawn(3);

	_log.clear();
	for (const auto& _m : _monsters) AddLog("Monster Spawn : " + to_string(_m.GetPosition().x) + ", " + to_string(_m.GetPosition().y));
	AddLog("Game Start!");
}

void Game::Run()
{
	while (_running)
	{
		Render();

		cout << "\n> ";
		cout.flush();

		Action a = _input.ReadInput();
		HandlePlayerAction(a);

		// 지금 단계에서는 몬스터/턴 규칙 아직 안 붙임.
		// 일단 “입력 → 이동 → 화면 갱신”이 목적.
		if (_running)
			++_turn;
	}
}

void Game::HandlePlayerAction(Action a)
{
	switch (a)
	{
	case Action::MoveUp:    TryMovePlayer(0, -1); break;
	case Action::MoveDown:  TryMovePlayer(0, 1); break;
	case Action::MoveLeft:  TryMovePlayer(-1, 0); break;
	case Action::MoveRight: TryMovePlayer(1, 0); break;

	case Action::Interact:
		AddLog("Interact (not implemented yet).");
		break;

	case Action::Quit:
		_running = false;
		AddLog("Quit.");
		break;

	case Action::None:
	default:
		AddLog("No action.");
		break;
	}
}

void Game::TryMovePlayer(int dx, int dy)
{
	Position cur = _player.GetPosition();
	Position next{ cur.x + dx, cur.y + dy };

	if (!_map.IsInside(next))
	{
		AddLog("Blocked: outside map.");
		return;
	}

	if (_map.GetTile(next) == Tile::TILE_WALL)
	{
		AddLog("Blocked: wall.");
		return;
	}

	int idx = FindMonsterIndexAt(next);

	if (idx != -1)
	{
		while (!_player.IsDead() && !_monsters[idx].IsDead())
		{
			Combat(_player, _monsters[idx]);
		}

		if (_player.IsDead())
		{
			HandlePlayerAction(Action::Quit);
		}

		{
			AddLog(_monsters[idx].GetName() + "가 사망했습니다.");

			Position deadPos = _monsters[idx].GetPosition();

			_monsterPosition.erase(deadPos);

			_monsters.erase(_monsters.begin() + idx);

			return; // 여기서 끝
		}
		
	}

	_player.SetPosition(next.x, next.y);
	AddLog("Player moved.");
}

void Game::MonsterSpawn(const int count)
{
	// 몬스터를 count만큼 생성합니다.
	for (int i = 0; i < count; i++)
	{
		while (true)
		{
			int x = Random::Range(0, Map::width - 1);
			int y = Random::Range(0, Map::height - 1);

			Tile tile = _map.GetTile(x, y);

			if (tile == Tile::TILE_WALL) continue;

			int playerX = _player.GetPosition().x;
			int playerY = _player.GetPosition().y;

			if (x == playerX && y == playerY) continue;

			bool overlap = false;

			for (size_t j = 0; j < _monsters.size(); j++)
			{
				if (_monsters[j].GetPosition().x == x &&
					_monsters[j].GetPosition().y == y)
				{
					overlap = true;
					continue;
				}
			}

			if (!overlap)
			{
				Monster monster("monster" + to_string(i), 8, 1, 1);
				monster.SetPosition(x, y);

				_monsters.push_back(monster);
				_monsterPosition.insert(monster.GetPosition());
				break;
			}
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
	for (int i = 0; i < _monsters.size(); i++)
	{
		if (_monsters[i].GetPosition() == pos)
		{
			return i;
		}
	}

	return -1;
}

void Game::Combat(Entity& attacker, Entity& defender)
{
	defender.TakeDamage(attacker.GetATK());
	AddLog(defender.GetName() + "가 " + to_string(attacker.GetATK()) + "만큼 데미지를 입었습니다.");

	if (!defender.IsDead())
	{
		attacker.TakeDamage(defender.GetATK());
		AddLog(attacker.GetName() + "가 " + to_string(defender.GetATK()) + "만큼 데미지를 입었습니다.");
	}

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
			if (p.x == x && p.y == y)
			{
				cout << 'P';
			}
			else if (_monsterPosition.contains({ x,y }))
			{
				cout << 'M';
			}
			else
			{
				cout << TileToChar(_map.GetTile(x, y));
			}
		}
		cout << "\n";
	}

	bool hasMonster = false;

	cout << "\n[Log]\n";
	for (const auto& line : _log)
		cout << "- " << line << "\n";

	cout << "\nControls: W/A/S/D move, E interact, Q quit\n";
}

void Game::PlayerStatRender() const
{
	cout << "Turn: " << _turn << "\n";
	cout << "HP: " << _player.GetHP() << "/" << _player.GetMaxHP()
		<< "  ATK: " << _player.GetATK()
		<< "  DEF: " << _player.GetDEF()
		<< "  Potions: " << _player.GetPotionCount()
		<< "\n\n";
}

void Game::AddLog(const string& msg)
{
	_log.push_back(msg);
	while (_log.size() > 5)
		_log.pop_front();
}

void Game::ClearScreen()
{
	// ANSI clear screen (대부분 터미널에서 동작)
	cout << "\x1B[2J\x1B[H";
}

char Game::TileToChar(Tile t)
{
	switch (t)
	{
	case Tile::TILE_WALL:  return '#';
	case Tile::TILE_EMPTY: return '.';
	default:          return '?';
	}
}
