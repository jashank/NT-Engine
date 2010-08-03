package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")
math.randomseed( os.time() )

--Slime Behavior Table

local Slime = {}

Slime.spawnPointX = -1
Slime.spawnPointY = -1
Slime.hitSlime = nil

function Slime.Init( self )
  local spawnPoint = State.GetNearestObject( "SpawnPoint", self:GetTile())
  if spawnPoint then
    Slime.spawnPointX, Slime.spawnPointY = spawnPoint:GetTile()
  end
end


function Slime.AI( self )
  Util.GenericEnemyAI( self )
end


function Slime.HandleCollision( self, other )
  local otherType = other:GetType()
	if otherType == "Slime" or otherType == "Penguin" then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir())) 	
    self:PlayAnimation( dir )
  end
end

return Slime
