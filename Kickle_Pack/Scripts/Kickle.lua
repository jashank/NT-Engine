package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

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
  -- Things that kill Kickle
	if ( otherType == "Slime" or otherType == "Penguin" or 
       otherType == "IceBlock" ) then
    if Kickle.state ~= DYING then
      Kickle.state = DYING
      self:PlayAnimation( self:GetDir() + Kickle.state )
    end

  elseif ( otherType == "DreamBag" and Kickle.state ~= DYING ) then
    State.DestroyObject( other )
  end
end


function Kickle.AI( self )
  if ( Kickle.state == WALKING ) then
    Kickle.state = STANDING
    self:PlayAnimation( self:GetDir() + Kickle.state )

  elseif ( Kickle.state ~= DYING and not self:IsAnimating() ) then
    Kickle.state = STANDING
    self:PlayAnimation( self:GetDir() + Kickle.state )
  end
end


function Kickle.FaceUp( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.UP )
    self:PlayAnimation( Util.UP + Kickle.state )
	end
end


function Kickle.FaceDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.DOWN )
    self:PlayAnimation( Util.DOWN + Kickle.state )
	end
end


function Kickle.FaceLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.LEFT )
    self:PlayAnimation( Util.LEFT + Kickle.state )
	end
end


function Kickle.FaceRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.RIGHT )
    self:PlayAnimation( Util.RIGHT + Kickle.state )
	end
end


function Kickle.WalkUp( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.UP )
		Kickle.state = WALKING
		self:Move()
		self:PlayAnimation( Util.UP + Kickle.state )
	end
end


function Kickle.WalkDown( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.DOWN )
		Kickle.state = WALKING
		self:Move()
		self:PlayAnimation( Util.DOWN + Kickle.state )
	end
end


function Kickle.WalkLeft( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.LEFT )
		Kickle.state = WALKING
		self:Move()
		self:PlayAnimation( Util.LEFT + Kickle.state )
	end
end


function Kickle.WalkRight( self )
	if ( Kickle.state == STANDING ) then
		self:SetDir( Util.RIGHT )
		Kickle.state = WALKING
		self:Move()
		self:PlayAnimation( Util.RIGHT + Kickle.state )
	end
end


function Kickle.Suicide( self )
  if ( Kickle.state ~= DYING ) then
    Kickle.state = DYING
    self:PlayAnimation( self:GetDir() + Kickle.state )
  end
end


function Kickle.PerformPillar( self )
	if( Kickle.state == STANDING ) then
		local tileX, tileY = Util.GetTileObjectFaces( self )

		if ( State.TileIsCrossable( tileX, tileY ) and
			   not State.ObjectBlockingTile( tileX, tileY ) ) then
			Kickle.state = PILLAR
      self:PlayAnimation( self:GetDir() + Kickle.state )
			local pillar = State.CreateObject(
				"Kickle_Pack/Objects/Pillar.xml", tileX, tileY )

		elseif ( State.ObjectBlockingTile( tileX, tileY ) ) then
			local objOnTile = State.GetObjectOnTile( tileX, tileY )
			if( objOnTile:GetType() == "Pillar" ) then
				Kickle.state = PILLAR
        self:PlayAnimationReverse( self:GetDir() + Kickle.state )
        objOnTile:GetTable().lower = true
        objOnTile:PlayAnimationReverse( 0 )
			end
		end
	end
end


function Kickle.PerformAttack( self )
	if ( Kickle.state == STANDING ) then
		local tileX, tileY = Util.GetTileObjectFaces( self )
    local kickleDir = self:GetDir()
		local objOnTile = State.GetObjectOnTile( tileX, tileY )

    if objOnTile then
      objType = objOnTile:GetType()

      if objType == "IceBlock" then
        Kickle.state = KICKING
        self:PlayAnimation( kickleDir + Kickle.state );

        local blockFacingX, blockFacingY =
          Util.GetTileInDirection( objOnTile, kickleDir )
        local tileType = State.GetTileInfo( blockFacingX, blockFacingY )
        if (( State.TileIsCrossable( blockFacingX, blockFacingY ) or
              tileType == "water" ) and
              not State.ObjectBlockingTile( blockFacingX, blockFacingY )) then
          objOnTile:GetTable().moving = true
          objOnTile:SetDir( kickleDir );
        else 
          objOnTile:GetTable().destroyed = true
        end
        return
    
      elseif objType == "Penguin" and 
             objOnTile:GetTable().frozen == true then
        Kickle.state = KICKING
        self:PlayAnimation( kickleDir + Kickle.state )
        State.DestroyObject( objOnTile )
        return

      elseif objType == "PowerRock" then
        Kickle.state = KICKING
        self:PlayAnimation( kickleDir + Kickle.state )
        objOnTile:GetTable().kicked = true
        return
      end
    end

		if (( State.TileIsCrossable( tileX, tileY ) or
          State.GetTileInfo( tileX, tileY ) == "water" ) and
          not State.ObjectBlockingTile( tileX, tileY ) ) then
			local iceBreath = State.CreateObject(
				"Kickle_Pack/Objects/IceBreath.xml", tileX, tileY )
			local iceBreathdir = self:GetDir()
			iceBreath:SetDir( iceBreathdir )
			iceBreath:PlayAnimation( iceBreathdir )
		end
	end
end


function Kickle.GodMode( self )
  Kickle.godMode = not Kickle.godMode 
  self:SetNoClip( Kickle.godMode )
end

return Kickle
