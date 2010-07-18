--StageManager Behavior Table
local StageManager = {}

function StageManager.AI( self )
  if( not State.GetObject( "DreamBag" )) then
    State.Portal( "nextStage" )
  end
end

function StageManager.Reset( self )
  State.Reset()
end

return StageManager
