package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local IceBreath = {}
setmetatable( IceBreath, {__index = require("Entity")})

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

  if tileType == "water" or
     ( otherObj and otherObj:GetTable().IsFreezable() ) then
    self:SetNoClip( true )
  else
    self:SetNoClip( false )
  end

  if not self:Move() then
    State.DestroyObject( self )
  end

  self:PlayAnim()
end


function IceBreath.HandleCollision( self, other )
  if other:GetTable().IsFreezable() then
    other:GetTable().Freeze( other )
    State.DestroyObject( self )
  end
end

return IceBreath

