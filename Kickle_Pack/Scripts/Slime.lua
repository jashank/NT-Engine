package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--Slime Behavior Table

local Slime = {}

Slime.spawnPointX = -1
Slime.spawnPointY = -1
Slime.hasMoved = false

function Slime.AILogic( self )
  if ( not Slime.hasMoved ) then
    Slime.spawnPointX, Slime.spawnPointY = self:GetTile()
    Slime.hasMoved = true
  end

  GenericEnemyAI( self )
end


function Slime.HandleCollision( self, other )
	if ( other:GetType() == "Slime" ) then
		self:Reverse()
		self:SetAnimation( self:GetDir() )
	end
end

return Slime
