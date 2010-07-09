--Title Play Button Behavior Table

local PlayTable = {}

function PlayTable.MousePressed()
  Game.NewState( "Kickle_Pack/States/GardenLand_A.xml" );
end

function PlayTable.MouseReleased() end
function PlayTable.MouseClicked() end

return PlayTable
