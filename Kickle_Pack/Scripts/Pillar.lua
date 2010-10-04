package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require( "ObjectUtilities" )

-- Pillar Behavior Table

local Pillar = {}

Pillar.lower = false

function Pillar.Init( self )
  Util.SetAndPlay( self, 0 )
end


function Pillar.AI( self )
  if ( Pillar.lower and not self:IsAnimating() ) then
    State.DestroyObject( self )
  end
end


function Pillar.HandleCollision( self, other )
  local otherType = other:GetType()
        if ( otherType == "Slime" or otherType == "DreamBag" ) then
                State.DestroyObject( self )
        end
end


function Pillar.Lower( self )
  Pillar.lower = true
  self:SetReverseAnim( true )
  self:PlayAnim( 0 )
end
  
  

return Pillar
