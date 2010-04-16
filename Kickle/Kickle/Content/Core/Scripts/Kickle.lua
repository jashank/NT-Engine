package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

-- These values are determined by the layout of Kickle's animation xml file
local STANDING = 0
local WALKING = 4
local KICKING = 8
local RAISE_PILLAR = 12
local LOWER_PILLAR = 16
local DYING = 20

--Kickle's Behavior Table

local KickleTable = {}

KickleTable.state = STANDING --Current state kickle is in
KickleTable.pillar = nil

function KickleTable.HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		KickleTable.state = DYING
		Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
	end
end


function KickleTable.AILogic( Kickle )
	if( KickleTable.state ~= DYING ) then
		Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )

		if( KickleTable.state ~= RAISE_PILLAR and
			  KickleTable.state ~= LOWER_PILLAR ) then
			KickleTable.state = STANDING
		end

		if (( KickleTable.state == RAISE_PILLAR or
			    KickleTable.state == LOWER_PILLAR ) and not
			    Kickle:IsAnimating() ) then
			 KickleTable.state = STANDING
		end
	end

	if( KickleTable.pillar and not KickleTable.pillar:IsAnimating() ) then
		Level.DestroyGameObject( KickleTable.pillar )
		KickleTable.pillar = nil
	end
end


function KickleTable.FaceUp( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( UP )
	end
end


function KickleTable.FaceDown( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( DOWN )
	end
end


function KickleTable.FaceLeft( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( LEFT )
	end
end


function KickleTable.FaceRight( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( RIGHT )
	end
end


function KickleTable.WalkUp( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( UP )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( UP + KickleTable.state )
	end
end


function KickleTable.WalkDown( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( DOWN )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( DOWN + KickleTable.state )
	end
end


function KickleTable.WalkLeft( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( LEFT )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( LEFT + KickleTable.state )
	end
end


function KickleTable.WalkRight( Kickle )
	if( KickleTable.state == STANDING ) then
		Kickle:SetDir( RIGHT )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( RIGHT + KickleTable.state )
	end
end


function KickleTable.PerformPillar( Kickle )
	if( KickleTable.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle )

		if ( not Level.TileIsSolid( tileX, tileY ) and
			   not Level.TileHasGridObject( tileX, tileY ) ) then
			KickleTable.state = RAISE_PILLAR
			Level.CreateGameObject(
				"Content/Core/Objects/Pillar.xml", tileX, tileY )

		elseif ( Level.TileHasGridObject( tileX, tileY ) ) then
			local objOnTile = Level.GetGameObjectOnTile( tileX, tileY )
			if( objOnTile:GetType() == "Pillar" ) then
				KickleTable.state = LOWER_PILLAR
				KickleTable.pillar = objOnTile
				KickleTable.pillar:SetAnimation( 1 )
			end
		end
	end
end

-- Spawns an icebreath object if there is not an iceblock infront of it. Else
-- it will set the direction of the iceblock.
function KickleTable.PerformAttack( Kickle )
	if ( KickleTable.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle )
		local objOnTile = Level.GetGameObjectOnTile( tileX, tileY )

		if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
			KickleTable.state = KICKING
			local kickleDir = Kickle:GetDir()
			Kickle:SetAnimation( kickleDir + KickleTable.state );
			objOnTile:GetTable().hasMoved = true
			objOnTile:SetDir( kickleDir );
			objOnTile:Move();

		elseif ( not Level.TileIsSolid( tileX, tileY ) and
				     not Level.TileHasGridObject( tileX, tileY ) )then
			local iceBreath = Level.CreateGameObject(
				"Content/Core/Objects/IceBreath.xml", tileX, tileY );
			local iceBreathdir = Kickle:GetDir();
			iceBreath:SetDir( iceBreathdir );
			iceBreath:SetAnimation( iceBreathdir );
		end
	end
end

return KickleTable
