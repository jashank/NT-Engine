package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlock = {}

IceBlock.moving = false
IceBlock.startingX = -1
IceBlock.startingY = -1
IceBlock.destroyed = false 
IceBlock.slimeSpawnX = -1
IceBlock.slimeSpawnY = -1

function IceBlock.AI( self )
  if IceBlock.startingX == -1 and IceBlock.startingY == -1 then
    self:ResetTimer()
    IceBlock.startingX, IceBlock.startingY = self:GetTile()
  else 
    local timeFrozen = self:GetElapsedTime()
    if ( timeFrozen >= 45 ) then
      IceBlock.destroyed = true
    end
  end

	if IceBlock.moving then
		IceBlock.moving = self:Move()

		local facingX, facingY = Util.GetTileObjectFaces( self )
    local tileType = Game.GetTileInfo( facingX, facingY )

		if tileType == "water" or tileType == "" then
      IceBlock.destroyed = true
      if tileType == "water" then
        local tileType, tileName =
          Game.GetTileInfo( self:GetTile() )
        Game.SetTile( facingX, facingY, tileName, 0 )
      end
    end
	end

  if IceBlock.destroyed then
    Game.DestroyGameObject( self )
    if IceBlock.slimeSpawnX == -1 and IceBlock.slimeSpawnY == -1 then
      local stateName = Game.GetName()
      if stateName == "GardenLand_E" then
        if IceBlock.startingY == 6 then
          IceBlock.slimeSpawnX = 6
          IceBlock.slimeSpawnY = 3
        else
          IceBlock.slimeSpawnX = 8
          IceBlock.slimeSpawnY = 3
        end
      end
    end
    Game.CreateGameObject(
      "Kickle_Pack/Objects/Slime.xml",
      IceBlock.slimeSpawnX,
      IceBlock.slimeSpawnY
    )

  end
end


function IceBlock.HandleCollision( self, other )
  otherType = other:GetType()
	if otherType == "IceBreath" then
		Game.DestroyGameObject( other )
    self:ResetTimer() -- Refreezes IceBlock

  elseif otherType == "Slime" then
    local slimeSpawnX = other:GetTable().spawnPointX;
    local slimeSpawnY = other:GetTable().spawnPointY;
    Game.CreateGameObject( 
      "Kickle_Pack/Objects/Slime.xml",
      slimeSpawnX,
      slimeSpawnY
    )

  elseif otherType == "Penguin" then
    Game.DestroyGameObject( other )
	end
end

return IceBlock
