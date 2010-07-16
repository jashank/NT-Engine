--One Up Behavior Table
local OneUp = {}

function OneUp.HandleCollision( self, other )
  otherType = other:GetType()
  if ( otherType == "Kickle" ) then
    Game.DestroyGameObject( self )
  end
end

return OneUp
