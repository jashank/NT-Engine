-- Base class for everything in Kickle Cubicle.
-- Classes that sub are responsible for fulfilling the conditions they
-- say they fill.
local Entity = {}

Entity.isEnemy = false
Entity.isFreezable = false

-- Whether this entity is an enemy in the game.
function Entity.IsEnemy()
  return Entity.isEnemy
end


-- Whether entity is freezable. This means that the entity should
-- have functions Freeze and IsFrozen.
function Entity.IsFreezable()
  return Entity.isFreezable
end

return Entity

