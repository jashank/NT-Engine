package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroBlock = require("Entity"):New()

function IntroBlock:Init( block )
  block:SetDir( Util.DOWN )
end


function IntroBlock:AI( block )
  block:Move()
end


function IntroBlock:HandleCollision( block, other )
  Map.DestroyObject( other )
end

return IntroBlock

