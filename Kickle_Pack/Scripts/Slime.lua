package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")
EnemyLib = require("EnemyLib")

--Slime Behavior Table

local Slime = {}

Slime.spawn = nil

function Slime.Init( self )
  Slime.spawn = State.GetNearestObject( "SpawnPoint", self:GetTile())
end

function Slime.AI( self )
  EnemyLib.GenericAI( self )
end


function Slime.HandleCollision( self, other )
  if EnemyLib.GenericCollision( self, other ) then
    return
  end

  local otherType = other:GetType()
  if otherType == "SlipperyIce" then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir()))
    Util.SetAndPlay( self, dir )

  elseif otherType == "IceBreath" then
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
    State.DestroyObject( other )
  end
end

return Slime
