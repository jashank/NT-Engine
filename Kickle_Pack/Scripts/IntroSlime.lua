package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroSlime = require("Entity"):New()
IntroSlime.sitting = false

function IntroSlime.Init( self )
  objs = State.GetObjects("IntroSlime")
  self:SetDir( Util.RIGHT )
  Util.SetAndPlay( self, Util.RIGHT )
  self:Move()
end


function IntroSlime.AI( self )
  if not IntroSlime.sitting then
    local rightX = 9

    local x, y = self:GetTile()
    obj = State.GetObjectOnTile( x + 1, y )
    if ( x == rightX or ( obj ~= self and obj ~= nil )) then
      self:SetDir( Util.DOWN )
      Util.SetAndPlay( self, Util.DOWN )
      IntroSlime.sitting = true
    else
      self:Move()
    end
  end
end

return IntroSlime

