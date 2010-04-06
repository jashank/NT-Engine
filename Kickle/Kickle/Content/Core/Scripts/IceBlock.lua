--IceBlock's Script

local lastTileX = -1;
local lastTileY = -1;
local kicked = 0;

function AILogic( IceBlock )
	tileX = IceBlock:GetTileX();
	tileY = IceBlock:GetTileY();

	if ( lastTileX ~= -1 and lastTileY ~= -1 ) then
		if ( lastTileX ~= tileX or lastTileY ~= tileY ) then
			IceBlock:Move();
		end
	end

	lastTileX = tileX;
	lastTileY = tileY;

end

function HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Game.DestroyGameObject( Other );
	elseif ( Other:GetType() == "Kickle" ) then
		Other:Reverse();
	end
end
