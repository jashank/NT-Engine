--Title Play Button Behavior Table

local TitlePlay = {}

function TitlePlay.MousePressed()
  Game.NewState( "Kickle_Pack/States/GardenLand_A.xml" );
end

function TitlePlay.MouseReleased() end
function TitlePlay.MouseClicked() end

return TitlePlay
