-- Kickle's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

-- These values are determined by the layout of Kickle's animation sheet
local STANDING = 0;
local WALKING = 4;
local KICKING = 8;
local RAISE_PILLAR = 12;
local DYING = 16;

local dir = UP;
local moveable = true;
local mode = STANDING;

-- Whether Kickle is taking an action ( ice breath, raise pillar, etc.)
local inAction = false;


-- [[ Function that is called to handle scripted collision response events ]]
function HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		Kickle:SetAnimation( dir + DYING );
	     moveable = false;
	end

end


-- [[ Function that is called to handle scripted user input events ]]
function HandleUserInput( Kickle )
	if( moveable ) then

		if ( Game.IsKeyDown( 293 ) and not inAction ) then
			dir = UP;
			mode = WALKING;
		elseif ( Game.IsKeyDown( 294 ) and not inAction ) then
			dir = DOWN;
			mode = WALKING;
		elseif ( Game.IsKeyDown( 291 ) and not inAction ) then
			dir = LEFT;
			mode = WALKING;
		elseif ( Game.IsKeyDown( 292 ) and not inAction ) then
			dir = RIGHT;
			mode = WALKING;
		elseif ( Game.IsKeyDown( 120 ) ) then
			tileX, tileY = GetTileObjectFaces( Kickle, dir );

			if ( not Game.IsTileSolid( tileX, tileY ) and
			     not Game.TileHasSolidObject( tileX, tileY )) then
				mode = RAISE_PILLAR;
				inAction = true;
				Game.CreateGameObject(
					"Content/Core/Objects/Pillar.xml", tileX, tileY );
			end

		elseif ( not inAction ) then
			mode = STANDING;
		end

		if ( mode == WALKING ) then
			Kickle:MoveDir( dir );
		end

		if ( not Kickle:IsAnimating() ) then
			inAction = false;
		end

		Kickle:SetAnimation( dir + mode );

	end
end
