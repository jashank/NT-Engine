package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require( "ObjectUtilities" )

local Pillar = require("Entity")

Pillar.lower = false

function Pillar.Init( self )
  Util.SetAndPlay( self, 0 )
end


function Pillar.AI( self )
  if ( Pillar.lower and not self:IsAnimating() ) then
    State.DestroyObject( self )
  end
end


function Pillar.Lower( self )
  Pillar.lower = true
  self:SetReverseAnim( true )
  self:PlayAnim( 0 )
end
  
  

return Pillar

