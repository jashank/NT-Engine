package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require "ObjectUtilities"

--Spring Behavior Table
local Spring = {}

function Spring.Init( self )
  local dir = self:GetAnim()
  self:SetDir( Util.GetOppositeDir( dir ))
end

function Spring.HandleCollision( self, other )
  self:StopAnim()
  local otherType = other:GetType()
  if otherType == "IceBlock" then
    self:PlayAnim()
  end
end

return Spring
