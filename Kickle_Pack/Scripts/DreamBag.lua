package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require ("ObjectUtilities")

math.randomseed( os.time() )

local DreamBag = {}
setmetatable( DreamBag, {__index = require("Entity")})

function DreamBag.HandleCollision( self, other )
  if other:GetType() == "Pillar" then
    State.DestroyObject( other )
  end
end


function DreamBag.Push( self )
  if ( not self:IsMoving() ) then
    local dir = math.random( Util.UP, Util.RIGHT )
    local canMove = false
    local tileX, tileY = Util.GetTileInDirection( self, dir )
    local otherBag = State.GetObjectOnTile( tileX, tileY )

    if (( otherBag and otherBag:GetType() == "DreamBag" ) or
         not State.TileIsCrossable( tileX, tileY )) then
      local newDir = Util.GetNextDir( dir )

      while newDir ~= dir do
        local tileX, tileY = Util.GetTileInDirection( self, newDir ) 
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
      self:SetDir( dir )
      self:Move()
    end
  end
end


return DreamBag

