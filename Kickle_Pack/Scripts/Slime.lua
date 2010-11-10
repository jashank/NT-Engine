package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Slime = require("Enemy"):New{ isFreezable = true }
Slime.spawn = nil

function Slime.Init( self )
  Slime.spawn = State.GetNearestObject( "SpawnPoint", self:GetTile())
end


local EnemyCollision = Slime.HandleCollision
function Slime.HandleCollision( self, other )
  EnemyCollision( self, other )
  if other:GetType() == "SlipperyIce" then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir()))
    Util.SetAndPlay( self, dir )
  end
end


function Slime.Freeze( self )
  local tx, ty, bx, by = self:GetTileRange()
  dir = self:GetDir()

  local cx, cy = tx, ty
  -- protect against block spawning on Kickle
  if dir == Util.UP then
    cx, cy = tx, by
  elseif dir == Util.LEFT then
    cx, cy = bx, ty
  end

  block = State.CreateObject("Kickle_Pack/Objects/IceBlock.xml", cx, cy)
  State.DestroyObject( self )
end


-- Slime never freezes, just gets destroyed and turned into an IceBlock.
function Slime.IsFrozen()
  return false
end

return Slime

