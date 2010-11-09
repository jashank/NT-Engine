package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require("ObjectUtilities")

local TitleKickle = {}
setmetatable( TitleKickle, {__index = require("Kickle")})


function TitleKickle.Init( self )
  self:SetDir( Util.DOWN )
end

-- temporary!!
function TitleKickle.Play( self )
  State.LoadPath( "Kickle_Pack/States/GardenLand_A.xml" )
end

return TitleKickle

