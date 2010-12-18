package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require( "ObjectUtilities" )

-- Correspond to Kickle's sprite sheet (each row makes val increment by 1)
local STANDING = 0
local WALKING = 4
local KICKING = 8
local PILLAR = 12
local DYING = 16

-- Kickle Behavior Table

local Kickle = require("Entity"):New()
Kickle.state = STANDING
Kickle.godMode = false

function Kickle:HandleCollision( kickle, other )
  local otherType = other:GetType()
  if ( otherType == "DreamBag" and self.state ~= DYING ) then
    Map.DestroyObject( other )
  end
end


function Kickle:AI( kickle )
  if ( self.state == WALKING ) then
    self.state = STANDING
    kickle:SetAnim( kickle:GetDir() + self.state )
  elseif ( self.state ~= DYING and not kickle:IsAnimating() ) then
    self.state = STANDING
    kickle:SetAnim( kickle:GetDir() + self.state )
  end
  kickle:PlayAnim()
end


function Kickle:FaceUp( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.UP )
    Util.SetAndPlay( kickle, Util.UP + self.state )
  end
end


function Kickle:FaceDown( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.DOWN )
    Util.SetAndPlay( kickle, Util.DOWN + self.state )
  end
end


function Kickle:FaceLeft( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.LEFT )
    Util.SetAndPlay( kickle, Util.LEFT + self.state )
  end
end


function Kickle:FaceRight( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.RIGHT )
    Util.SetAndPlay( kickle, Util.RIGHT + self.state )
  end
end


function Kickle:WalkUp( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.UP )
    self.state = WALKING
    kickle:Move()
    Util.SetAndPlay( kickle, Util.UP + self.state )
  end
end


function Kickle:WalkDown( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.DOWN )
    self.state = WALKING
    kickle:Move()
    Util.SetAndPlay( kickle, Util.DOWN + self.state )
  end
end


function Kickle:WalkLeft( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.LEFT )
    self.state = WALKING
    kickle:Move()
    Util.SetAndPlay( kickle, Util.LEFT + self.state )
  end
end


function Kickle:WalkRight( kickle )
  if ( self.state ~= DYING ) then
    kickle:SetDir( Util.RIGHT )
    self.state = WALKING
    kickle:Move()
    Util.SetAndPlay( kickle, Util.RIGHT + self.state )
  end
end


function Kickle:PerformPillar( kickle )
  if ( self.state == STANDING ) then
    local tileX, tileY = Util.GetTileObjectFaces( kickle )

    if ( Map.TileIsCrossable( tileX, tileY ) and
         not Map.ObjectBlockingTile( tileX, tileY ) ) then
      self.state = PILLAR
      Util.SetAndPlay( kickle, kickle:GetDir() + self.state )
      local pillar = Map.CreateObject(
                       "Kickle_Pack/Objects/Pillar.xml", tileX, tileY )

    elseif ( Map.ObjectBlockingTile( tileX, tileY ) ) then
      local objOnTile = Map.GetObjectOnTile( tileX, tileY )
      if( objOnTile:GetType() == "Pillar" ) then
        self.state = PILLAR
        kickle:SetReverseAnim( true )
        Util.SetAndPlay( kickle, kickle:GetDir() + self.state )
        objOnTile:GetTable():Lower( objOnTile )
      end
    end
  end
end


function Kickle:PerformAttack( kickle )
  if ( self.state == STANDING ) then
    local tileX, tileY = Util.GetTileObjectFaces( kickle )
    local kickleDir = kickle:GetDir()
    local objOnTile = Map.GetObjectOnTile( tileX, tileY )

    if objOnTile and objOnTile:GetTable():IsFreezable() then
      objType = objOnTile:GetType()
      objTable = objOnTile:GetTable()

      if objType == "IceBlock" then
        self.state = KICKING
        Util.SetAndPlay( kickle, kickleDir + self.state );
        objTable:Kick( objOnTile, kickleDir )
        return

      elseif objTable:IsEnemy() and objTable:IsFrozen() then
        self.state = KICKING
        Util.SetAndPlay( kickle, kickleDir + self.state )
        Map.DestroyObject( objOnTile )
        return
      end
    end

    local iceBreath = Map.CreateObject(
                       "Kickle_Pack/Objects/IceBreath.xml", tileX, tileY )
    iceBreath:SetDir( kickle:GetDir()  )
    Util.SetAndPlay( iceBreath, 0 )
  end
end


function Kickle:GodMode( kickle )
  self.godMode = not self.godMode 
  kickle:SetNoClip( self.godMode )
end


function Kickle:Kill( kickle )
  if self.state ~= DYING then
    self.state = DYING
    Util.SetAndPlay( kickle, kickle:GetDir() + self.state )
    kickle:SetRenderPriority( -2 )
  end
end

return Kickle

