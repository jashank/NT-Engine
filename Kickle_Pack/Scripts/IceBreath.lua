package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

-- IceBreath Behavior Table

local IceBreath = {}

IceBreath.lastTileX = -1
IceBreath.lastTileY = -1

function IceBreath.AILogic( self )
	local tileX, tileY = self:GetTile()

	if ( tileX == IceBreath.lastTileX and
			 tileY == IceBreath.lastTileY ) then
		Game.DestroyGameObject( self )

	else
    local tileType = Game.GetTileInfo( GetTileObjectFaces( self ) )
    self:SetNoClip( tileType == "water" )
		IceBreath.lastTileX = tileX
		IceBreath.lastTileY = tileY
		self:Move()
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
