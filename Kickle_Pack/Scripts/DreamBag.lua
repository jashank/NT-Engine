package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--DreamBag Behavior Table

local DreamBagTable = {}

function DreamBagTable.HandleCollision( DreamBag, Other )
  if ( not DreamBag:IsMoving() and (
        Other:GetType() == "Slime" or Other:GetType() == "Penguin" )) then
    local dir = math.random( UP, RIGHT )
    local canMove = false
    local tileX, tileY = GetTileInDirection( DreamBag, dir )
    local otherBag = Game.GetGameObjectOnTile( tileX, tileY )

    if (( otherBag and otherBag:GetType() == "DreamBag" ) or
         not Game.TileIsCrossable( tileX, tileY )) then
      local newDir = GetNextDir( dir )

      while newDir ~= dir do
        local tileX, tileY = GetTileInDirection( DreamBag, newDir ) 
        local otherBag = Game.GetGameObjectOnTile( tileX, tileY )
        if (( otherBag and otherBag:GetType() == "DreamBag" ) or
            not Game.TileIsCrossable( tileX, tileY )) then
          newDir = GetNextDir( newDir )
        else
          dir = newDir
          canMove = true
        end
      end
    else
      canMove = true
    end

    if canMove then
      DreamBag:SetDir( dir )
      DreamBag:Move()
    end

  elseif ( Other:GetType() == "Kickle" ) then
    Game.DestroyGameObject( DreamBag )
  end
end

return DreamBagTable
