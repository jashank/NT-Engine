-- Slime's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

function AILogic( Slime )

	Kickle = Game.GetGameObject( "Kickle" );


	if ( Kickle ) then
		KickleX = Kickle:GetTileX();
		KickleY = Kickle:GetTileY();


		-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT
		SlimeX = Slime:GetTileX();
		SlimeY = Slime:GetTileY();
		SlimeUpX, SlimeUpY = GetTileObjectFaces( Slime, 0 );
		SlimeDownX, SlimeDownY = GetTileObjectFaces( Slime, 1 );
		SlimeLeftX, SlimeLeftY = GetTileObjectFaces( Slime, 2 );
		SlimeRightX, SlimeRightY = GetTileObjectFaces( Slime, 3 );

		if ( SlimeX < KickleX and
		  not Game.IsTileSolid( SlimeRightX, SlimeRightY ) and
		  not Game.TileHasSolidObject( SlimeRightX, SlimeRightY ) ) then
			Slime:MoveDir( 3 );
			Slime:SetAnimation( 3 );

		elseif ( SlimeX > KickleX and
		  not Game.IsTileSolid( SlimeLeftX, SlimeLeftY ) and
		  not Game.TileHasSolidObject( SlimeLeftX, SlimeLeftY ) ) then
			Slime:MoveDir( 2 );
			Slime:SetAnimation( 2 );

		elseif ( SlimeY < KickleY and
		  not Game.IsTileSolid( SlimeDownX, SlimeDownY ) and
		  not Game.TileHasSolidObject( SlimeDownX, SlimeDownY ) ) then
			Slime:MoveDir( 1 );
			Slime:SetAnimation( 1 );

		elseif ( SlimeY > KickleY and
		  not Game.IsTileSolid( SlimeUpX, SlimeUpY ) and
		  not Game.TileHasSolidObject( SlimeUpX, SlimeUpY ) ) then
		    Slime:MoveDir( 0 );
			Slime:SetAnimation( 0 );

		end
	end

end
