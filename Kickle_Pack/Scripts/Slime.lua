package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--Slime Behavior Table

local Slime = {}

Slime.lastTileX = -1
Slime.lastTileY = -1
Slime.spawnPointX = -1
Slime.spawnPointY = -1
Slime.hasMoved = false

function Slime.AILogic( self )

  if ( not Slime.hasMoved ) then
    Slime.spawnPointX, Slime.spawnPointY = self:GetTile()
    Slime.hasMoved = true
  end

	local Kickle = Game.GetGameObject( "Kickle" )

	if ( Kickle ) then
    local selfX, selfY = self:GetTile()
    local KickleX, KickleY = Kickle:GetTile()

		local distanceX = math.abs( selfX - KickleX )
		local distanceY = math.abs( selfY - KickleY )

		local dir = UP;

		if ( distanceX > distanceY ) then
			if ( selfX < KickleX ) then
				dir = RIGHT
			elseif ( selfX > KickleX ) then
				dir = LEFT
			else
				dir = math.random( LEFT, RIGHT )
			end
		else
			if ( selfY < KickleY ) then
				dir = DOWN
			elseif ( selfY > KickleY ) then
				dir = UP
			else
				dir = math.random( UP, DOWN )
			end
		end

		-- self has been on same tile for 2 updates
		if ( Slime.lastTileX == selfX and
				 Slime.lastTileY == selfY ) then
			dir = math.random( UP, RIGHT )
		end

		Slime.lastTileX = selfX
		Slime.lastTileY = selfY

		self:SetDir( dir )
		self:Move()
		self:SetAnimation( dir )
	end
end


function Slime.HandleCollision( self, other )
	if ( other:GetType() == "Slime" ) then
		self:Reverse()
		self:SetAnimation( self:GetDir() )
	end
end

return Slime
