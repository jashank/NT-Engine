--StageManager Behavior Table
local StageManager = {}

function StageManager.AILogic( self )
  if( not Game.GetGameObject( "DreamBag" )) then
    Game.NewState( "Kickle_Pack/States/GardenLand_B.xml" )
  end
end

function StageManager.Reset( self )
  Game.ResetState()
end

return StageManager
