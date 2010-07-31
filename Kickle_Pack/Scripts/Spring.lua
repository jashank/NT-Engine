package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require "ObjectUtilities"

--Spring Behavior Table
local Spring = {}

function Spring.Init( self )
  local dir = self:GetAnimation()
  self:SetDir( Util.GetOppositeDir( dir ))
end

function Spring.HandleCollision( self, other )
  local otherType = other:GetType()
  if otherType == "IceBlock" then
    if not self:IsAnimating() then
      self:PlayAnimation( self:GetAnimation() )
    end
  end
end

return Spring
