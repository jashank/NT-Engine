package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

--Slime Behavior Table

local Slime = {}

Slime.spawn = nil

function Slime.Init( self )
  Slime.spawn = State.GetNearestObject( "SpawnPoint", self:GetTile())
end

function Slime.AI( self )
  Util.GenericEnemyAI( self )
end


function Slime.HandleCollision( self, other )
  local otherType = other:GetType()
  if otherType == "Slime" or otherType == "Penguin" or
     otherType == "SlipperyIce" then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir()))       
    Util.SetAndPlay( self, dir )
  end
end

return Slime
