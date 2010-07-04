package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

-- These values are determined by the layout of Kickle's animation xml file
local STANDING = 0
local WALKING = 4
local KICKING = 8
local PILLAR = 12
local DYING = 16

--Kickle's Behavior Table

local KickleTable = {}

KickleTable.state = STANDING --Current state kickle is in

function KickleTable.HandleCollision( Kickle, Other )
  if ( Other:GetType() == "Slime" ) then
    if ( KickleTable.state ~= DYING ) then     
      KickleTable.state = DYING
      Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
    end
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

        if ( not Game.GetGameObject( "DreamBag" ) ) then
                Game.NextLevel()
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

                if ( Game.TileIsCrossable( tileX, tileY ) and
                           not Game.TileHasGridObject( tileX, tileY ) ) then
                        KickleTable.state = PILLAR
      Kickle:SetAnimation( Kickle:GetDir() + KickleTable.state )
                        local pillar = Game.CreateGameObject(
                                "Kickle_Pack/Objects/Pillar.xml", tileX, tileY )

                elseif ( Game.TileHasGridObject( tileX, tileY ) ) then
                        local objOnTile = Game.GetGameObjectOnTile( tileX, tileY )
                        if( objOnTile:GetType() == "Pillar" ) then
                                KickleTable.state = PILLAR
        Kickle:SetAnimationReverse( Kickle:GetDir() + KickleTable.state )
        objOnTile:GetTable().lower = true
        objOnTile:SetAnimationReverse( 0 )
                        end
                end
        end
end


function KickleTable.PerformAttack( Kickle )
        if ( KickleTable.state == STANDING ) then
                local tileX, tileY = GetTileObjectFaces( Kickle )
                local objOnTile = Game.GetGameObjectOnTile( tileX, tileY )

                if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
                        KickleTable.state = KICKING
                        local kickleDir = Kickle:GetDir()
                        Kickle:SetAnimation( kickleDir + KickleTable.state );
                        objOnTile:GetTable().moving = true
                        objOnTile:SetDir( kickleDir );

                elseif (( Game.TileIsCrossable( tileX, tileY ) or
              Game.GetTileInfo( tileX, tileY ) == "water" ) and
              not Game.TileHasGridObject( tileX, tileY ) ) then
                        local iceBreath = Game.CreateGameObject(
                                "Kickle_Pack/Objects/IceBreath.xml", tileX, tileY );
                        local iceBreathdir = Kickle:GetDir();
                        iceBreath:SetDir( iceBreathdir );
                        iceBreath:SetAnimation( iceBreathdir );
                end
        end
end

return KickleTable
