package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlock = {}

IceBlock.moving = false
IceBlock.created = false
IceBlock.slimeSpawnX = -1
IceBlock.slimeSpawnY = -1

function IceBlock.AI( self )
  if ( IceBlock.created == false ) then
    self:ResetTimer()
    IceBlock.created = true
  else 
    local timeFrozen = self:GetElapsedTime()
    if ( timeFrozen >= 45 ) then
      Game.DestroyGameObject( self )
      Game.CreateGameObject(
        "Kickle_Pack/Objects/Slime.xml",
        IceBlock.slimeSpawnX,
        IceBlock.slimeSpawnY
      ) 
    end
  end

	if ( IceBlock.moving ) then
		IceBlock.moving = self:Move()

		local facingX, facingY = Util.GetTileObjectFaces( self )
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

    elseif ( tileType == "" ) then -- edge of map 
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
  otherType = other:GetType()
	if ( otherType == "IceBreath" ) then
		Game.DestroyGameObject( other )
    self:ResetTimer() -- Refreezes IceBlock

  elseif ( otherType == "Slime" ) then
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
  elseif ( otherType == "Penguin" ) then
    Game.DestroyGameObject( other )
	end
end

return IceBlock
