-- Kickle's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

-- These values are determined by the layout of Kickle's animation xml file
local STANDING = 0;
local WALKING = 4;
local KICKING = 8;
local RAISE_PILLAR = 12;
local LOWER_PILLAR = 16;
local DYING = 20;

local state = STANDING; --Current state kickle is in

local pillar = nil;

--[[ Function that is called to handle scripted collision response events ]]
function HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		state = DYING;
		Kickle:SetAnimation( Kickle:GetDir() + state );
	end

end



function AILogic( Kickle )
	if( state ~= DYING ) then
		Kickle:SetAnimation( Kickle:GetDir() + state );

		if( state ~= RAISE_PILLAR and state ~= LOWER_PILLAR ) then
			state = STANDING;
		end

		if ( (state == RAISE_PILLAR
			 or state == LOWER_PILLAR)
			 and not Kickle:IsAnimating() ) then
			 state = STANDING;
		end
	end

	if( pillar and not pillar:IsAnimating() ) then
		Level.DestroyGameObject( pillar );
		pillar = nil;
	end
end


function FaceUp( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( UP );
	end
end


function FaceDown( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( DOWN );
	end
end


function FaceLeft( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( LEFT );
	end
end


function FaceRight( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( RIGHT );
	end
end


function WalkUp( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( UP );
		state = WALKING;
		Kickle:Move();
		Kickle:SetAnimation( UP + state );
	end
end


function WalkDown( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( DOWN );
		state = WALKING;
		Kickle:Move();
		Kickle:SetAnimation( DOWN + state );
	end
end


function WalkLeft( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( LEFT );
		state = WALKING;
		Kickle:Move();
		Kickle:SetAnimation( LEFT + state );
	end
end


function WalkRight( Kickle )
	if( state == STANDING ) then
		Kickle:SetDir( RIGHT );
		state = WALKING;
		Kickle:Move();
		Kickle:SetAnimation( RIGHT + state );
	end
end


function PerformPillar( Kickle )
	if( state == STANDING ) then
		tileX, tileY = GetTileObjectFaces( Kickle );

		if ( not Level.TileIsSolid( tileX, tileY ) and
			 not Level.TileHasGridObject( tileX, tileY ) ) then

			state = RAISE_PILLAR;
			Level.CreateGameObject(
				"Content/Core/Objects/Pillar.xml", tileX, tileY );

		elseif ( Level.TileHasGridObject( tileX, tileY ) ) then

			GameObjectOnTile = Level.GetGameObjectOnTile( tileX, tileY );

			if( GameObjectOnTile:GetType() == "Pillar" ) then
				state = LOWER_PILLAR;
				pillar = GameObjectOnTile;
				pillar:SetAnimation(1);
			end
		end
	end
end

-- Spawns an icebreath object if there is not an iceblock infront of it. Else
-- it will set the direction of the iceblock.
function PerformAttack( Kickle )
	if ( state == STANDING ) then
		tileX, tileY = GetTileObjectFaces( Kickle );
		FacingObject = Level.GetGameObjectOnTile( tileX, tileY );

		if ( FacingObject and FacingObject:GetType() == "IceBlock" ) then
			state = KICKING;

			kickleDir = Kickle:GetDir();
			Kickle:SetAnimation( kickleDir + state );
			FacingObject:SetDir( kickleDir );
			FacingObject:Move();


		elseif ( not Level.TileIsSolid( tileX, tileY ) and
				 not Level.TileHasGridObject( tileX, tileY ) )then
			IceBreath = Level.CreateGameObject(
				"Content/Core/Objects/IceBreath.xml", tileX, tileY );
			iceBreathdir = Kickle:GetDir();
			IceBreath:SetDir( iceBreathdir );
			IceBreath:SetAnimation( iceBreathdir );
		end
	end
end
