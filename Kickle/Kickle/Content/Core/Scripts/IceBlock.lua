package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlockTable = {}

IceBlockTable.moving = false
IceBlockTable.slimeSpawnX = -1;
IceBlockTable.slimeSpawnY = -1;

function IceBlockTable.AILogic( IceBlock )
	if ( IceBlockTable.moving ) then
		IceBlock:Move()

		local facingX, facingY = GetTileObjectFaces( IceBlock )
    local tileType = Level.GetTile( facingX, facingY )
		if ( tileType == "water" ) then
      local tileType, tileName =
        Level.GetTile( IceBlock:GetTilePos() )
      Level.SetTile( facingX, facingY, tileName, 0 )
			Level.DestroyGameObject( IceBlock )
      Level.CreateGameObject(
        "Content/Core/Objects/Slime.xml",
        IceBlockTable.slimeSpawnX,
        IceBlockTable.slimeSpawnY
      )

    elseif ( tileType == "" ) then
      Level.DestroyGameObject( IceBlock )
      Level.CreateGameObject(
        "Content/Core/Objects/Slime.xml",
        IceBlockTable.slimeSpawnX,
        IceBlockTable.slimeSpawnY
      )
    end
	end
end


function IceBlockTable.HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Level.DestroyGameObject( Other )

  elseif ( Other:GetType() == "Slime" ) then
    local slimeSpawnX = Other:GetTable().spawnPointX;
    local slimeSpawnY = Other:GetTable().spawnPointY;

    if (( slimeSpawnX ~= -1 and slimeSpawnY ~= -1 ) and
        ( slimeSpawnX ~= IceBlockTable.slimeSpawnX or
          slimeSpawnY ~= IceBlockTable.slimeSpawnY )) then
      Level.DestroyGameObject( Other )
      Level.CreateGameObject(
        "Content/Core/Objects/Slime.xml",
        slimeSpawnX,
        slimeSpawnY
      )
    end
	end
end

return IceBlockTable
