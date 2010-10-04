--Object Utilities
math.randomseed( os.time() )

local Util = {}

-- Direction constants for Objects
Util.UP = 0
Util.DOWN = 1
Util.LEFT = 2
Util.RIGHT = 3

-- Returns tile in direction passed relative to tile location passed
function Util.GetTileInDir( dir, tileX, tileY )
        if ( dir == Util.UP ) then
                tileY = tileY - 1
        elseif ( dir == Util.DOWN ) then
                tileY = tileY + 1
        elseif ( dir == Util.LEFT ) then
                tileX = tileX - 1
        else
                tileX = tileX + 1
        end

        return tileX, tileY
end


-- Returns tile Object is facing
function Util.GetTileObjectFaces( Object )
        return Util.GetTileInDir( Object:GetDir(), Object:GetTile())
end


-- Returns tile in in direction relative to Object passed
function Util.GetTileInDirection( Object, dir )
        return Util.GetTileInDir( dir, Object:GetTile() )
end


-- Returns next logical direction via order of Up, Down, Left, Right
function Util.GetNextDir( dir )
  if dir == Util.UP then
    return Util.DOWN
  elseif dir == Util.DOWN then
    return Util.LEFT
  elseif dir == Util.LEFT then
    return Util.RIGHT
  else
    return Util.UP
  end
end


-- Returns opposite of direction passed
function Util.GetOppositeDir( dir )
  if dir == Util.UP then
    return Util.DOWN
  elseif dir == Util.DOWN then
    return Util.UP
  elseif dir == Util.LEFT then
    return Util.RIGHT
  elseif dir == Util.RIGHT then
    return Util.LEFT
  else
    return Util.UP
  end
end


-- Pass a table of dirs that you don't want included in the selection.
-- Returns a random dir out of dirs not in 'dirsNotToUse'
function Util.GetRandomDir( dirsNotToUse )
  local dirs = { Util.UP, Util.DOWN, Util.LEFT, Util.RIGHT }

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


-- Sets an object's animation and then plays it.
function Util.SetAndPlay( object, animIndex )
  object:SetAnim( animIndex )
  object:PlayAnim()
end


-- Generic enemy AI for an Object to use. Moves in same direction until
-- either it hits the same axis as Kickle (in which case it changes direction
-- to go towards Kickle unless it can't move in that direction)
-- or can't move any further.
function Util.GenericEnemyAI( enemy )
  local kickle = State.GetObject( "Kickle" )
  if kickle then
    local enemyX, enemyY = enemy:GetTile()
    local kickleX, kickleY = kickle:GetTile()
    local newDir = enemy:GetDir()
    if enemyX == kickleX then
      if enemyY < kickleY then
        newDir = Util.DOWN
      elseif enemyY > kickleY then
        newDir = Util.UP
      end
    elseif enemyY == kickleY then
      if enemyX < kickleX then
        newDir = Util.RIGHT
      elseif enemyX > kickleX then
        newDir = Util.LEFT
      end
    end
    local facingX, facingY = Util.GetTileInDir( newDir, enemyX, enemyY )
    if State.TileIsCrossable( facingX, facingY ) and
       not State.ObjectBlockingTile( facingX, facingY ) then
      enemy:SetDir( newDir )
    end
  end

  local facingX, facingY = Util.GetTileObjectFaces( enemy )
  if not State.TileIsCrossable( facingX, facingY ) or  
     State.ObjectBlockingTile( facingX, facingY ) then
    enemy:SetDir( Util.GetNextDir( enemy:GetDir() ))
  end

  
  Util.SetAndPlay( enemy, enemy:GetDir())
  enemy:Move()
end

return Util

