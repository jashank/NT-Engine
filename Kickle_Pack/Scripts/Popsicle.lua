--Popsicle Behavior Table

local Popsicle = {}

function Popsicle.HandleCollision( self, other )
  if ( other:GetType() == "Kickle" ) then
    Game.DestroyGameObject( self )
  end
end

return Popsicle
