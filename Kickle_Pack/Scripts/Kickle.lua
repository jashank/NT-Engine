package.path = package.path .. ";self_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

-- These values are determined by the layout of self's animation xml file
local STANDING = 0
local WALKING = 4
local KICKING = 8
local PILLAR = 12
local DYING = 16

--Kickle Behavior Table

local Kickle = {}

Kickle.state = STANDING --Current state kickle is in
Kickle.godMode = false

function Kickle.HandleCollision( self, other )
	if ( other:GetType() == "Slime" ) then
		Kickle.state = DYING
		self:SetAnimation( self:GetDir() + Kickle.state )
	end
end


function Kickle.AILogic( self )
  if ( Kickle.state == WALKING ) then
    Kickle.state = STANDING
    self:SetAnimation( self:GetDir() + Kickle.state )

  elseif ( Kickle.state ~= DYING and not self:IsAnimating() ) then
    Kickle.state = STANDING
    self:SetAnimation( self:GetDir() + Kickle.state )
  end
end


function Kickle.FaceUp( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( UP )
    self:SetAnimation( UP + Kickle.state )
	end
end


function Kickle.FaceDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( DOWN )
    self:SetAnimation( DOWN + Kickle.state )
	end
end


function Kickle.FaceLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( LEFT )
    self:SetAnimation( LEFT + Kickle.state )
	end
end


function Kickle.FaceRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( RIGHT )
    self:SetAnimation( RIGHT + Kickle.state )
	end
end


function Kickle.WalkUp( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( UP )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( UP + Kickle.state )
	end
end


function Kickle.WalkDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( DOWN )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( DOWN + Kickle.state )
	end
end


function Kickle.WalkLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( LEFT )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( LEFT + Kickle.state )
	end
end


function Kickle.WalkRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( RIGHT )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( RIGHT + Kickle.state )
	end
end


function Kickle.Suicide( self )
  if ( Kickle.state ~= DYING ) then
    Kickle.state = DYING
    self:SetAnimation( self:GetDir() + Kickle.state )
  end
end


function Kickle.PerformPillar( self )
	if( Kickle.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( self )

		if ( Game.TileIsCrossable( tileX, tileY ) and
			   not Game.ObjectBlockingTile( tileX, tileY ) ) then
			Kickle.state = PILLAR
      self:SetAnimation( self:GetDir() + Kickle.state )
			local pillar = Game.CreateGameObject(
				"Kickle_Pack/Objects/Pillar.xml", tileX, tileY )

		elseif ( Game.ObjectBlockingTile( tileX, tileY ) ) then
			local objOnTile = Game.GetGameObjectOnTile( tileX, tileY )
			if( objOnTile:GetType() == "Pillar" ) then
				Kickle.state = PILLAR
        self:SetAnimationReverse( self:GetDir() + Kickle.state )
        objOnTile:GetTable().lower = true
        objOnTile:SetAnimationReverse( 0 )
			end
		end
	end
end


function Kickle.PerformAttack( self )
	if ( Kickle.state == STANDING ) then
		local tileX, tileY = GetTileObjectFaces( self )
		local objOnTile = Game.GetGameObjectOnTile( tileX, tileY )

		if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
			Kickle.state = KICKING
			local kickleDir = self:GetDir()
			self:SetAnimation( kickleDir + Kickle.state );
			objOnTile:GetTable().moving = true
			objOnTile:SetDir( kickleDir );

		elseif (( Game.TileIsCrossable( tileX, tileY ) or
              Game.GetTileInfo( tileX, tileY ) == "water" ) and
              not Game.ObjectBlockingTile( tileX, tileY ) ) then
			local iceBreath = Game.CreateGameObject(
				"Kickle_Pack/Objects/IceBreath.xml", tileX, tileY );
			local iceBreathdir = self:GetDir();
			iceBreath:SetDir( iceBreathdir );
			iceBreath:SetAnimation( iceBreathdir );
		end
	end
end


function Kickle.GodMode( self )
  Kickle.godMode = not Kickle.godMode 
  self:SetNoClip( Kickle.godMode )
end

return Kickle
