--Popsicle Behavior Table

local PopsicleTable = {}

function PopsicleTable.HandleCollision( Popsicle, Other )
  if ( Other:GetType() == "Kickle" ) then
    Game.DestroyGameObject( Popsicle )
  end
end

return PopsicleTable
