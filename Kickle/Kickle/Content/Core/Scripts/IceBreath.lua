-- IceBreath Behavior Table

local IceBreathTable = {}

IceBreathTable.lastTileX = -1
IceBreathTable.lastTileY = -1

function IceBreathTable.AILogic( IceBreath )
	local tileX = IceBreath:GetTileX()
	local tileY = IceBreath:GetTileY()

	if ( tileX == IceBreathTable.lastTileX and
			 tileY == IceBreathTable.lastTileY ) then
		Level.DestroyGameObject( IceBreath )
	else
		IceBreathTable.lastTileX = tileX
		IceBreathTable.lastTileY = tileY
		IceBreath:Move()
	end
end

function IceBreathTable.HandleCollision( IceBreath, Other )
	if ( Other:GetType() == "Slime" ) then
    IceBlock = Level.CreateGameObject(
      "Content/Core/Objects/IceBlock.xml",
       Other:GetTileX(),
       Other:GetTileY()
    );
    IceBlock:GetTable().slimeSpawnX = Other:GetTable().spawnPointX
    IceBlock:GetTable().slimeSpawnY = Other:GetTable().spawnPointY
    Level.DestroyGameObject( Other )
    Level.DestroyGameObject( IceBreath )
	end
end

return IceBreathTable
