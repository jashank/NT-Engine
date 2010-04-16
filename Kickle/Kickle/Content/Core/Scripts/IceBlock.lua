package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlockTable = {}

IceBlockTable.hasMoved = false

function IceBlockTable.AILogic( IceBlock )
	if ( IceBlockTable.hasMoved ) then
		IceBlock:Move()

		local facingX, facingY = GetTileObjectFaces( IceBlock )
		if ( Level.GetTile( facingX, facingY ) == WATER ) then
			Level.DestroyGameObject( IceBlock )
			Level.SetTile( facingX, facingY, ICE, 0)
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
