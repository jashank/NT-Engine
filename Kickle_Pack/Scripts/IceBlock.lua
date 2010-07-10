package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlock = {}

IceBlock.moving = false
IceBlock.slimeSpawnX = -1;
IceBlock.slimeSpawnY = -1;

function IceBlock.AILogic( self )
	if ( IceBlock.moving ) then
		self:Move()

		local facingX, facingY = GetTileObjectFaces( self )
    local tileType = Game.GetTileInfo( facingX, facingY )
		if ( tileType == "water" ) then
      local tileType, tileName =
        Game.GetTileInfo( self:GetTile() )
      Game.SetTile( facingX, facingY, tileName, 0 )
			Game.DestroyGameObject( self )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        IceBlock.slimeSpawnX,
        IceBlock.slimeSpawnY
      )

    elseif ( tileType == "" ) then
      Game.DestroyGameObject( self )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        IceBlock.slimeSpawnX,
        IceBlock.slimeSpawnY
      )
    end
	end
end


function IceBlock.HandleCollision( self, other )
	if ( other:GetType() == "IceBreath" ) then
		Game.DestroyGameObject( other )

  elseif ( other:GetType() == "Slime" ) then
    local slimeSpawnX = other:GetTable().spawnPointX;
    local slimeSpawnY = other:GetTable().spawnPointY;

    if (( slimeSpawnX ~= -1 and slimeSpawnY ~= -1 ) and
        ( slimeSpawnX ~= IceBlock.slimeSpawnX or
          slimeSpawnY ~= IceBlock.slimeSpawnY )) then
      Game.DestroyGameObject( other )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        slimeSpawnX,
        slimeSpawnY
      )
    end
	end
end

return IceBlock
