--Power Rock behavior table
local PowerRock = {}
PowerRock.kicked = false

function PowerRock.AI( self )
  if PowerRock.kicked then
    for k,v in ipairs( State.GetObjects( "Slime" )) do
      IceBlock = State.CreateObject(
                   "Kickle_Pack/Objects/IceBlock.xml",
                   v:GetTile()
                   );
      IceBlock:GetTable().slimeSpawnX = v:GetTable().spawnPointX
      IceBlock:GetTable().slimeSpawnY = v:GetTable().spawnPointY
      State.DestroyObject( v )
    end
    State.DestroyObject( self )
  end
end

return PowerRock
