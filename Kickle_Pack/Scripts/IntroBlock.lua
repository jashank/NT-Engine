package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroBlock = {}
setmetatable( IntroBlock, {__index = require("Entity")})

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

