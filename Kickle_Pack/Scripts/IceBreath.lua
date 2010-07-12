package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

-- IceBreath Behavior Table

local IceBreath = {}

function IceBreath.AILogic( self )	
  local facingTileX, facingTileY = GetTileObjectFaces( self )
  local tileType = Game.GetTileInfo( facingTileX, facingTileY )
  local otherObj = Game.GetGameObjectOnTile( facingTileX, facingTileY )

  self:SetNoClip( tileType == "water" or
    ( otherObj and otherObj:GetType() == "IceBlock" ))

  if not self:Move() then
    Game.DestroyGameObject( self )
  end
end

function IceBreath.HandleCollision( self, other )
  otherType = other:GetType()
	if ( otherType == "Slime" ) then
    IceBlock = Game.CreateGameObject(
      "Kickle_Pack/Objects/IceBlock.xml",
      other:GetTile()
    );
    IceBlock:GetTable().slimeSpawnX = other:GetTable().spawnPointX
    IceBlock:GetTable().slimeSpawnY = other:GetTable().spawnPointY
    Game.DestroyGameObject( other )
    Game.DestroyGameObject( self )
	end
end

return IceBreath
