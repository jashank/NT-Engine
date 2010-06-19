package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--DreamBag Behavior Table

local DreamBagTable = {}

function DreamBagTable.HandleCollision( DreamBag, Other )
  if ( Other:GetType() == "Slime" or Other:GetType() == "Penguin" ) then
    local dir = math.random( UP, RIGHT )

    if ( not Game.TileIsCrossable( GetTileInDirection( DreamBag, dir ))) then
      local newDir = GetNextDir( dir )
      while newDir ~= dir do
        if ( not Game.TileIsCrossable( GetTileInDirection( DreamBag, newDir ))) then
          newDir = GetNextDir( newDir )
        else
          dir = newDir
        end
      end
    end

    DreamBag:SetDir( dir )
    DreamBag:Move()
    DreamBagTable.isMoving = true

  elseif ( Other:GetType() == "Kickle" ) then
    Game.DestroyGameObject( DreamBag )
  end
end

return DreamBagTable
