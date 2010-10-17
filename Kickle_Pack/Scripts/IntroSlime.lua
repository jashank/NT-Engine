package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require("ObjectUtilities")

local IntroSlime = {}
IntroSlime.sitting = false
IntroSlime.moving = true


function IntroSlime.Init( self )
  self:SetDir( Util.RIGHT )
  Util.SetAndPlay( self, Util.RIGHT )
  self:Move()
end


function IntroSlime.AI( self )
  if not IntroSlime.sitting then
    local rightX, rightY = 8, 8
    local midX, midY = 7, 8
    local leftX, leftY = 6, 8

    local x, y = self:GetTile()
    local leftSlime = State.GetObjectOnTile( x - 2, y )
    local rightSlime = State.GetObjectOnTile( x + 1, y )

    if (( x == rightX and y == rightY ) or
        ( x == midX and y == midY and leftSlime and rightSlime ) or
        ( x == leftX and y == leftY and rightSlime )) then
        self:SetDir( Util.DOWN )
        Util.SetAndPlay( self, Util.DOWN )
        IntroSlime.sitting = true
    else
      self:Move()
    end
  end
end


return IntroSlime
