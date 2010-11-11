package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

-- A Freezable enemy is an enemy that is frozen in the sense that it becomes
-- frozen solid for a bit, but this effect wears off. I.e. this does not
-- apply to Slimes who turn into IceBlocks.
local FreezableEnemy = require("Enemy"):New{ isFreezable = true }
FreezableEnemy.frozen = false 

-- Same as EnemyAI but if the enemy is frozen it needs to set a timer until
-- it can be unfrozen.
local EnemyAI = FreezableEnemy.AI
function FreezableEnemy:AI( fenemy )
  if not self.frozen then
    EnemyAI( self, fenemy )
  else 
    local timeFrozen = fenemy:GetElapsedTime()
    if ( timeFrozen >= 5 ) then
      self.frozen = false
      fenemy:BlockTileRange( false )
    end
  end
end


function FreezableEnemy:Freeze( fenemy )
  self.frozen = true
  fenemy:BlockTileRange( true )
  fenemy:ResetTimer()
end


function FreezableEnemy:IsFrozen()
  return self.frozen
end

return FreezableEnemy

