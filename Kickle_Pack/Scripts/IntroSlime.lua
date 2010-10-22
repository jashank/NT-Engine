package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require("ObjectUtilities")

local IntroSlime = {}
IntroSlime.sitting = false

function IntroSlime.Init( self )
  objs = State.GetObjects("IntroSlime")
  if #objs == 1 then
    IntroSlime.test = "A";
  elseif #objs == 2 then
    IntroSlime.test = "B";
  else IntroSlime.test = "C"; end
  self:SetDir( Util.LEFT )
  Util.SetAndPlay( self, Util.LEFT )
  self:Move()
end


function IntroSlime.AI( self )
  if not IntroSlime.sitting then
    local rightX = 7

    local x, y = self:GetTile()
    print (IntroSlime.test .. " " .. x .. " " .. y .. "\n");
    if ( x == rightX or State.GetObjectOnTile( x - 1, y )) then
      self:SetDir( Util.DOWN )
      Util.SetAndPlay( self, Util.DOWN )
      IntroSlime.sitting = true
    else
      self:Move()
    end
  end
end

--[[
function IntroSlime.AI( self )
  if not IntroSlime.sitting then
    local rightX = 9
    local midX = 8
    local leftX = 7

    local x, y = self:GetTile()
    local leftSlime = State.GetObjectOnTile( x - 2, y )
    local rightSlime = State.GetObjectOnTile( x + 1, y )

    if (( x == rightX ) or
        ( x == midX and leftSlime and rightSlime ) or
        ( x == leftX and rightSlime )) then
        self:SetDir( Util.DOWN )
        Util.SetAndPlay( self, Util.DOWN )
        IntroSlime.sitting = true
    else
      self:Move()
    end
  end
end
--]]


return IntroSlime
