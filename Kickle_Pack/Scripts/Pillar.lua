package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require( "ObjectUtilities" )

local Pillar = require("Entity"):New()
Pillar.lower = false

function Pillar:Init( pillar )
  Util.SetAndPlay( pillar, 0 )
end


function Pillar:AI( pillar )
  if ( self.lower and not pillar:IsAnimating() ) then
    State.DestroyObject( pillar )
  end
end


function Pillar:Lower( pillar )
  self.lower = true
  pillar:SetReverseAnim( true )
  pillar:PlayAnim( 0 )
end
  
return Pillar

