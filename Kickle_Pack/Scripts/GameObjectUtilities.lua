--GameObject Utilities
math.randomseed( os.time() )

-- Direction constants for GameObjects
UP = 0
DOWN = 1
LEFT = 2
RIGHT = 3

-- Returns tile in direction passed relative to tile location passed
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


-- Returns tile GameObject is facing
function GetTileObjectFaces( GameObject )
	return GetTileInDir( GameObject:GetDir(), GameObject:GetTile())
end


-- Returns tile in in direction relative to GameObject passed
function GetTileInDirection( GameObject, dir )
	return GetTileInDir( dir, GameObject:GetTile() )
end


-- Returns next logical direction via order of Up, Down, Left, Right
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


-- Pass a table of dirs that you don't want included in the selection.
-- Returns a random dir out of dirs not in 'dirsNotToUse'
function GetRandomDir( dirsNotToUse )
  local dirs = { UP, DOWN, LEFT, RIGHT }

  for i, badDir in ipairs( dirsNotToUse ) do
    for j, dir in ipairs( dirs ) do
      if dir == badDir then
        table.remove( dirs, j )
      end
    end
  end
      
  local i = math.random( #dirs )
  return dirs[i]
end


-- Generic enemy AI for a GameObject to use. Tries its hardest to get
-- to Kickle, if all else fails tries to find any direction it can 
-- possibly move.
function GenericEnemyAI( enemy )
	local Kickle = Game.GetGameObject( "Kickle" )

	if ( Kickle ) then
		local enemyX, enemyY = enemy:GetTile()
    local KickleX, KickleY = Kickle:GetTile()

		local distanceX = math.abs( enemyX - KickleX )
		local distanceY = math.abs( enemyY - KickleY )

		local dir = UP;

		if ( distanceX > distanceY ) then
			if ( enemyX < KickleX ) then
				dir = RIGHT
			elseif ( enemyX > KickleX ) then
				dir = LEFT
			else
				dir = math.random( LEFT, RIGHT )
			end
		else
			if ( enemyY < KickleY ) then
				dir = DOWN
			elseif ( enemyY > KickleY ) then
				dir = UP
			else
				dir = math.random( UP, DOWN )
			end
		end

		enemy:SetDir( dir )

    local dirsTried = {}
    while #dirsTried  < 4 and not enemy:Move() do
      dirsTried[#dirsTried+1] = dir
      dir = GetRandomDir( dirsTried )
      enemy:SetDir( dir )
    end

		enemy:SetAnimation( dir )
	end
end

