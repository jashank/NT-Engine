--Popsicle Behavior Table

local Popsicle = {}

function Popsicle.HandleCollision( self, other )
  if ( other:GetType() == "Kickle" ) then
    State.DestroyObject( self )
  end
end

return Popsicle
