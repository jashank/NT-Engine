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

function KickleTable.HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		KickleTable.state = DYING
		Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
	end
end


function KickleTable.AILogic( Kickle )
  if ( KickleTable.state == WALKING ) then
    KickleTable.state = STANDING
    Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )

  elseif ( KickleTable.state ~= DYING and not Kickle:IsAnimating() ) then
    KickleTable.state = STANDING
    Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
  end

	if ( not Level.GetGameObject( "DreamBag" ) ) then
		Level.NextLevel()
	end
end


function KickleTable.FaceUp( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( UP )
    Kickle:SetAnimation( UP + KickleTable.state )
	end
end


function KickleTable.FaceDown( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( DOWN )
    Kickle:SetAnimation( DOWN + KickleTable.state )
	end
end


function KickleTable.FaceLeft( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( LEFT )
    Kickle:SetAnimation( LEFT + KickleTable.state )
	end
end


function KickleTable.FaceRight( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( RIGHT )
    Kickle:SetAnimation( RIGHT + KickleTable.state )
	end
end


function KickleTable.WalkUp( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( UP )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( UP + KickleTable.state )
	end
end


function KickleTable.WalkDown( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( DOWN )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( DOWN + KickleTable.state )
	end
end


function KickleTable.WalkLeft( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( LEFT )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( LEFT + KickleTable.state )
	end
end


function KickleTable.WalkRight( Kickle )
	if ( KickleTable.state == STANDING ) then
		Kickle:SetDir( RIGHT )
		KickleTable.state = WALKING
		Kickle:Move()
		Kickle:SetAnimation( RIGHT + KickleTable.state )
	end
end


function KickleTable.Suicide( Kickle )
  if ( KickleTable.state ~= DYING ) then
    KickleTable.state = DYING
    Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
  end
end


function KickleTable.PerformPillar( Kickle )
	if( KickleTable.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle )

		if ( Level.TileIsCrossable( tileX, tileY ) and
			   not Level.TileHasGridObject( tileX, tileY ) ) then
			KickleTable.state = RAISE_PILLAR
      Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
			local pillar = Level.CreateGameObject(
				"Content/Core/Objects/Pillar.xml", tileX, tileY )

		elseif ( Level.TileHasGridObject( tileX, tileY ) ) then
			local objOnTile = Level.GetGameObjectOnTile( tileX, tileY )
			if( objOnTile:GetType() == "Pillar" ) then
				KickleTable.state = LOWER_PILLAR
        Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
        objOnTile:GetTable().lower = true
        objOnTile:SetAnimation( 1 )
			end
		end
	end
end


function KickleTable.PerformAttack( Kickle )
	if ( KickleTable.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle )
		local objOnTile = Level.GetGameObjectOnTile( tileX, tileY )

		if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
			KickleTable.state = KICKING
			local kickleDir = Kickle:GetDir()
			Kickle:SetAnimation( kickleDir + KickleTable.state );
			objOnTile:GetTable().moving = true
			objOnTile:SetDir( kickleDir );

		elseif (( Level.TileIsCrossable( tileX, tileY ) or
              Level.GetTileInfo( tileX, tileY ) == "water" ) and
              not Level.TileHasGridObject( tileX, tileY ) ) then
			local iceBreath = Level.CreateGameObject(
				"Content/Core/Objects/IceBreath.xml", tileX, tileY );
			local iceBreathdir = Kickle:GetDir();
			iceBreath:SetDir( iceBreathdir );
			iceBreath:SetAnimation( iceBreathdir );
		end
	end
end

return KickleTable
