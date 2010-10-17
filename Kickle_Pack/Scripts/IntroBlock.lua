package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require("ObjectUtilities")

local IntroBlock = {}

function IntroBlock.Init( self )
  self:SetDir( Util.DOWN )
end


function IntroBlock.AI( self )
  self:Move()
end


function IntroBlock.HandleCollision( self, other )
  State.DestroyObject( other )
end

return IntroBlock

