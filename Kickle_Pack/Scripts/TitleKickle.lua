package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local TitleKickle = require("Kickle"):New()


function TitleKickle:Init( kickle )
  kickle:SetDir( Util.DOWN )
  Music.Play( "MenuTheme" )
end

-- temporary!!
function TitleKickle:Play( kickle )
  State.LoadPath( "Kickle_Pack/States/GardenLand_A.xml" )
end

return TitleKickle

