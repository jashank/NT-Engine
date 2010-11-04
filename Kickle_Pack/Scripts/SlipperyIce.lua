local SlipperyIce = {}

function SlipperyIce.HandleCollision( self, other )
  otherType = other:GetType()
  if otherType == "Pillar" then
    print "happened"
    State.DestroyObject( other )
  end
end


return SlipperyIce

