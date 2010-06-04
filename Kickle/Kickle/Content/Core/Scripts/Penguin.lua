--Penguin Behavior Table

local PenguinTable = {}

PenguinTable.lastTileX = -1;
PenguinTable.lastTileY = -1;

function PenguinTable.AILogic( Penguin )

	local Kickle = Game.GetGameObject( "Kickle" )

	if ( Kickle ) then
		local PenguinX, PenguinY = Penguin:GetTile()
    local KickleX, KickleY = Kickle:GetTile()

		local distanceX = math.abs( PenguinX - KickleX )
		local distanceY = math.abs( PenguinY - KickleY )

		local dir = UP;

		if ( distanceX > distanceY ) then
			if ( PenguinX < KickleX ) then
				dir = RIGHT
			elseif ( PenguinX > KickleX ) then
				dir = LEFT
			else
				dir = math.random( LEFT, RIGHT )
			end
		else
			if ( PenguinY < KickleY ) then
				dir = DOWN
			elseif ( PenguinY > KickleY ) then
				dir = UP
			else
				dir = math.random( UP, DOWN )
			end
		end

		-- Penguin has been on same tile for 2 updates
		if ( PenguinTable.lastTileX == PenguinX and
				 PenguinTable.lastTileY == PenguinY ) then
			dir = math.random( UP, RIGHT )
		end

		PenguinTable.lastTileX = PenguinX
		PenguinTable.lastTileY = PenguinY

		Penguin:SetDir( dir )
		Penguin:Move()
		Penguin:SetAnimation( dir )
	end
end

return PenguinTable
