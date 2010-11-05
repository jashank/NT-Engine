package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require( "ObjectUtilities" )

-- Correspond to Kickle's sprite sheet (each row makes val increment by 1)
local STANDING = 0
local WALKING = 4
local KICKING = 8
local PILLAR = 12
local DYING = 16

-- Kickle Behavior Table

local Kickle = {}

Kickle.state = STANDING -- Current state kickle is in
Kickle.godMode = false

function Kickle.HandleCollision( self, other )
  local otherType = other:GetType()
  -- Things that kill Kickle
  if ( otherType == "Slime" or otherType == "Penguin" or 
       otherType == "IceBlock" or otherType == "Chicken" ) then
    if Kickle.state ~= DYING then
      Kickle.state = DYING
      Util.SetAndPlay( self, self:GetDir() + Kickle.state )
      self:SetRenderPriority( -2 )
    end

  elseif ( otherType == "DreamBag" and Kickle.state ~= DYING ) then
    State.DestroyObject( other )
  end
end


function Kickle.AI( self )
  if ( Kickle.state == WALKING ) then
    Kickle.state = STANDING
    self:SetAnim( self:GetDir() + Kickle.state )
  elseif ( Kickle.state ~= DYING and not self:IsAnimating() ) then
    Kickle.state = STANDING
    self:SetAnim( self:GetDir() + Kickle.state )
  end
  self:PlayAnim()
end


function Kickle.FaceUp( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.UP )
    Util.SetAndPlay( self, Util.UP + Kickle.state )
  end
end


function Kickle.FaceDown( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.DOWN )
    Util.SetAndPlay( self, Util.DOWN + Kickle.state )
  end
end


function Kickle.FaceLeft( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.LEFT )
    Util.SetAndPlay( self, Util.LEFT + Kickle.state )
  end
end


function Kickle.FaceRight( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.RIGHT )
    Util.SetAndPlay( self, Util.RIGHT + Kickle.state )
  end
end


function Kickle.WalkUp( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.UP )
    Kickle.state = WALKING
    self:Move()
    Util.SetAndPlay( self, Util.UP + Kickle.state )
  end
end


function Kickle.WalkDown( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.DOWN )
    Kickle.state = WALKING
    self:Move()
    Util.SetAndPlay( self, Util.DOWN + Kickle.state )
  end
end


function Kickle.WalkLeft( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.LEFT )
    Kickle.state = WALKING
    self:Move()
    Util.SetAndPlay( self, Util.LEFT + Kickle.state )
  end
end


function Kickle.WalkRight( self )
  if ( Kickle.state ~= DYING ) then
    self:SetDir( Util.RIGHT )
    Kickle.state = WALKING
    self:Move()
    Util.SetAndPlay( self, Util.RIGHT + Kickle.state )
  end
end


function Kickle.PerformPillar( self )
  if ( Kickle.state == STANDING ) then
    local tileX, tileY = Util.GetTileObjectFaces( self )

    if ( State.TileIsCrossable( tileX, tileY ) and
         not State.ObjectBlockingTile( tileX, tileY ) ) then
      Kickle.state = PILLAR
      Util.SetAndPlay( self, self:GetDir() + Kickle.state )
      local pillar = State.CreateObject(
                       "Kickle_Pack/Objects/Pillar.xml", tileX, tileY )

    elseif ( State.ObjectBlockingTile( tileX, tileY ) ) then
      local objOnTile = State.GetObjectOnTile( tileX, tileY )
      if( objOnTile:GetType() == "Pillar" ) then
        Kickle.state = PILLAR
        self:SetReverseAnim( true )
        Util.SetAndPlay( self, self:GetDir() + Kickle.state )
        objOnTile:GetTable().Lower( objOnTile )
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
        Util.SetAndPlay( self, kickleDir + Kickle.state );
        objOnTile:GetTable().Kick( objOnTile, kickleDir )
        return

      elseif objType == "Penguin" or objType == "Chicken" and 
             objOnTile:GetTable().frozen then
        Kickle.state = KICKING
        Util.SetAndPlay( self, kickleDir + Kickle.state )
        State.DestroyObject( objOnTile )
        return
      end
    end

    if (( State.TileIsCrossable( tileX, tileY ) or
          State.GetTileInfo( tileX, tileY ) == "water" ) and
          not State.ObjectBlockingTile( tileX, tileY ) ) then
      local iceBreath = State.CreateObject(
                         "Kickle_Pack/Objects/IceBreath.xml", tileX, tileY )
      local iceBreathDir = self:GetDir()
      iceBreath:SetDir( iceBreathDir )
      Util.SetAndPlay( iceBreath, iceBreathDir )
    end
  end
end


function Kickle.GodMode( self )
  Kickle.godMode = not Kickle.godMode 
  self:SetNoClip( Kickle.godMode )
end

return Kickle

