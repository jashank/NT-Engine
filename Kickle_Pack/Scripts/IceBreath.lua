package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

-- IceBreath Behavior Table

local IceBreath = {}
IceBreath.tilesCrossed = 0

function IceBreath.AI( self )
  if ( IceBreath.tilesCrossed >= 6 ) then
    State.DestroyObject( self )
    return
  else
    IceBreath.tilesCrossed = IceBreath.tilesCrossed + 1
  end

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


return IceBreath
