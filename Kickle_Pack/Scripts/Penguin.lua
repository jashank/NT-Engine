package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require("ObjectUtilities")
EnemyLib = require("EnemyLib")

local Penguin = {}

Penguin.frozen = false

function Penguin.AI( self )
  EnemyLib.FrozenAI( self, 5 )
end

function Penguin.HandleCollision( self, other )
  if not EnemyLib.FrozenCollision( self, other ) then
    EnemyLib.GenericCollision( self, other )
  end
end

return Penguin
