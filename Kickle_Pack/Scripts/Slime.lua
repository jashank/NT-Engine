package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Slime = require("Enemy"):New{ isFreezable = true }
Slime.spawn = nil

function Slime:Init( slime )
  self.spawn = State.GetNearestObject( "SpawnPoint", slime:GetTile())
end


function Slime:HandleCollision( slime, other )
  self.__index:HandleCollision( slime, other )
  if other:GetType() == "SlipperyIce" then
    local dir = slime:SetDir( Util.GetOppositeDir( slime:GetDir()))
    Util.SetAndPlay( slime, dir )
  end
end


function Slime:Freeze( slime )
  local tx, ty, bx, by = slime:GetTileRange()
  dir = slime:GetDir()

  local cx, cy = tx, ty
  -- protect against block spawning on Kickle
  if dir == Util.UP then
    cx, cy = tx, by
  elseif dir == Util.LEFT then
    cx, cy = bx, ty
  end

  block = State.CreateObject("Kickle_Pack/Objects/IceBlock.xml", cx, cy)
  State.DestroyObject( slime )
end


-- Slime never freezes, just gets destroyed and turned into an IceBlock.
function Slime:IsFrozen()
  return false
end


function Slime:GetSpawn()
  return slime.spawn
end

return Slime

