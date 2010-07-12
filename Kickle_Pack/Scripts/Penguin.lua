--Penguin Behavior Table
package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

local Penguin = {}

function Penguin.AILogic( self )
  GenericEnemyAI( self )
end

return Penguin
