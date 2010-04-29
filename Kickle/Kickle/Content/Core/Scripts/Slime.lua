package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--Slime Behavior Table

local SlimeTable = {}

SlimeTable.lastTileX = -1
SlimeTable.lastTileY = -1
SlimeTable.spawnPointX = -1
SlimeTable.spawnPointY = -1
SlimeTable.hasMoved = false

function SlimeTable.AILogic( Slime )

  if ( not SlimeTable.hasMoved ) then
    SlimeTable.spawnPointX, SlimeTable.spawnPointY = Slime:GetTilePos()
    SlimeTable.hasMoved = true
  end

	local Kickle = Level.GetGameObject( "Kickle" )

	if ( Kickle ) then
    local SlimeX, SlimeY = Slime:GetTilePos()
    local KickleX, KickleY = Kickle:GetTilePos()

		local distanceX = math.abs( SlimeX - KickleX )
		local distanceY = math.abs( SlimeY - KickleY )

		local dir = UP;

		if ( distanceX > distanceY ) then
			if ( SlimeX < KickleX ) then
				dir = RIGHT
			elseif ( SlimeX > KickleX ) then
				dir = LEFT
			else
				dir = math.random( LEFT, RIGHT )
			end
		else
			if ( SlimeY < KickleY ) then
				dir = DOWN
			elseif ( SlimeY > KickleY ) then
				dir = UP
			else
				dir = math.random( UP, DOWN )
			end
		end

		-- Slime has been on same tile for 2 updates
		if ( SlimeTable.lastTileX == SlimeX and
				 SlimeTable.lastTileY == SlimeY ) then
			dir = math.random( UP, RIGHT )
		end

		SlimeTable.lastTileX = SlimeX
		SlimeTable.lastTileY = SlimeY

		Slime:SetDir( dir )
		Slime:Move()
		Slime:SetAnimation( dir )
	end
end


function SlimeTable.HandleCollision( Slime, Other )
	if ( Other:GetType() == "Slime" ) then
		Slime:Reverse()
		Slime:SetAnimation( Slime:GetDir() )
	end
end

return SlimeTable
