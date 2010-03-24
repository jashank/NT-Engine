-- Kickle's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT
local dir = 0; -- Direction
local moveable = true; -- If true then kickle is moveable

-- 0 STANDING, 4 WALKING, 8 KICKING, 12 PILLAR RAISE, 16 DYING
-- These values are determined by the layout of Kickle's animation sheet
local mode = 0;

-- Whether Kickle is taking an action ( ice breath, raise pillar, etc.)
local inAction = false;


-- [[ Function that is called to handle scripted collision response events ]]
function HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		-- Show death animation
		Kickle:SetAnimation( dir + 16 );
		-- Disable kickle's movement
	     moveable = false;
	end

end


-- [[ Function that is called to handle scripted user input events ]]
function HandleUserInput( Kickle )
	if( moveable ) then

		if ( Game.IsKeyDown( 293 ) and not inAction ) then
			dir = 0;
			mode = 4;
		elseif ( Game.IsKeyDown( 294 ) and not inAction ) then
			dir = 1;
			mode = 4;
		elseif ( Game.IsKeyDown( 291 ) and not inAction ) then
			dir = 2;
			mode = 4;
		elseif ( Game.IsKeyDown( 292 ) and not inAction ) then
			dir = 3;
			mode = 4;
		elseif ( Game.IsKeyDown( 120 ) ) then
			tileX, tileY = GetTileObjectFaces( Kickle, dir );

			if ( not Game.IsTileSolid( tileX, tileY ) and
			     not Game.TileHasSolidObject( tileX, tileY )) then
				mode = 12;
				inAction = true;
				Game.CreateGameObject(
					"Content/Core/Objects/Pillar.xml", tileX, tileY );
			end

		elseif ( not inAction ) then
			mode = 0; -- set mode to STANDING
		end

		if ( mode == 4 ) then
			Kickle:MoveDir( dir );
		end

		if ( not Kickle:IsAnimating() ) then
			inAction = false;
		end

		Kickle:SetAnimation( dir + mode );

	end
end
