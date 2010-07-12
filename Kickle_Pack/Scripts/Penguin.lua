--Penguin Behavior Table
package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("GameObjectUtilities")

local Penguin = {}

function Penguin.AI( self )
  Util.GenericEnemyAI( self )
end

return Penguin
