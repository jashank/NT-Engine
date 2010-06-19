package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
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
    local tileType = Game.GetTileInfo( facingX, facingY )
		if ( tileType == "water" ) then
      local tileType, tileName =
        Game.GetTileInfo( IceBlock:GetTile() )
      Game.SetTile( facingX, facingY, tileName, 0 )
			Game.DestroyGameObject( IceBlock )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        IceBlockTable.slimeSpawnX,
        IceBlockTable.slimeSpawnY
      )

    elseif ( tileType == "" ) then
      Game.DestroyGameObject( IceBlock )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        IceBlockTable.slimeSpawnX,
        IceBlockTable.slimeSpawnY
      )
    end
	end
end


function IceBlockTable.HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Game.DestroyGameObject( Other )

  elseif ( Other:GetType() == "Slime" ) then
    local slimeSpawnX = Other:GetTable().spawnPointX;
    local slimeSpawnY = Other:GetTable().spawnPointY;

    if (( slimeSpawnX ~= -1 and slimeSpawnY ~= -1 ) and
        ( slimeSpawnX ~= IceBlockTable.slimeSpawnX or
          slimeSpawnY ~= IceBlockTable.slimeSpawnY )) then
      Game.DestroyGameObject( Other )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        slimeSpawnX,
        slimeSpawnY
      )
    end
	end
end

return IceBlockTable
