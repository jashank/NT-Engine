-- Slime's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");
math.randomseed( os.time() );

local dir = UP;
local slimeCollision = false;
local lastTileX = 0;
local lastTileY = 0;

function AILogic( Slime )

	Kickle = Game.GetGameObject( "Kickle" );

	if ( Kickle ) then

		SlimeX = Slime:GetTileX();
		SlimeY = Slime:GetTileY();

		if ( not slimeCollision ) then
			KickleX = Kickle:GetTileX();
			KickleY = Kickle:GetTileY();

			moveAxis = math.random( 0, 1 );

			if ( moveAxis == 0 ) then
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
		end

		if ( lastTileX == SlimeX and lastTileY == SlimeY ) then
			dir = GetOppositeDirection( dir );
		end

		lastTileX = SlimeX;
		lastTileY = SlimeY;
		slimeCollision = false;

		Slime:MoveDir( dir );
		Slime:SetAnimation( dir );
	end

end


function HandleCollision( Slime, Other )
	if ( Other:GetType() == "Slime" ) then
		dir = GetOppositeDirection( dir );
		slimeCollision = true;
	end
end
