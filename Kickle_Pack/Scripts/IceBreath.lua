package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local IceBreath = require("Entity"):New()
IceBreath.tilesCrossed = 0

function IceBreath:AI( icebreath )
  if ( self.tilesCrossed >= 6 ) then
    Map.DestroyObject( icebreath )
    return
  else
    self.tilesCrossed = self.tilesCrossed + 1
  end

  local facingTileX, facingTileY = Util.GetTileObjectFaces( icebreath )
  local tileType = Map.GetTileInfo( facingTileX, facingTileY )
  local otherObj = Map.GetObjectOnTile( facingTileX, facingTileY )

  if not icebreath:Move() then
    Map.DestroyObject( icebreath )
  end

  icebreath:PlayAnim()
end


function IceBreath:HandleCollision( icebreath, other )
  if other:GetTable():IsFreezable() then
    other:GetTable():Freeze( other )
    Map.DestroyObject( icebreath )
  end
end

return IceBreath

