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
  self:SetDir( Util.RIGHT )
  Util.SetAndPlay( self, Util.RIGHT )
  self:Move()
end


function IntroSlime.AI( self )
  if not IntroSlime.sitting then
    local rightX = 9

    local x, y = self:GetTile()
    if ( x == rightX or State.GetObjectOnTile( x + 1, y )) then
      self:SetDir( Util.DOWN )
      Util.SetAndPlay( self, Util.DOWN )
      IntroSlime.sitting = true
    else
      self:Move()
    end
  end
end



return IntroSlime
