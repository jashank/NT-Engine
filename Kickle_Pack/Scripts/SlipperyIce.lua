package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local SlipperyIce = require("Entity")

function SlipperyIce.HandleCollision( self, other )
  otherType = other:GetType()
  if otherType == "Pillar" then
    State.DestroyObject( other )
  end
end

return SlipperyIce

