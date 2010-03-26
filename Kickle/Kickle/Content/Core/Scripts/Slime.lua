-- Slime's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");
math.randomseed( os.time() );

local dir = UP;
local tryingNewDirection = false;

function AILogic( Slime )

	Kickle = Game.GetGameObject( "Kickle" );

	if ( Kickle ) then
		KickleX = Kickle:GetTileX();
		KickleY = Kickle:GetTileY();

		SlimeX = Slime:GetTileX();
		SlimeY = Slime:GetTileY();

		if ( SlimeX < KickleX and not tryingNewDirection ) then
			dir = RIGHT;
		elseif ( SlimeX > KickleX and not tryingNewDirection ) then
			dir = LEFT;
		elseif ( SlimeY < KickleY and not tryingNewDirection ) then
			dir = DOWN;
		elseif ( SlimeY > KickleY and not tryingNewDirection) then
			dir = UP;
		end

		tileFacingX, tileFacingY = GetTileObjectFaces( Slime, dir );

		if ( Game.IsTileSolid( tileFacingX, tileFacingY ) or
			 Game.TileHasSolidObject( tileFacingX, tileFacingY ) ) then
			dir = math.random( UP, RIGHT );
			if ( dir > RIGHT ) then
				dir = UP;
			end
			tryingNewDirection = true;
		else
			tryingNewDirection = false;
		end

		Slime:MoveDir( dir );
		Slime:SetAnimation( dir );
	end

end


function HandleCollision( Slime, Other )
	if ( Other:GetType() == "Slime" ) then
		dir = GetOppositeDirection( dir );
	end
end
