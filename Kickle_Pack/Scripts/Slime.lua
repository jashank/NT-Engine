package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("GameObjectUtilities")
math.randomseed( os.time() )

--Slime Behavior Table

local Slime = {}

Slime.spawnPointX = -1
Slime.spawnPointY = -1

function Slime.AI( self )
  if Slime.spawnPointX == -1 and Slime.spawnPointY == -1 then
    spawnPoint = Game.GetNearestGameObject( "SpawnPoint", self:GetTile() )
    if spawnPoint then
      Slime.spawnPointX, Slime.spawnPointY = spawnPoint:GetTile()
    end
  end

  Util.GenericEnemyAI( self )
end


function Slime.HandleCollision( self, other )
  otherType = other:GetType()
	if otherType == "Slime" or otherType == "Penguin" then
		self:Reverse()
    self:SetAnimation( self:GetDir())
	end
end

return Slime
