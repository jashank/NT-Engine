-- Slime's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

math.randomseed( os.time() );

local lastTileX = -1;
local lastTileY = -1;

function AILogic( Slime )

	Kickle = Game.GetGameObject( "Kickle" );

	if ( Kickle ) then

		SlimeX = Slime:GetTileX();
		SlimeY = Slime:GetTileY();

		KickleX = Kickle:GetTileX();
		KickleY = Kickle:GetTileY();
		distanceX = math.abs( SlimeX - KickleX );
		distanceY = math.abs( SlimeY - KickleY );

		dir = UP;

		if ( distanceX > distanceY ) then
			if ( SlimeX < KickleX ) then
				dir = RIGHT;
			elseif ( SlimeX > KickleX ) then
				dir = LEFT;
			else
				dir = math.random( LEFT, RIGHT );
			end
		else
			if ( SlimeY < KickleY ) then
				dir = DOWN;
			elseif ( SlimeY > KickleY ) then
				dir = UP;
			else
				dir = math.random( UP, DOWN );
			end
		end

		-- Slime has been on same tile for 2 updates
		if ( lastTileX == SlimeX and lastTileY == SlimeY ) then
			dir = math.random( UP, RIGHT );
		end

		lastTileX = SlimeX;
		lastTileY = SlimeY;

		Slime:SetDir( dir );
		Slime:Move();
		Slime:SetAnimation( dir );
	end
end


function HandleCollision( Slime, Other )
	if ( Other:GetType() == "Slime" ) then
		Slime:Reverse();
		Slime:SetAnimation( Slime:GetDir() );
	end
end
