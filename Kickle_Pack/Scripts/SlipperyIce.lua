package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local SlipperyIce = require("Entity"):New()

function SlipperyIce:HandleCollision( ice, other )
  otherType = other:GetType()
  if otherType == "Pillar" then
    Map.DestroyObject( other )
  end
end

return SlipperyIce

