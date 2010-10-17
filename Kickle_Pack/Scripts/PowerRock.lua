--Power Rock behavior table
local PowerRock = {}
PowerRock.kicked = false

function PowerRock.AI( self )
  if PowerRock.kicked then
    slimes = State.GetObjects( "Slime" )
    for k,v in ipairs( slimes ) do
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
