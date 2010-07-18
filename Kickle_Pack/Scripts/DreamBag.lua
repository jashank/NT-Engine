package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
Util = require ("ObjectUtilities")

math.randomseed( os.time() )

--DreamBag Behavior Table

local DreamBag = {}

function DreamBag.HandleCollision( self, other )
  if ( not self:IsMoving() and (
        other:GetType() == "Slime" or other:GetType() == "Penguin" )) then
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
