package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IceBlock = require("Entity"):New{ isFreezable = true }
IceBlock.kicked = false
IceBlock.moving = false
IceBlock.destroyed = false 
IceBlock.slimeSpawn = nil


function IceBlock:Init( iceblock )
  self.slimeSpawn = State.GetNearestToObject( "SpawnPoint", iceblock )
  iceblock:ResetTimer()
end


function IceBlock:AI( iceblock )
  local timeFrozen = iceblock:GetElapsedTime()
  if ( timeFrozen >= 45 ) then
    self.destroyed = true
  end

  local facingX, facingY = Util.GetTileObjectFaces( iceblock )
  local tileType = State.GetTileInfo( facingX, facingY )
  local obj = State.GetObjectOnTile( facingX, facingY )
  local objType = ""
  if obj then
    objType = obj:GetType()
  end 

  -- IceBlock just kicked
  if self.kicked then
    self.kicked = false

    local facingSpring = false
    if objType == "Spring" then
      facingSpring = 
        ( iceblock:GetDir() == Util.GetOppositeDir( obj:GetDir()))
    end      
    
    if ( State.ObjectBlockingTile( facingX, facingY ) and not facingSpring ) or 
       ( not State.TileIsCrossable( facingX, facingY ) and
         tileType ~= "water" ) then 
      self.destroyed = true
    else
      self.moving = true
    end
  end

  -- IceBlock in motion
  if self.moving then
    if tileType == "water" or tileType == "" then
      self.destroyed = true
      if tileType == "water" then
        local tileType, tileName =
          State.GetTileInfo( iceblock:GetTile() )
          State.SetTile( facingX, facingY, tileName, 0 )
      end
    else
      if objType == "Spring" then
        iceblock:SetNoClip( true ) 
      end
    end
    self.moving = iceblock:Move()
  end

  -- IceBlock destroyed
  if self.destroyed then
    spawn = self.slimeSpawn
    spawn:GetTable():Spawn( spawn )
    State.DestroyObject( iceblock )
  end
end


function IceBlock:HandleCollision( iceblock, other )
  if other:GetTable():IsEnemy() then
    State.DestroyObject( other )
  end

  local otherType = other:GetType()
  if otherType == "Slime" then
    spawn = other:GetTable():GetSpawn()
    spawn:GetTable():Spawn( spawn )
  elseif otherType == "Spring" then
    iceblock:SetNoClip( false )
    other:GetTable():SpringBlock( iceblock )
  elseif otherType == "DreamBag" then
    other:GetTable():Push( other )
  elseif otherType == "Kickle" then
    other:GetTable():Kill( other )
  end
end


function IceBlock:Kick( iceblock, dir )
  self.kicked = true
  iceblock:SetDir( dir )
end 


function IceBlock:Freeze( iceblock )
  iceblock:ResetTimer() -- Refreeze IceBlock
end


function IceBlock:IsKicked()
  return self.kicked
end


function IceBlock:SetSlimeSpawn( spawn )
  self.slimeSpawn = spawn
end

return IceBlock

