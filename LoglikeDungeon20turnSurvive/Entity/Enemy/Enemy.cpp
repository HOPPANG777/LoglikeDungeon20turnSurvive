#include "Enemy.h"
#include "Systems/Map/Map.h"
#include "Entity/Player/Player.h"
#include "Systems/Log/Log.h"
#include "Systems/Combat/Combat.h"
#include "Type/Tile.h"

using namespace std;

// 몬스터·위치 캐시 초기화
void Enemy::Clear()
{
	_monsters.clear();
	_monsterPositions.clear();
}

// 몬스터 1마리 추가 및 위치 등록
void Enemy::AddMonster(Monster monster)
{
	_monsterPositions.insert(monster.GetPosition());
	_monsters.push_back(std::move(monster));
}

// 해당 칸에 살아 있는 몬스터가 있는지
bool Enemy::HasMonsterAt(Position pos) const
{
	return _monsterPositions.contains(pos);
}

// 해당 칸의 몬스터 포인터 (없으면 nullptr)
const Monster* Enemy::FindMonsterAt(Position pos) const
{
	for (const auto& monster : _monsters)
	{
		if (!monster.IsDead() && monster.GetPosition() == pos)
			return &monster;
	}

	return nullptr;
}

// 좌표로 몬스터 찾기
Monster* Enemy::FindMonsterAt(int x, int y)
{
	for (auto& monster : _monsters)
	{
		if (monster.GetPosition().x == x && monster.GetPosition().y == y)
			return &monster;
	}

	return nullptr;
}

// 위치로 몬스터 인덱스 (없으면 -1)
int Enemy::FindIndexAt(Position pos) const
{
	for (int i = 0; i < static_cast<int>(_monsters.size()); ++i)
	{
		if (_monsters[i].GetPosition() == pos)
			return i;
	}

	return -1;
}

// 다른 몬스터가 점유한 칸인지 (skipIndex 제외)
bool Enemy::IsOccupied(Position pos, int skipIndex) const
{
	for (int i = 0; i < static_cast<int>(_monsters.size()); ++i)
	{
		if (i == skipIndex)
			continue;

		if (!_monsters[i].IsDead() && _monsters[i].GetPosition() == pos)
			return true;
	}

	return false;
}

// 살아 있는 몬스터 위치로 캐시 재구성
void Enemy::SyncPositions()
{
	_monsterPositions.clear();

	for (const auto& monster : _monsters)
	{
		if (!monster.IsDead())
			_monsterPositions.insert(monster.GetPosition());
	}
}

// 몬스터 AI 이동 후 플레이어와 접촉 시 전투
void Enemy::Update(Map& map, Player& player, Log& log)
{
	for (size_t i = 0; i < _monsters.size(); ++i)
	{
		if (_monsters[i].IsDead())
			continue;

		TryMoveMonster(_monsters[i], map, player);

		if (player.IsDead())
			return;

		if (_monsters[i].GetPosition() == player.GetPosition())
		{
			log.Add(_monsters[i].GetName() + " attacks!");
			CombatWithMonster(static_cast<int>(i), player, log);
		}
	}

	SyncPositions();
}

// 사망 몬스터 제거, 보스 처치 여부 반환
bool Enemy::RemoveDead(bool& bossKilled)
{
	bossKilled = false;

	for (int i = static_cast<int>(_monsters.size()) - 1; i >= 0; --i)
	{
		if (!_monsters[i].IsDead())
			continue;

		if (_monsters[i].IsBoss())
			bossKilled = true;

		_monsterPositions.erase(_monsters[i].GetPosition());
		_monsters.erase(_monsters.begin() + i);
	}

	return bossKilled;
}

// 플레이어가 처치한 몬스터 처리
void Enemy::OnKilled(int monsterIndex, Log& log, bool& bossKilled)
{
	if (monsterIndex < 0 || monsterIndex >= static_cast<int>(_monsters.size()))
		return;

	log.Add(_monsters[monsterIndex].GetName() + " died.");

	if (_monsters[monsterIndex].IsBoss())
		bossKilled = true;

	_monsterPositions.erase(_monsters[monsterIndex].GetPosition());
	_monsters.erase(_monsters.begin() + monsterIndex);
}

// 몬스터 1마리 추적 이동
void Enemy::TryMoveMonster(Monster& monster, Map& map, const Player& player)
{
	ChaseStep step = monster.CalcChaseStep(player.GetPosition());

	if (!step.shouldMove)
		return;

	Position cur = monster.GetPosition();
	Position next{ cur.x + step.stepX, cur.y + step.stepY };

	if (!map.IsInside(next))
		return;

	if (map.GetTile(next) == Tile::TILE_WALL)
		return;

	if (next == player.GetPosition())
	{
		_monsterPositions.erase(cur);
		monster.ApplyMove(step.stepX, step.stepY);
		_monsterPositions.insert(monster.GetPosition());
		return;
	}

	int selfIndex = -1;

	for (size_t i = 0; i < _monsters.size(); ++i)
	{
		if (&_monsters[i] == &monster)
		{
			selfIndex = static_cast<int>(i);
			break;
		}
	}

	if (IsOccupied(next, selfIndex))
		return;

	_monsterPositions.erase(cur);
	monster.ApplyMove(step.stepX, step.stepY);
	_monsterPositions.insert(monster.GetPosition());
}

// 플레이어와 몬스터 교환 전투
void Enemy::CombatWithMonster(int monsterIndex, Player& player, Log& log)
{
	FightWithPlayer(monsterIndex, player, log);
}

// 플레이어가 몬스터 칸으로 이동해 전투할 때
void Enemy::FightWithPlayer(int monsterIndex, Player& player, Log& log)
{
	while (!player.IsDead() && monsterIndex < static_cast<int>(_monsters.size())
		&& !_monsters[monsterIndex].IsDead())
	{
		Combat::PerformExchange(player, _monsters[monsterIndex], log);
	}
}
