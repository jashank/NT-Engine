package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

math.randomseed( os.time() )

local DreamBag = require("Entity"):New()

function DreamBag:HandleCollision( bag, other )
  if other:GetType() == "Pillar" then
    State.DestroyObject( other )
  end
end


function DreamBag:Push( bag )
  if ( not bag:IsMoving() ) then
    local dir = math.random( Util.UP, Util.RIGHT )
    local canMove = false
    local tileX, tileY = Util.GetTileInDirection( bag, dir )
    local otherBag = State.GetObjectOnTile( tileX, tileY )

    if (( otherBag and otherBag:GetType() == "DreamBag" ) or
         not State.TileIsCrossable( tileX, tileY )) then
      local newDir = Util.GetNextDir( dir )

      while newDir ~= dir do
        local tileX, tileY = Util.GetTileInDirection( bag, newDir ) 
        local otherBag = State.GetObjectOnTile( tileX, tileY )
        if (( otherBag and otherBag:GetType() == "DreamBag" ) or
            not State.TileIsCrossable( tileX, tileY )) then
          newDir = Util.GetNextDir( newDir )
        else
          dir = newDir
          canMove = true
        end
      end
    else
      canMove = true
    end

    if canMove then
      bag:SetDir( dir )
      bag:Move()
    end
  end
end


return DreamBag

