package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Enemy = require("Entity"):New{ isEnemy = true }

-- Moves in same direction until either it hits the same axis as Kickle 
-- (in which case it changes direction to go towards Kickle unless it can't 
-- move in that direction) or can't move any further.
function Enemy.AI( self )
  local kickle = State.GetObject( "Kickle" )
  if kickle then
    local enemyX, enemyY = self:GetTile()
    local kickleX, kickleY = kickle:GetTile()
    local newDir = self:GetDir()
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
      self:SetDir( newDir )
    end
  end

  local facingX, facingY = Util.GetTileObjectFaces( self )
  if not State.TileIsCrossable( facingX, facingY ) or  
     State.ObjectBlockingTile( facingX, facingY ) then
    self:SetDir( Util.GetNextDir( self:GetDir() ))
  end

  
  Util.SetAndPlay( self, self:GetDir())
  self:Move()
end


-- Bounce off of other enemies and kill Kickle if encountered.
function Enemy.HandleCollision( self, other )
  if other:GetTable().IsEnemy() then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir() ))
    Util.SetAndPlay( enemy, dir )
  else
    otherType = other:GetType()
    if otherType == "Kickle" then
      other:GetTable().Kill( other )
    elseif otherType == "DreamBag" then
      other:GetTable().Push( other )
    elseif otherType == "Pillar" then
      State.DestroyObject( other )
    end
  end
end

return Enemy

