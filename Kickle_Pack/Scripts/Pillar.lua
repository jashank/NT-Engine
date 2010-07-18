-- Pillar Behavior Table

local Pillar = {}

Pillar.raise = false
Pillar.lower = false


function Pillar.AI( self )
  if ( not Pillar.raise and not Pillar.lower ) then
    Pillar.raise = true
    self:SetAnimation( 0 )

  elseif ( Pillar.lower and not self:IsAnimating() ) then
    State.DestroyObject( self )
  end
end


function Pillar.HandleCollision( self, other )
  local otherType = other:GetType()
	if ( otherType == "Slime" or otherType == "DreamBag" ) then
		State.DestroyObject( self )
	end
end

return Pillar
