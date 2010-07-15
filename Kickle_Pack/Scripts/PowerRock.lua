--Power Rock behavior table
local PowerRock = {}

function PowerRock.HandleCollision( self, other )
  otherType = other:GetType()
  if ( otherType == "Kickle" ) then
    for k,v in ipairs( Game.GetGameObjects( "Slime" )) do
      IceBlock = Game.CreateGameObject(
                   "Kickle_Pack/Objects/IceBlock.xml",
                   v:GetTile()
                   );
      IceBlock:GetTable().slimeSpawnX = v:GetTable().spawnPointX
      IceBlock:GetTable().slimeSpawnY = v:GetTable().spawnPointY
      Game.DestroyGameObject( v )
    end
    Game.DestroyGameObject( self )
  end
end

return PowerRock
