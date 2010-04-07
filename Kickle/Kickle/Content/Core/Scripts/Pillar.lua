-- Pillar's Script

function HandleCollision( Pillar, Other )
	if ( Other:GetType() == "Slime" ) then
		Level.DestroyGameObject( Pillar );
	end
end
