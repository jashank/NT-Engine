package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Slime = require("Enemy"):New{ isFreezable = true }
Slime.spawn = nil

function Slime:Init( slime )
  self.spawn = Map.GetNearestObject( "SpawnPoint", slime:GetTile())
end


local EnemyCollision = Slime.HandleCollision
function Slime:HandleCollision( slime, other )
  EnemyCollision( self, slime, other )
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
  
  -- Ensure that slimes isn't currently on a spawn point, can be taken out
  -- when slimes don't spawn directly on spawn points anymore
  if not Map.ObjectBlockingTile( cx, cy ) then
    block = Map.CreateObject("Kickle_Pack/Objects/IceBlock.xml", cx, cy)
    block:GetTable():SetSlimeSpawn( self.spawn )
  else
    self.spawn:GetTable():Spawn( self.spawn )
  end

  Map.DestroyObject( slime )
end


-- Slime never freezes, just gets destroyed and turned into an IceBlock.
function Slime:IsFrozen()
  return false
end


function Slime:GetSpawn()
  return self.spawn
end

return Slime

