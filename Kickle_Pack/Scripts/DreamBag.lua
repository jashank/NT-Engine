package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
require ("GameObjectUtilities")

math.randomseed( os.time() )

--DreamBag Behavior Table

local DreamBag = {}

function DreamBag.HandleCollision( self, other )
  if ( not self:IsMoving() and (
        other:GetType() == "Slime" or other:GetType() == "Penguin" )) then
    local dir = math.random( UP, RIGHT )
    local canMove = false
    local tileX, tileY = GetTileInDirection( self, dir )
    local otherBag = Game.GetGameObjectOnTile( tileX, tileY )

    if (( otherBag and otherBag:GetType() == "DreamBag" ) or
         not Game.TileIsCrossable( tileX, tileY )) then
      local newDir = GetNextDir( dir )

      while newDir ~= dir do
        local tileX, tileY = GetTileInDirection( self, newDir ) 
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
      self:SetDir( dir )
      self:Move()
    end

  elseif ( other:GetType() == "Kickle" ) then
    Game.DestroyGameObject( self )
  end
end

return DreamBag
