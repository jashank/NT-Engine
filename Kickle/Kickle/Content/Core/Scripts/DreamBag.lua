package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--DreamBag Behavior Table

local DreamBagTable = {}



function DreamBagTable.HandleCollision( DreamBag, Other )
  if ( Other:GetType() == "Slime" or Other:GetType() == "Penguin" ) then
    local dir = math.random( UP, RIGHT )

    if ( Level.TileIsSolid( GetTileInDirection( DreamBag, dir ))) then
      local newDir = GetNextDir( dir )
      while newDir ~= dir do
        if ( Level.TileIsSolid( GetTileInDirection( DreamBag, newDir ))) then
          newDir = GetNextDir( newDir )
        else
          dir = newDir
        end
      end
    end

    DreamBag:SetDir( dir )
    DreamBag:Move()

  elseif ( Other:GetType() == "Kickle" ) then
    Level.DestroyGameObject( DreamBag )
  end
end

return DreamBagTable