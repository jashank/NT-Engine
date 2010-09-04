--Penguin Behavior Table
package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

local Penguin = {}

Penguin.frozen = false

function Penguin.AI( self )
  if not Penguin.frozen then
    Util.GenericEnemyAI( self )
  else 
    local timeFrozen = self:GetElapsedTime()
    if ( timeFrozen >= 5 ) then
      Penguin.frozen = false
      self:BlockTile( false )
    end
  end
end

function Penguin.HandleCollision( self, other )
  local otherType = other:GetType()
  if otherType == "IceBreath" then
    Penguin.frozen = true
    self:BlockTile( true )
    -- Set animation to frozen
    self:ResetTimer()
    State.DestroyObject( other )
  
  elseif otherType == "Slime" then
    local dir = self:SetDir( Util.GetOppositeDir( self:GetDir()))
    Util.SetAndPlay( self, dir )
  end
end

return Penguin
