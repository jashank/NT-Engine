package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local StageManager = require("Entity"):New()


function StageManager:AI( mngr )
  if( not Map.GetObject( "DreamBag" )) then
    State.Portal( "nextStage" )
  end
end


function StageManager:Reset( mngr )
  State.Reset()
end

return StageManager

