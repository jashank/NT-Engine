-- Base class for everything in Kickle Cubicle.
-- Classes that sub are responsible for fulfilling the conditions they
-- say they fill.
local Entity = { isEnemy = false, isFreezable = false }

function Entity:New( o )
  o = o or {} -- create table if none provided
  setmetatable( o, self )
  self.__index = self
  return o
end

-- Whether this entity is an enemy in the game.
function Entity:IsEnemy()
  return self.isEnemy
end


-- Whether entity is freezable. This means that the entity should
-- have functions Freeze and IsFrozen.
function Entity:IsFreezable()
  return self.isFreezable
end

return Entity

