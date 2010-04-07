-- IceBreath script

local lastTileX = -1;
local lastTileY = -1;

function AILogic( IceBreath )
	tileX = IceBreath:GetTileX();
	tileY = IceBreath:GetTileY();

	if ( tileX == lastTileX and tileY == lastTileY ) then
		Level.DestroyGameObject( IceBreath );
	else
		lastTileX = tileX;
		lastTileY = tileY;
		IceBreath:Move();
	end

end

function HandleCollision( IceBreath, Other )
	if ( Other:GetType() == "Slime" ) then
		Level.CreateGameObject(
			"Content/Core/Objects/IceBlock.xml",
			 Other:GetTileX(),
			 Other:GetTileY()
		);
		Level.DestroyGameObject( Other );
		Level.DestroyGameObject( IceBreath );
	end
end
