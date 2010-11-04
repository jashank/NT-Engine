local SlipperyIce = {}

function SlipperyIce.HandleCollision( self, other )
  otherType = other:GetType()
  if otherType == "Pillar" then
    State.DestroyObject( other )
  end
end


return SlipperyIce

