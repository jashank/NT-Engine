package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

-- These values are determined by the layout of Kickle's animation xml file


--Kickle's Behavior Table
local KickleTable = {}

KickleTable.state = 0 -- Current state kickle is in
KickleTable.pillar = nil

function KickleTable.HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		KickleTable.state = KickleAnimation.UP_DYING
		Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
	end
end


function KickleTable.AILogic( Kickle )

	if( KickleTable.state ~= KickleAnimation.UP_DYING ) then
		Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )

		if( KickleTable.state ~= KickleAnimation.UP_RAISE_PILLAR  and
			  KickleTable.state ~= KickleAnimation.UP_LOWER_PILLAR ) then
			KickleTable.state = KickleAnimation.UP_STANDING
		end

		if (( KickleTable.state == KickleAnimation.UP_RAISE_PILLAR or
			    KickleTable.state == KickleAnimation.UP_LOWER_PILLAR ) and not
			    Kickle:IsAnimating() ) then
			 KickleTable.state = KickleAnimation.UP_STANDING
		end
	end

	if( KickleTable.pillar and not KickleTable.pillar:IsAnimating() ) then
		Level.DestroyGameObject( KickleTable.pillar )
		KickleTable.pillar = nil
	end

	if ( not Level.GetGameObject( "DreamBag" ) ) then
		Level.NextLevel()
	end
end


function KickleTable.FaceUp( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( UP )
	end
end


function KickleTable.FaceDown( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( DOWN )
	end
end


function KickleTable.FaceLeft( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( LEFT )
	end
end


function KickleTable.FaceRight( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( RIGHT )
	end
end


function KickleTable.WalkUp( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( UP )
		KickleTable.state = KickleAnimation.UP_WALKING
		Kickle:Move()
		Kickle:SetAnimation( KickleTable.state )
	end
end


function KickleTable.WalkDown( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( DOWN )
		KickleTable.state = KickleAnimation.DOWN_WALKING
		Kickle:Move()
		Kickle:SetAnimation( KickleTable.state )
	end
end


function KickleTable.WalkLeft( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( LEFT )
		KickleTable.state = KickleAnimation.LEFT_WALKING
		Kickle:Move()
		Kickle:SetAnimation( KickleTable.state )
	end
end


function KickleTable.WalkRight( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		Kickle:SetDir( RIGHT )
		KickleTable.state = KickleAnimation.RIGHT_WALKING
		Kickle:Move()
		Kickle:SetAnimation( KickleTable.state )
	end
end


function KickleTable.PerformPillar( Kickle )
	if( KickleTable.state == KickleAnimation.UP_STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle )

		if ( not Level.TileIsSolid( tileX, tileY ) and
			   not Level.TileHasGridObject( tileX, tileY ) ) then
			KickleTable.state = KickleAnimation.UP_RAISE_PILLAR
			Level.CreateGameObject(
				"Content/Core/Objects/Pillar.xml", tileX, tileY )

		elseif ( Level.TileHasGridObject( tileX, tileY ) ) then
			local objOnTile = Level.GetGameObjectOnTile( tileX, tileY )
			if( objOnTile:GetType() == "Pillar" ) then
				KickleTable.state = KickleAnimation.UP_LOWER_PILLAR
				KickleTable.pillar = objOnTile
				KickleTable.pillar:SetAnimation( PillarAnimation.PILLAR_LOWER )
			end
		end
	end
end

-- Spawns an icebreath object if there is not an iceblock infront of it. Else
-- it will set the direction of the iceblock.
function KickleTable.PerformAttack( Kickle )
	if ( KickleTable.state == KickleAnimation.UP_STANDING ) then
		local tileX, tileY = GetTileObjectFaces( Kickle );
		local objOnTile = Level.GetGameObjectOnTile( tileX, tileY );
        local kickleDir = Kickle:GetDir();

		if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
			KickleTable.state = KickleAnimation.UP_KICKING
			Kickle:SetAnimation( kickleDir + KickleTable.state );
			objOnTile:GetTable().moving = true;
			objOnTile:SetDir( kickleDir );
			objOnTile:Move();

		elseif ( not Level.TileIsSolid( tileX, tileY ) and
				     not Level.TileHasGridObject( tileX, tileY ) )then
			local iceBreath = Level.CreateGameObject(
				"Content/Core/Objects/IceBreath.xml", tileX, tileY );
			iceBreath:SetDir( kickleDir );
			iceBreath:SetAnimation( kickleDir );
		end
	end
end

return KickleTable
