#pragma once
//types of tiles
enum class TileType : uint8_t {
	TILE,
	WALL,
	START,
	END,
	PATH,
	OPEN,
	OLDPATH
};