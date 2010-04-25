-- Pillar Behavior Table

local PillarTable = {}

PillarTable.raise = false
PillarTable.lower = false


function PillarTable.AILogic( Pillar )
  if ( not PillarTable.raise and not PillarTable.lower ) then
    PillarTable.raise = true
    Pillar:SetAnimation( 0 )

  elseif ( PillarTable.lower and not Pillar:IsAnimating() ) then
    Level.DestroyGameObject( Pillar )
  end
end


function PillarTable.HandleCollision( Pillar, Other )
	if ( Other:GetType() == "Slime" ) then
		Level.DestroyGameObject( Pillar )
	end
end

return PillarTable
