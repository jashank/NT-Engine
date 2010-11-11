package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local IntroKickle = require("Entity"):New()
IntroKickle.setTimer = false

function IntroKickle:Init( kickle )
  kickle:SetDir( Util.DOWN )
  Util.SetAndPlay( kickle, 5 )
  kickle:Move()
end


-- Walk down until at middle of screen. Wait a couple seconds, throw arms
-- in air, state change.
function IntroKickle:AI( kickle )
  x, y = kickle:GetTile()
  if y ~= 8 then
    kickle:Move()
    return
  end

  if not self.setTimer then
    kickle:ResetTimer()
    Util.SetAndPlay( kickle, 13 )
    self.setTimer = true
  else
    if kickle:GetElapsedTime() > 0.65 then
      State.LoadPath( "Kickle_Pack/States/MainMenu.xml" )
    end
  end
end

return IntroKickle

