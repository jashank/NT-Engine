package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")
EnemyLib = require("EnemyLib")

local Chicken = {}
Chicken.frozen = false

-- Kicks any blocks around him because he is a jerk.
function Chicken.AI( self )
  for i = Util.UP, Util.RIGHT do
    local x, y = Util.GetTileInDirection( self, i )
    print(x .. " " .. y)
    block = State.GetObjectOnTile( x, y, "IceBlock" )
    if block and not block:GetTable().kicked then
      self:SetDir( i )
      block:GetTable().Kick( block, i )
      return
    end
  end

  EnemyLib.FrozenAI( self, 5 )
  self:SetAnim( 0 ) -- temp until anims in
end


function Chicken.HandleCollision( self, other )
  if not EnemyLib.FrozenCollision( self, other ) then
    EnemyLib.GenericCollision( self, other )
  end
  self:SetAnim( 0 ) -- temp until anims in
end

return Chicken
