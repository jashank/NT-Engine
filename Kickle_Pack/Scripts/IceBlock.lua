package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

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
		local facingX, facingY = Util.GetTileObjectFaces( self )
    local tileType = State.GetTileInfo( facingX, facingY )

		if tileType == "water" or tileType == "" then
      IceBlock.destroyed = true
      if tileType == "water" then
        local tileType, tileName =
          State.GetTileInfo( self:GetTile() )
        State.SetTile( facingX, facingY, tileName, 0 )
      end
    else
      obj = State.GetObjectOnTile( facingX, facingY )
      if obj then
        self:SetNoClip( obj:GetType() == "Spring" )
      end
    end
    IceBlock.moving = self:Move()
  end

  if IceBlock.destroyed then
    State.DestroyObject( self )
    if IceBlock.slimeSpawnX == -1 and IceBlock.slimeSpawnY == -1 then
      local stateName = State.GetName()
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
    State.CreateObject(
      "Kickle_Pack/Objects/Slime.xml",
      IceBlock.slimeSpawnX,
      IceBlock.slimeSpawnY
    )

  end
end


function IceBlock.HandleCollision( self, other )
  otherType = other:GetType()
	if otherType == "IceBreath" then
		State.DestroyObject( other )
    self:ResetTimer() -- Refreezes IceBlock

  elseif otherType == "Slime" then
    local slimeSpawnX = other:GetTable().spawnPointX;
    local slimeSpawnY = other:GetTable().spawnPointY;
    State.CreateObject( 
      "Kickle_Pack/Objects/Slime.xml",
      slimeSpawnX,
      slimeSpawnY
    )

  elseif otherType == "Penguin" then
    State.DestroyObject( other )
  
  elseif otherType == "Spring" then
    if other:GetFrame() <= 4 then
      self:SlowDown( 1 )
    elseif self:GetSpeed() == 0 and other:GetFrame() >= 5 then
      self:Reverse()
      self:SetSpeed( 4 )
    end
  end
end

return IceBlock
