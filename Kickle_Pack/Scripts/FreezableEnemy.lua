package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

-- A Freezable enemy is an enemy that is frozen in the sense that it becomes
-- frozen solid for a bit, but this effect wears off. I.e. this does not
-- apply to Slimes who turn into IceBlocks.
local FreezableEnemy = {}
setmetatable( FreezableEnemy, {__index = require("Enemy")})


FreezableEnemy.isFreezable = true
FreezableEnemy.frozen = true

local EnemyAI = FreezableEnemy.AI
-- Same as EnemyAI but if the enemy is frozen it needs to set a timer until
-- it can be unfrozen.
function FreezableEnemy.AI( self )
  if not FreezableEnemy.frozen then
    EnemyAI( self )
  else 
    local timeFrozen = self:GetElapsedTime()
    if ( timeFrozen >= 5 ) then
      FreezableEnemy.frozen = false
      self:BlockTileRange( false )
    end
  end
end


function FreezableEnemy.Freeze( self )
  FreezableEnemy.frozen = true
  self:BlockTileRange( true )
  self:ResetTimer()
end


function FreezableEnemy.IsFrozen()
  return FreezableEnemy.frozen
end

return FreezableEnemy

