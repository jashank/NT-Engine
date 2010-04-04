-- Pillar's Script

function HandleCollision( Pillar, Other )
	if ( Other:GetType() == "Slime" ) then
		Game.DestroyGameObject( Pillar );
	end
end
