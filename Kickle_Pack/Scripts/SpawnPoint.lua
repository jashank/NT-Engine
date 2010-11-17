package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local SpawnPoint = require("Entity"):New()
SpawnPoint.slimeQueue = 0

-- Spawn slime at this spawn point unless already spawning. If so,
-- check to see if there is another spawn point to spawn it at and force
-- it to spawn there. If not, queue it up to spawn here.
function SpawnPoint:Spawn( spawn )
  local x, y = spawn:GetTile()
  slime = Map.GetObjectOnTile( x, y, "Slime" )
  if slime then
    otherPoint = Map.GetNearestToObject( "SpawnPoint", spawn )
    if otherPoint then
      otherPoint:GetTable():MustSpawn( otherPoint )
    else
      self.slimeQueue = self.slimeQueue + 1
    end
  else
    Map.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
  end
end


-- Slime must spawn here, don't try to pass onto another spawn point.
function SpawnPoint:MustSpawn( spawn )
  local x, y = spawn:GetTile()
  slime = Map.GetObjectOnTile( x, y, "Slime" )
  if slime then
    self.slimeQueue = self.slimeQueue + 1
  else
    Map.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
  end
end


function SpawnPoint:AI( spawn )
  if self.slimeQueue > 0 then
    local x, y = spawn:GetTile()
    slime = Map.GetObjectOnTile( x, y, "Slime" )
    if not slime then
      Map.CreateObject( "Kickle_Pack/Objects/Slime.xml", x, y )
    end
  end
end

return SpawnPoint

