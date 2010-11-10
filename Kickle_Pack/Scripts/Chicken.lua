package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Chicken = require("FreezableEnemy"):New()

local FreezableEnemyAI = Chicken.AI
-- Kicks any blocks around him because he is a jerk.
function Chicken.AI( self )
  for i = Util.UP, Util.RIGHT do
    local x, y = Util.GetTileInDirection( self, i )
    block = State.GetObjectOnTile( x, y, "IceBlock" )
    if block and not block:GetTable():IsKicked() then
      self:SetDir( i )
      block:GetTable():Kick( block, i )
      return
    end
  end

  FreezableEnemyAI( self )
  self:SetAnim( 0 ) -- temp until anims in
end


local FreezableEnemyCollision = Chicken.HandleCollision
-- temporary until chicken animations in
function Chicken.HandleCollision( self, other )
  FreezableEnemyCollision( self, other )
  self:SetAnim( 0 ) -- temp until anims in
end

return Chicken

