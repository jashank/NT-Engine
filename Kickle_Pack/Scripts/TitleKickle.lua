package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local TitleKickle = require( "Kickle" )

function TitleKickle.Init( self )
  State.SpanCam( 15, 15 )
  State.OffsetCam( 0, 3 )
  self:SetDir( Util.DOWN )
end

-- temporary!!
function TitleKickle.Play( self )
  State.LoadPath( "Kickle_Pack/States/GardenLand_A.xml" )
end

return TitleKickle

