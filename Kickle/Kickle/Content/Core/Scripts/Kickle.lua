-- Kickle's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

-- These values are determined by the layout of Kickle's animation sheet
local STANDING = 0;
local WALKING = 4;
local KICKING = 8;
local RAISE_PILLAR = 12;
local DYING = 16;

local dir = DOWN; --Current direction kickle is moving
local state = STANDING; --Current state kickle is in

local pillar = nil;

--[[ Function that is called to handle scripted collision response events ]]
function HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		state = DYING;
		Kickle:SetAnimation( dir + state );
	end

end



function AILogic( Kickle )
	if( state ~= DYING ) then
		if( state ~= RAISE_PILLAR ) then
			state = STANDING;
		end

		if ( state == RAISE_PILLAR and not Kickle:IsAnimating() ) then
			state = STANDING;
			if( pillar ~= nil ) then
				Kickle:AnimateForward();
				Game.DestroyGameObject( pillar );
				pillar = nil;
			end
		end

		Kickle:SetAnimation( dir + state );
	end
end


function FaceUp( Kickle )
	if( state == STANDING ) then
		dir = UP;
	end
end


function FaceDown( Kickle )
	if( state == STANDING ) then
		dir = DOWN;
	end
end


function FaceLeft( Kickle )
	if( state == STANDING ) then
		dir = LEFT;
	end
end


function FaceRight( Kickle )
	if( state == STANDING ) then
		dir = RIGHT;
	end
end


function WalkUp( Kickle )
	if( state == STANDING ) then
		dir = UP;
		state = WALKING;
		Kickle:MoveDir( dir );
		Kickle:SetAnimation( dir + state );
	end
end


function WalkDown( Kickle )
	if( state == STANDING ) then
		dir = DOWN;
		state = WALKING;
		Kickle:MoveDir( dir );
		Kickle:SetAnimation( dir + state );
	end
end


function WalkLeft( Kickle )
	if( state == STANDING ) then
		dir = LEFT;
		state = WALKING;
		Kickle:MoveDir( dir );
		Kickle:SetAnimation( dir + state );
	end
end


function WalkRight( Kickle )
	if( state == STANDING ) then
		dir = RIGHT;
		state = WALKING;
		Kickle:MoveDir( dir );
		Kickle:SetAnimation( dir + state );
	end
end


function PerformPillar( Kickle )
	if( state == STANDING ) then
		tileX, tileY = GetTileObjectFaces( Kickle, dir );

		if ( not Game.TileIsSolid( tileX, tileY ) and
			 not Game.TileHasGridObject( tileX, tileY ) and
			 not Game.GetGameObjectOnTile( tileX, tileY ) ) then

			state = RAISE_PILLAR;
			Game.CreateGameObject(
				"Content/Core/Objects/Pillar.xml", tileX, tileY );

		elseif ( Game.TileHasGridObject( tileX, tileY ) ) then

			GameObjectOnTile = Game.GetGameObjectOnTile( tileX, tileY );

			if ( GameObjectOnTile:GetType() == "Pillar" ) then
				state = RAISE_PILLAR;
				Kickle:AnimateBackward();
				GameObjectOnTile:AnimateBackward();
				pillar = GameObjectOnTile;
			end
		end
	end
end
