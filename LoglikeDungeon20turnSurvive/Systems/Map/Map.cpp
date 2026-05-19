#include "Map.h"

Map::Map()
    : _map(width * height, Tile::TILE_EMPTY)
{
}

bool Map::IsInside(int x, int y) const
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

bool Map::IsInside(const Position& p) const
{
    return IsInside(p.x, p.y);
}

Tile Map::GetTile(int x, int y) const
{
    if (!IsInside(x, y)) return Tile::TILE_WALL;
    return _map[ToIndex(x, y)];
}

Tile Map::GetTile(const Position& p) const
{
    return GetTile(p.x, p.y);
}

void Map::SetTile(int x, int y, Tile t)
{
    if (!IsInside(x, y)) return;
    _map[ToIndex(x, y)] = t;
}

void Map::SetTile(const Position& p, Tile t)
{
    SetTile(p.x, p.y, t);
}

void Map::MapInit()
{
    for (auto& cell : _map)
        cell = Tile::TILE_EMPTY;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const bool border = (x == 0 || x == width - 1 || y == 0 || y == height - 1);
            if (border) SetTile(x, y, Tile::TILE_WALL);
        }
    }

    SetTile(3, 3, Tile::TILE_WALL);
    SetTile(3, 4, Tile::TILE_WALL);
    SetTile(6, 6, Tile::TILE_WALL);
}
