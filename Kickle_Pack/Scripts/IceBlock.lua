package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IceBlock = require("Entity")

IceBlock.isFreezable = true
IceBlock.kicked = false
IceBlock.moving = false
IceBlock.destroyed = false 
IceBlock.slimeSpawn = nil


function IceBlock.Init( self )
  IceBlock.slimeSpawn = State.GetNearestToObject( "SpawnPoint", self )
  self:ResetTimer()
end


function IceBlock.AI( self )
  local timeFrozen = self:GetElapsedTime()
  if ( timeFrozen >= 45 ) then
    IceBlock.destroyed = true
  end

  local facingX, facingY = Util.GetTileObjectFaces( self )
  local tileType = State.GetTileInfo( facingX, facingY )
  local obj = State.GetObjectOnTile( facingX, facingY )
  local objType = ""
  if obj then
    objType = obj:GetType()
  end 

  -- IceBlock just kicked
  if IceBlock.kicked then
    IceBlock.kicked = false

    local facingSpring = false
    if objType == "Spring" then
      facingSpring = ( self:GetDir() == Util.GetOppositeDir( obj:GetDir()))
    end      
    
    if ( State.ObjectBlockingTile( facingX, facingY ) and not facingSpring ) or 
       ( not State.TileIsCrossable( facingX, facingY ) and
         tileType ~= "water" ) then 
      IceBlock.destroyed = true
    else
      IceBlock.moving = true
    end
  end

  -- IceBlock in motion
  if IceBlock.moving then
    if tileType == "water" or tileType == "" then
      IceBlock.destroyed = true
      if tileType == "water" then
        local tileType, tileName =
          State.GetTileInfo( self:GetTile() )
          State.SetTile( facingX, facingY, tileName, 0 )
      end
    else
      if objType == "Spring" then
        self:SetNoClip( true ) 
      end
    end
    IceBlock.moving = self:Move()
  end

  -- IceBlock destroyed
  if IceBlock.destroyed then
    spawn = IceBlock.slimeSpawn
    spawn:GetTable().Spawn( spawn )
    State.DestroyObject( self )
  end
end


function IceBlock.HandleCollision( self, other )
  if other:GetTable().IsEnemy() then
    State.DestroyObject( other )
  end

  local otherType = other:GetType()
  if otherType == "Slime" then
    spawn = other:GetTable().spawn
    spawn:GetTable().Spawn( spawn )
  elseif otherType == "Spring" then
    self:SetNoClip( false )
    other:GetTable().SpringBlock( self )
  elseif otherType == "DreamBag" then
    other:GetTable.Push( other )
  elseif otherType == "Kickle" then
    other:GetTable().Kill( other )
  end
end


function IceBlock.Kick( self, dir )
  IceBlock.kicked = true
  self:SetDir( dir )
end 


function IceBlock.Freeze( self )
  self:ResetTimer() -- Refreeze IceBlock
end

return IceBlock

