
-- So objects don't have to put this in their script all the time.
package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

-- Base class for everything in Kickle Cubicle.
-- Classes that sub are responsible for fulfilling the conditions they
-- say they fill.
local Entity = {}
Entity.killsKickle = false
Entity.isEnemy = false


-- Whether this entity will kill Kickle when they collide.
function Entity.KillsKickle()
  return Entity.killsKickle
end


-- Whether this entity is an enemy in the game.
function Entity.IsEnemy()
  return Entity.isEnemy
end


return Entity

