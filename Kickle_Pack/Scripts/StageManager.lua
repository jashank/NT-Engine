package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local StageManager = {}
setmetatable( StageManager, {__index = require("Entity")})

function StageManager.AI( self )
  if( not State.GetObject( "DreamBag" )) then
    State.Portal( "nextStage" )
  end
end

function StageManager.Reset( self )
  State.Reset()
end

return StageManager
