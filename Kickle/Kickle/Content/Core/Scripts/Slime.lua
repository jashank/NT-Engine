-- Slime's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT

local dir = 0;

function AILogic( Slime )

	Kickle = Game.GetGameObject( "Kickle" );

	if ( Kickle ) then
		KickleX = Kickle:GetTileX();
		KickleY = Kickle:GetTileY();

		SlimeX = Slime:GetTileX();
		SlimeY = Slime:GetTileY();

		if ( SlimeX < KickleX ) then
			dir = 3;
		elseif ( SlimeX > KickleX ) then
			dir = 2;
		elseif ( SlimeY < KickleY ) then
			dir = 1;
		elseif ( SlimeY > KickleY ) then
			dir = 0;
		end

		for i = 0,3 do
			tileFacingX, tileFacingY = GetTileObjectFaces( Slime, dir );

			if ( Game.IsTileSolid( tileFacingX, tileFacingY ) or
				 Game.TileHasSolidObject( tileFacingX, tileFacingY ) ) then
				if ( dir == 0 ) then
					dir = 2;
				elseif ( dir == 1 ) then
					dir = 3;
				elseif ( dir == 2 ) then
					dir = 0;
				elseif ( dir == 3 ) then
					dir = 1;
				end
			else
				break;
			end
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
