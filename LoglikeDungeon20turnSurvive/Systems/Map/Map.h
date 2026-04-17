#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Entity/Player/Player.h"
#include "Systems/Input/Input.h"
#include "Type/Tile.h"
#include "Type/Position.h"

class Map
{
private:
	std::vector<Tile> _map;

public:
	static constexpr int width = 10;
	static constexpr int height = 10;

private:
	static int ToIndex(int x, int y) { return y * width + x; }

public:
	Map();
	~Map() = default;

public:
	void MapInit();

public:
	bool IsInside(int x, int y) const;
	bool IsInside(const Position& p) const;

public:
	Tile GetTile(int x, int y) const;
	Tile GetTile(const Position& p) const;

public:
	void SetTile(int x, int y, Tile t);
	void SetTile(const Position& p, Tile t);

public:
	const std::vector<Tile>& Raw() const { return _map; } // 디버그/출력용(선택)

};

