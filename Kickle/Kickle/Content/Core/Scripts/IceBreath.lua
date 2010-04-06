-- IceBreath script

local lastTileX = -1;
local lastTileY = -1;

function AILogic( IceBreath )
	tileX = IceBreath:GetTileX();
	tileY = IceBreath:GetTileY();

	if ( tileX == lastTileX and tileY == lastTileY ) then
		Game.DestroyGameObject( IceBreath );
	else
		lastTileX = tileX;
		lastTileY = tileY;
		IceBreath:Move();
	end

end

function HandleCollision( IceBreath, Other )
	if ( Other:GetType() == "Slime" ) then
		Game.CreateGameObject(
			"Content/Core/Objects/IceBlock.xml",
			 Other:GetTileX(),
			 Other:GetTileY()
		);
		Game.DestroyGameObject( Other );
	end
end
