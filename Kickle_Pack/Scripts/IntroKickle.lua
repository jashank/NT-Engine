package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroKickle = {}
setmetatable( IntroKickle, {__index = require("Entity")})

IntroKickle.setTimer = false

function IntroKickle.Init( self )
  self:SetDir( Util.DOWN )
  Util.SetAndPlay( self, 5 )
  self:Move()
end


-- Walk down until at middle of screen. Wait a couple seconds, throw arms
-- in air, state change.
function IntroKickle.AI( self )
  x, y = self:GetTile()
  if y ~= 8 then
    self:Move()
    return
  end

  if not IntroKickle.setTimer then
    self:ResetTimer()
    Util.SetAndPlay( self, 13 )
    IntroKickle.setTimer = true
  else
    if self:GetElapsedTime() > 0.65 then
      State.LoadPath( "Kickle_Pack/States/MainMenu.xml" )
    end
  end
end

return IntroKickle

