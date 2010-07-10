--Penguin Behavior Table

local Penguin = {}

Penguin.lastTileX = -1;
Penguin.lastTileY = -1;

function Penguin.AILogic( self )

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

		-- Penguin has been on same tile for 2 updates
		if ( Penguin.lastTileX == selfX and
				 Penguin.lastTileY == selfY ) then
			dir = math.random( UP, RIGHT )
		end

		Penguin.lastTileX = selfX
		Penguin.lastTileY = selfY

		self:SetDir( dir )
		self:Move()
		self:SetAnimation( dir )
	end
end

return Penguin
