#include "Game.h"
#include "Type/Position.h"
#include <iostream>
#include <string>

using namespace std;

// 플레이어 기본 스탯으로 게임 객체 생성
Game::Game()
	: _player("Player", 30, 4, 1)
{
}

// 맵, 엔티티, 턴, 로그 초기화
void Game::Init()
{
	_map.MapInit();

	_player.SetPosition(1, 1);

	_enemies.Clear();
	_items.Clear();
	_bossSpawned = false;
	_wonByBoss = false;
	_state = GameState::GAME_STATE_PLAYING;
	_turn = 1;
	_running = true;
	_endMessage.clear();

	_log.Clear();

	_spawner.SpawnMonsters(_map, _player, _enemies, _items, 3, _log);
	_spawner.SpawnItems(_map, _player, _enemies, _items, 3);
	_log.Add("Game start!");
}

// 메인 게임 루프 (출력 -> 입력 -> 몬스터 -> 턴 -> 승패)
void Game::Run()
{
	while (_running && _state == GameState::GAME_STATE_PLAYING)
	{
		_renderer.Render(_map, _player, _enemies, _log, _turn);

		cout << "\n> ";
		cout.flush();

		Action action = _input.ReadInput();

		if (action == Action::ACTION_QUIT)
		{
			_running = false;
			_log.Add("Quit.");
			break;
		}

		HandlePlayerAction(action);

		if (_state != GameState::GAME_STATE_PLAYING)
			break;

		if (_player.IsDead())
		{
			SetDefeat();
			break;
		}

		_enemies.Update(_map, _player, _log);

		bool bossKilled = false;
		if (_enemies.RemoveDead(bossKilled) && bossKilled)
			SetVictory(true);

		if (_player.IsDead())
		{
			SetDefeat();
			break;
		}

		EndTurn();

		if (CheckEndCondition())
			break;
	}

	if (_state == GameState::GAME_STATE_WON || _state == GameState::GAME_STATE_LOST)
	{
		_renderer.RenderEndScreen(_state, _endMessage, _turn, _player);

		string dummy;
		getline(cin, dummy);
	}
}

// 플레이어 입력에 따른 행동 분기
void Game::HandlePlayerAction(Action action)
{
	switch (action)
	{
	case Action::ACTION_MOVE_UP:    TryMovePlayer(0, -1); break;
	case Action::ACTION_MOVE_DOWN:  TryMovePlayer(0, 1); break;
	case Action::ACTION_MOVE_LEFT:  TryMovePlayer(-1, 0); break;
	case Action::ACTION_MOVE_RIGHT: TryMovePlayer(1, 0); break;

	case Action::ACTION_INTERACT:
		if (_items.TryPickup(_map, _player, _player.GetPosition()))
			_log.Add("Potion picked up.");
		else
		{
			if (!_items.HasAt(_map, _player.GetPosition()))
				_log.Add("No item to pick up.");
			TryUsePotion();
		}
		break;

	case Action::ACTION_USE_POTION:
		TryUsePotion();
		break;

	case Action::ACTION_NONE:
	default:
		_log.Add("Invalid input.");
		break;
	}
}

// 플레이어 이동 또는 몬스터와 전투
void Game::TryMovePlayer(int dx, int dy)
{
	Position cur = _player.GetPosition();
	Position next{ cur.x + dx, cur.y + dy };

	int monsterIndex = _enemies.FindIndexAt(next);

	if (monsterIndex != -1)
	{
		_enemies.FightWithPlayer(monsterIndex, _player, _log);

		if (_player.IsDead())
		{
			SetDefeat();
			return;
		}

		if (monsterIndex < static_cast<int>(_enemies.GetMonsters().size())
			&& _enemies.GetMonsters()[monsterIndex].IsDead())
		{
			bool bossKilled = false;
			_enemies.OnKilled(monsterIndex, _log, bossKilled);

			if (bossKilled)
				SetVictory(true);
		}

		return;
	}

	switch (_player.TryMove(_map, dx, dy))
	{
	case PlayerMoveResult::BlockedOutside:
		_log.Add("Blocked: outside map.");
		break;
	case PlayerMoveResult::BlockedWall:
		_log.Add("Blocked: wall.");
		break;
	case PlayerMoveResult::Moved:
		_log.Add("Player moved.");
		break;
	}
}

// 포션 사용 시도 및 로그
void Game::TryUsePotion()
{
	if (_player.TryUsePotion())
		_log.Add("Potion used. HP restored.");
	else if (_player.GetPotionCount() <= 0)
		_log.Add("No potions left.");
	else
		_log.Add("HP is already full.");
}

// 턴 증가 및 20턴 보스 스폰
void Game::EndTurn()
{
	++_turn;

	if (_turn >= GameRenderer::kSurvivalTurns && !_bossSpawned)
	{
		_bossSpawned = _spawner.TrySpawnBoss(_map, _player, _enemies, _items, _log);
	}
}

// 생존, 사망, 승리 조건 검사
bool Game::CheckEndCondition()
{
	if (_player.IsDead())
	{
		SetDefeat();
		return true;
	}

	if (_state == GameState::GAME_STATE_WON)
		return true;

	// 보스 등장 후에는 처치 승리만 가능 (생존 승리 없음)
	return _state != GameState::GAME_STATE_PLAYING;
}

// 승리 처리 (보스 처치)
void Game::SetVictory(bool byBoss)
{
	_state = GameState::GAME_STATE_WON;
	_wonByBoss = byBoss;
	_running = false;

	_endMessage = "Victory! defeat the Boss!.";
	_log.Add("Victory! Boss defeated.");
}

// 패배 처리
void Game::SetDefeat()
{
	_state = GameState::GAME_STATE_LOST;
	_running = false;
	_endMessage = "Falie! HP is gone.";
	_log.Add("Defeat! HP reached 0.");
}
