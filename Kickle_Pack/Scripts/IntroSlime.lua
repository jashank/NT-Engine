package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroSlime = require("Entity"):New()
IntroSlime.sitting = false

function IntroSlime:Init( slime )
  objs = Map.GetObjects("IntroSlime")
  slime:SetDir( Util.RIGHT )
  Util.SetAndPlay( slime, Util.RIGHT )
  slime:Move()
end


function IntroSlime:AI( slime )
  if not self.sitting then
    local rightX = 9

    local x, y = slime:GetTile()
    local obj = Map.GetObjectOnTile( x + 1, y )
    if ( x == rightX or ( obj ~= slime and obj ~= nil )) then
      slime:SetDir( Util.DOWN )
      Util.SetAndPlay( slime, Util.DOWN )
      self.sitting = true
    else
      slime:Move()
    end
  end
end


function IntroSlime:IsSitting()
  return self.sitting
end

return IntroSlime

