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
  end
end

return Slime
