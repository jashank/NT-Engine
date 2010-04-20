package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--Slime Behavior Table

local SlimeTable = {}

SlimeTable.lastTileX = -1
SlimeTable.lastTileY = -1

function SlimeTable.AILogic( Slime )

	local Kickle = Level.GetGameObject( "Kickle" )

	if ( Kickle ) then
		local SlimeX = Slime:GetTileX()
		local SlimeY = Slime:GetTileY()
		local KickleX = Kickle:GetTileX()
		local KickleY = Kickle:GetTileY()

		local distanceX = math.abs( SlimeX - KickleX )
		local distanceY = math.abs( SlimeY - KickleY )

		local dir = SlimeAnimation.UP_WALKING;

		if ( distanceX > distanceY ) then
			if ( SlimeX < KickleX ) then
				dir = SlimeAnimation.RIGHT_WALKING
			elseif ( SlimeX > KickleX ) then
				dir = SlimeAnimation.LEFT_WALKING
			else
				dir = math.random( LEFT, RIGHT )
			end
		else
			if ( SlimeY < KickleY ) then
				dir = SlimeAnimation.DOWN_WALKING
			elseif ( SlimeY > KickleY ) then
				dir = SlimeAnimation.UP_WALKING
			else
				dir = math.random( SlimeAnimation.UP_WALKING, SlimeAnimation.DOWN_WALKING )
			end
		end

		-- Slime has been on same tile for 2 updates
		if ( SlimeTable.lastTileX == SlimeX and
				 SlimeTable.lastTileY == SlimeY ) then
			dir = math.random( SlimeAnimation.UP_WALKING, SlimeAnimation.DOWN_WALKING )
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
