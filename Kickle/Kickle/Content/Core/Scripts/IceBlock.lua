--IceBlock's Script

--IceBlock's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");

local lastTileX = -1;
local lastTileY = -1;

function AILogic( IceBlock )
	tileX = IceBlock:GetTileX();
	tileY = IceBlock:GetTileY();

	if ( lastTileX ~= -1 and lastTileY ~= -1 ) then
		if ( lastTileX ~= tileX or lastTileY ~= tileY ) then
			IceBlock:Move();
		end

		facingX, facingY = GetTileObjectFaces( IceBlock );
		-- if tille at facingX facingY is a water tile then
		-- convert it to an ice tile
		-- @Ben, I've made lua funcs available
	end

	lastTileX = tileX;
	lastTileY = tileY;

end

function HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Level.DestroyGameObject( Other );
	end
end
