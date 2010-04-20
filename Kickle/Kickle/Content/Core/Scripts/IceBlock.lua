package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlockTable = {}

IceBlockTable.moving = false

function IceBlockTable.AILogic( IceBlock )
	if ( IceBlockTable.moving ) then
		IceBlock:Move()

		local facingX, facingY = GetTileObjectFaces( IceBlock )
        tile = Level.GetTile( facingX, facingY );

		if ( tile == Tiles.WATER  or
             tile == Tiles.WATER_BOTTOM_LEFT or
             tile == Tiles.WATER_BOTTOM_CENTER or
             tile == Tiles.WATER_BOTTOM_RIGHT or
             tile == Tiles.ICE_CLIFF_LEFT or
             tile == Tiles.ICE_CLIFF_CENTER or
             tile == Tiles.ICE_CLIFF_RIGHT ) then
			Level.DestroyGameObject( IceBlock )
			Level.SetTile( facingX, facingY, ICE, 0 )
			Level.CreateGameObject("Content/Core/Objects/Slime.xml", 11, 8 )
		end
	end
end


function IceBlockTable.HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Level.DestroyGameObject( Other )
	end
end

return IceBlockTable
