#include "Player.h"
#include "Systems/Map/Map.h"
#include "Type/Tile.h"

using namespace std;

void Player::Update()
{
}

void Player::Render()
{
}

void Player::Dead()
{
	__super::Dead();
}

bool Player::TryUsePotion()
{
	if (_potionCount <= 0)
		return false;

	if (_currentHealth >= _maxHealth)
		return false;

	_potionCount -= 1;

	int resultHealth = _currentHealth + 10;

	if (resultHealth > _maxHealth)
		resultHealth = _maxHealth;

	_currentHealth = resultHealth;
	return true;
}

PlayerMoveResult Player::TryMove(const Map& map, int directionX, int directionY)
{
	Position current = _position;
	Position next{ current.x + directionX, current.y + directionY };

	if (!map.IsInside(next))
		return PlayerMoveResult::BlockedOutside;

	if (map.GetTile(next) == Tile::TILE_WALL)
		return PlayerMoveResult::BlockedWall;

	_position = next;
	return PlayerMoveResult::Moved;
}

bool Player::TryPickupItem(Map& map)
{
	if (map.GetTile(_position) != Tile::TILE_ITEM) return false;

	AddPotion(1);
	map.SetTile(_position, Tile::TILE_EMPTY);
	return true;
}
