local SpawnPoint = {}
SpawnPoint.slimeQueue = 0


-- Spawn slime at this spawn point unless already spawning. If so,
-- check to see if there is another spawn point to spawn it at and force
-- it to spawn there. If not, queue it up to spawn here.
function SpawnPoint.Spawn( self )
  local x, y = self:GetTile()
  slime = State.GetObjectOnTile( x, y, "Slime" )
  if slime then
    otherPoint = State.GetNearestToObject( "SpawnPoint", self )
    if otherPoint then
      otherPoint:GetTable().MustSpawn( otherPoint )
    else
      SpawnPoint.slimeQueue = SpawnPoint.slimeQueue + 1
    end
  else
    State.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
  end
end


-- Slime must spawn here, don't try to pass onto another spawn point.
function SpawnPoint.MustSpawn( self )
  local x, y = self:GetTile()
  slime = State.GetObjectOnTile( x, y, "Slime" )
  if slime then
    SpawnPoint.slimeQueue = SpawnPoint.slimeQueue + 1
  else
    State.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
  end
end


function SpawnPoint.AI( self )
  if SpawnPoint.slimeQueue > 0 then
    local x, y = self:GetTile()
    slime = State.GetObjectOnTile( x, y, "Slime" )
    if not slime then
      State.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
    end
  end
end

return SpawnPoint

