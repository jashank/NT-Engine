package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

-- IceBreath Behavior Table

local IceBreathTable = {}

IceBreathTable.lastTileX = -1
IceBreathTable.lastTileY = -1

function IceBreathTable.AILogic( IceBreath )
	local tileX, tileY = IceBreath:GetTile()

	if ( tileX == IceBreathTable.lastTileX and
			 tileY == IceBreathTable.lastTileY ) then
		Game.DestroyGameObject( IceBreath )

	else
    local tileType = Game.GetTileInfo( GetTileObjectFaces( IceBreath ) )
    IceBreath:SetNoClip( tileType == "water" )
		IceBreathTable.lastTileX = tileX
		IceBreathTable.lastTileY = tileY
		IceBreath:Move()
	end
end

function IceBreathTable.HandleCollision( IceBreath, Other )
	if ( Other:GetType() == "Slime" ) then
    IceBlock = Game.CreateGameObject(
      "Kickle_Pack/Objects/IceBlock.xml",
      Other:GetTile()
    );
    IceBlock:GetTable().slimeSpawnX = Other:GetTable().spawnPointX
    IceBlock:GetTable().slimeSpawnY = Other:GetTable().spawnPointY
    Game.DestroyGameObject( Other )
    Game.DestroyGameObject( IceBreath )
	end
end

return IceBreathTable
