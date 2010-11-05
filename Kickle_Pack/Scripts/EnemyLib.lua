package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")


-- This library of functions provides a basic starting point for
-- implementing behavior for enemies in Kickle Cubicle. The functions
-- make various assumptions about the objects being passed in, so read
-- the invariants.
local EnemyLib = {}

-- Generic enemy AI for an Object to use. Moves in same direction until
-- either it hits the same axis as Kickle (in which case it changes direction
-- to go towards Kickle unless it can't move in that direction)
-- or can't move any further.
function EnemyLib.GenericAI( enemy )
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


-- Does same as generic but assumes that enemy has a 'frozen' element.
-- If the enemy isn't frozen then the generic AI is run. Otherwise the
-- time the enemy has been frozen is checked (assuming that timer was set
-- when frozen) and the enemy is unfrozen if enough time has passed.
function EnemyLib.FrozenAI( enemy, maxFrozenTime )
  if not enemy:GetTable().frozen then
    EnemyLib.GenericAI( enemy )
  else 
    local timeFrozen = enemy:GetElapsedTime()
    if ( timeFrozen >= maxFrozenTime ) then
      enemy:GetTable().frozen = false
      enemy:BlockTileRange( false )
    end
  end
end


-- Generic collision handling for all enemies. Tests if any of the standard
-- enemy types are the source of collision and if so turns 'enemy' around.
-- Returns true if collision occured with one of the types checked.
function EnemyLib.GenericCollision( enemy, other )
  local otherType = other:GetType()
    
  if otherType == "Slime" or otherType == "Penguin" or 
     otherType == "Chicken" then
    local dir = enemy:SetDir( Util.GetOppositeDir( enemy:GetDir()))
    Util.SetAndPlay( enemy, dir )
    return true
  end

  return false
end

-- Tests for collision against an ice breath with the enemy passed. If it
-- occured then the enemy's 'frozen' element is set to true, its timer is
-- reset, its tile range is blocked, and the ice breath is destroyed. Returns
-- true if this collision occured.
function EnemyLib.FrozenCollision( enemy, other )
  local otherType = other:GetType()

  if otherType == "IceBreath" then
    enemy:GetTable().frozen = true
    enemy:BlockTileRange( true )
    enemy:ResetTimer()
    State.DestroyObject( other )
    return true
  end
  return false
end

return EnemyLib

