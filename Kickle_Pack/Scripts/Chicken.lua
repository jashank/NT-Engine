package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

local Chicken = require("FreezableEnemy"):New()

-- Kicks any blocks around him because he is a jerk.
function Chicken:AI( chicken )
  for i = Util.UP, Util.RIGHT do
    local x, y = Util.GetTileInDirection( chicken, i )
    block = State.GetObjectOnTile( x, y, "IceBlock" )
    if block and not block:GetTable():IsKicked() then
      chicken:SetDir( i )
      block:GetTable():Kick( block, i )
      return
    end
  end

  self.__index:AI( chicken )
  chicken:SetAnim( 0 ) -- temp until anims in
end


-- temporary until chicken animations in
function Chicken:HandleCollision( chicken, other )
  self.__index:HandleCollision( chicken, other )
  chicken:SetAnim( 0 ) -- temp until anims in
end

return Chicken

