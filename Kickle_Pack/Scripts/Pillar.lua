-- Pillar Behavior Table

local Pillar = {}

Pillar.lower = false

function Pillar.Init( self )
  self:PlayAnimation( 0 )
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

return Pillar
