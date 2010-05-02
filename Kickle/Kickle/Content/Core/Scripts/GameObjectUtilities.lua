--GameObject Utilities

-- Direction constants for GameObjects
UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

local function GetTileInDir( dir, tileX, tileY )
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
	return GetTileInDir( GameObject:GetDir(), GameObject:GetTile())
end


function GetTileInDirection( GameObject, dir )
	return GetTileInDir( dir, GameObject:GetTile() )
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
