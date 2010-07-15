package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("GameObjectUtilities")

-- Correspond to Kickle's sprite sheet (each row makes val increment by 1)
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
  otherType = other:GetType()
	if ( otherType == "Slime" or otherType == "Penguin" ) then
		Kickle.state = DYING
		self:SetAnimation( self:GetDir() + Kickle.state )
  
  elseif ( otherType == "DreamBag" and Kickle.state ~= DYING ) then
    Game.DestroyGameObject( other )
  end
end


function Kickle.AI( self )
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
		self:SetDir( Util.UP )
    self:SetAnimation( Util.UP + Kickle.state )
	end
end


function Kickle.FaceDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.DOWN )
    self:SetAnimation( Util.DOWN + Kickle.state )
	end
end


function Kickle.FaceLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.LEFT )
    self:SetAnimation( Util.LEFT + Kickle.state )
	end
end


function Kickle.FaceRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.RIGHT )
    self:SetAnimation( Util.RIGHT + Kickle.state )
	end
end


function Kickle.WalkUp( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.UP )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( Util.UP + Kickle.state )
	end
end


function Kickle.WalkDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.DOWN )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( Util.DOWN + Kickle.state )
	end
end


function Kickle.WalkLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.LEFT )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( Util.LEFT + Kickle.state )
	end
end


function Kickle.WalkRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.RIGHT )
		Kickle.state = WALKING
		self:Move()
		self:SetAnimation( Util.RIGHT + Kickle.state )
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
		local tileX, tileY = Util.GetTileObjectFaces( self )

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
		local tileX, tileY = Util.GetTileObjectFaces( self )
    local kickleDir = self:GetDir()
		local objOnTile = Game.GetGameObjectOnTile( tileX, tileY )

		if ( objOnTile and objOnTile:GetType() == "IceBlock" ) then
			Kickle.state = KICKING
			self:SetAnimation( kickleDir + Kickle.state );

      local blockFacingX, blockFacingY =
        Util.GetTileInDirection( objOnTile, kickleDir )
      local tileType = Game.GetTileInfo( blockFacingX, blockFacingY )
      if (( Game.TileIsCrossable( blockFacingX, blockFacingY ) or
            tileType == "water" ) and
            not Game.ObjectBlockingTile( blockFacingX, blockFacingY )) then
			  objOnTile:GetTable().moving = true
			  objOnTile:SetDir( kickleDir );
      else 
        Game.DestroyGameObject( objOnTile )
        Game.CreateGameObject( 
          "Kickle_Pack/Objects/Slime.xml",
          objOnTile:GetTable().slimeSpawnX,
          objOnTile:GetTable().slimeSpawnY
        )
      end
    
    elseif ( objOnTile and objOnTile:GetType() == "Penguin" and 
             objOnTile:GetTable().frozen == true ) then
      Kickle.state = KICKING
      self:SetAnimation( kickleDir + Kickle.state )
      Game.DestroyGameObject( objOnTile )

		elseif (( Game.TileIsCrossable( tileX, tileY ) or
              Game.GetTileInfo( tileX, tileY ) == "water" ) and
              not Game.ObjectBlockingTile( tileX, tileY ) ) then
			local iceBreath = Game.CreateGameObject(
				"Kickle_Pack/Objects/IceBreath.xml", tileX, tileY )
			local iceBreathdir = self:GetDir()
			iceBreath:SetDir( iceBreathdir )
			iceBreath:SetAnimation( iceBreathdir )
		end
	end
end


function Kickle.GodMode( self )
  Kickle.godMode = not Kickle.godMode 
  self:SetNoClip( Kickle.godMode )
end

return Kickle
