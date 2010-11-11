package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local IceBreath = require("Entity"):New()
IceBreath.tilesCrossed = 0

function IceBreath:AI( icebreath )
  if ( self.tilesCrossed >= 6 ) then
    State.DestroyObject( icebreath )
    return
  else
    self.tilesCrossed = self.tilesCrossed + 1
  end

  local facingTileX, facingTileY = Util.GetTileObjectFaces( icebreath )
  local tileType = State.GetTileInfo( facingTileX, facingTileY )
  local otherObj = State.GetObjectOnTile( facingTileX, facingTileY )

  if tileType == "water" or
     ( otherObj and otherObj:GetTable():IsFreezable() ) then
    icebreath:SetNoClip( true )
  else
    icebreath:SetNoClip( false )
  end

  if not icebreath:Move() then
    State.DestroyObject( icebreath )
  end

  icebreath:PlayAnim()
end


function IceBreath:HandleCollision( icebreath, other )
  if other:GetTable():IsFreezable() then
    other:GetTable():Freeze( other )
    State.DestroyObject( icebreath )
  end
end

return IceBreath

