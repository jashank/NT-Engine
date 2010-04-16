-- Pillar Behavior Table

local PillarTable = {}

function PillarTable.HandleCollision( Pillar, Other )
	if ( Other:GetType() == "Slime" ) then
		Level.DestroyGameObject( Pillar )
	end
end

return PillarTable
