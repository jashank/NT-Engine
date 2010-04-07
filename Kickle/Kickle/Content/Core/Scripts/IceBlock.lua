--IceBlock's Script

--IceBlock's Script

package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities");


local WATER = 10;
local ICE = 1;

function AILogic( IceBlock )
	tileX = IceBlock:GetTileX();
	tileY = IceBlock:GetTileY();

	if ( IceBlock:HasMoved() ) then
		IceBlock:Move();
		facingX, facingY = GetTileObjectFaces( IceBlock );
		if ( Level.GetTile( facingX, facingY ) == WATER ) then
			Level.DestroyGameObject( IceBlock );
			Level.SetTile( facingX, facingY, ICE, 0);
			Level.CreateGameObject("Content/Core/Objects/Slime.xml", 11, 8 );
		end
	end
end

function HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Level.DestroyGameObject( Other );
	end
end
