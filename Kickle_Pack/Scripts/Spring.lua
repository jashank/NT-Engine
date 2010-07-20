--Spring Behavior Table
local Spring = {}

function Spring.HandleCollision( self, other )
  otherType = other:GetType()
  if otherType == "IceBlock" then
    if not self:IsAnimating() then
      self:PlayAnimation( self:GetAnimation() )
    end
  end
end

return Spring
