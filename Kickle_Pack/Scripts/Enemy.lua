package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Enemy = require("Entity"):New{ isEnemy = true }

-- Moves in same direction until either it hits the same axis as Kickle 
-- (in which case it changes direction to go towards Kickle unless it can't 
-- move in that direction) or can't move any further.
function Enemy:AI( enemy )
  local kickle = Map.GetObject( "Kickle" )
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
    if Map.TileIsCrossable( facingX, facingY ) and
       not Map.ObjectBlockingTile( facingX, facingY ) then
      enemy:SetDir( newDir )
    end
  end

  local facingX, facingY = Util.GetTileObjectFaces( enemy )
  if not Map.TileIsCrossable( facingX, facingY ) or  
     Map.ObjectBlockingTile( facingX, facingY ) then
    enemy:SetDir( Util.GetNextDir( enemy:GetDir() ))
  end

  
  Util.SetAndPlay( enemy, enemy:GetDir())
  enemy:Move()
end


-- Bounce off of other enemies and kill Kickle if encountered.
function Enemy:HandleCollision( enemy, other )
  if other:GetTable():IsEnemy() then
    local dir = enemy:SetDir( Util.GetOppositeDir( enemy:GetDir() ))
    Util.SetAndPlay( enemy, dir )
  else
    otherType = other:GetType()
    if otherType == "Kickle" then
      other:GetTable():Kill( other )
    elseif otherType == "DreamBag" then
      other:GetTable():Push( other )
    elseif otherType == "Pillar" then
      Map.DestroyObject( other )
    end
  end
end

return Enemy

