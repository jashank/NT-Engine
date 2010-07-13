--StageManager Behavior Table
local StageManager = {}

function StageManager.AI( self )
  if( not Game.GetGameObject( "DreamBag" )) then
    Game.Portal( "nextStage" )
  end
end

function StageManager.Reset( self )
  Game.ResetState()
end

return StageManager
