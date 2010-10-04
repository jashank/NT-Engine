package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

-- IceBreath Behavior Table

local IceBreath = {}

function IceBreath.AI( self )   
  local facingTileX, facingTileY = Util.GetTileObjectFaces( self )
  local tileType = State.GetTileInfo( facingTileX, facingTileY )
  local otherObj = State.GetObjectOnTile( facingTileX, facingTileY )

  self:SetNoClip( tileType == "water" or
    ( otherObj and ( otherObj:GetType() == "IceBlock" or
      otherObj:GetType() == "Penguin" )))

  if not self:Move() then
    State.DestroyObject( self )
  end

  self:PlayAnim()
end

function IceBreath.HandleCollision( self, other )
  local otherType = other:GetType()
        if ( otherType == "Slime" ) then
    IceBlock = State.CreateObject(
                 "Kickle_Pack/Objects/IceBlock.xml",
                 other:GetTile()
                 );
    IceBlock:GetTable().slimeSpawnX = other:GetTable().spawnPointX
    IceBlock:GetTable().slimeSpawnY = other:GetTable().spawnPointY
    State.DestroyObject( other )
    State.DestroyObject( self )
        end
end

return IceBreath
