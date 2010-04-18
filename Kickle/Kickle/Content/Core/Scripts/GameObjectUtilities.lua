--GameObject Utilities

-- Direction constants for GameObjects
UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

-- Tile constants for setting tiles
ICE = 1
WATER = 10

local function GetTileInDir( tileX, tileY, dir )
	if ( dir == UP ) then
		tileY = tileY - 1
	elseif ( dir == DOWN ) then
		tileY = tileY + 1
	elseif ( dir == LEFT ) then
		tileX = tileX - 1
	else
		tileX = tileX + 1
	end

	return tileX, tileY
end


function GetTileObjectFaces( GameObject )
	return GetTileInDir(
          GameObject:GetTileX(),
          GameObject:GetTileY(),
          GameObject:GetDir()
         )
end


function GetTileInDirection( GameObject, dir )
	return GetTileInDir(
          GameObject:GetTileX(),
          GameObject:GetTileY(),
          dir
         )
end


function GetNextDir( dir )
  if ( dir == UP ) then
    return DOWN
  elseif ( dir == DOWN ) then
    return LEFT
  elseif ( dir == LEFT ) then
    return RIGHT
  else
    return UP
  end
end
