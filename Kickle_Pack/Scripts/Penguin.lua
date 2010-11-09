package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local Penguin = {}
setmetatable( Penguin, {__index = require("FreezableEnemy")})

return Penguin

