package.path = package.path .. ";Content/Core/Scripts/?.lua"
require ("GameObjectUtilities")

--IceBlock Behavior Table

local IceBlockTable = {}

IceBlockTable.moving = false

function IceBlockTable.AILogic( IceBlock )
	if ( IceBlockTable.moving ) then
		IceBlock:Move()

		local facingX, facingY = GetTileObjectFaces( IceBlock )
        local tile = Level.GetTile( facingX, facingY );

		if ( tile == Tiles.WATER  or
             tile == Tiles.WATER_BOTTOM_LEFT or
             tile == Tiles.WATER_BOTTOM_CENTER or
             tile == Tiles.WATER_BOTTOM_RIGHT or
             tile == Tiles.ICE_CLIFF_LEFT or
             tile == Tiles.ICE_CLIFF_CENTER or
             tile == Tiles.ICE_CLIFF_RIGHT ) then

            local dir = IceBlock:GetDir();

			Level.DestroyGameObject( IceBlock )
			Level.CreateGameObject("Content/Core/Objects/Slime.xml", 11, 8 )


-- [[
            -- the tiles in cardinal directions from tile
            -- n:north, s:south, e:east, w:west
            local n = Level.GetTile( facingX, facingY-1 );
            local s = Level.GetTile( facingX, facingY+1 );
            local w = Level.GetTile( facingX-1, facingY );
            local e = Level.GetTile( facingX+1, facingY );

            -- the tiles in ordinal directions from tile
            -- nw:northwest, ne:northeast, se:southeast, sw:southwest
            local ne = Level.GetTile( facingX+1, facingY-1 );
            local se = Level.GetTile( facingX+1, facingY+1 );
            local nw = Level.GetTile( facingX-1, facingY-1 );
            local sw = Level.GetTile( facingX-1, facingY+1 );



            if( dir == UP ) then
                Level.SetTile( facingX, facingY+1, Tiles.ICE, 0 );
                if( tile == Tiles.ICE_CLIFF_CENTER ) then
                    Level.SetTile( facingX, facingY, Tiles.ICE, 0 );
                    Level.SetTile( facingX, facingY-1, Tiles.ICE, 0 );
                elseif( tile == Tiles.ICE_CLIFF_LEFT ) then
                    Level.SetTile( facingX, facingY, Tiles.ICE_LEFT, 0 );
                    Level.SetTile( facingX, facingY-1, Tiles.ICE_LEFT, 0 );
                elseif( tile == Tiles.ICE_CLIFF_RIGHT ) then
                    Level.SetTile( facingX, facingY, Tiles.ICE_RIGHT, 0 );
                    Level.SetTile( facingX, facingY-1, Tiles.ICE_RIGHT, 0 );
                else
                    Level.SetTile( facingX, facingY, Tiles.ICE_TOP, 0 );
                end


            elseif( dir == DOWN ) then
                Level.SetTile( facingX, facingY-1, Tiles.ICE, 0 );
                Level.SetTile( facingX, facingY, Tiles.ICE_BOTTOM, 0 );

            elseif( dir == LEFT ) then
                Level.SetTile( facingX-1, facingY, Tiles.ICE, 0 );
                Level.SetTile( facingX, facingY, Tiles.ICE_LEFT, 0 );

            elseif( dir == RIGHT ) then
                Level.SetTile( facingX+1, facingY, Tiles.ICE, 0 );
                Level.SetTile( facingX, facingY, Tiles.ICE_RIGHT, 0 );

            end
--]]


		end
	end
end


function IceBlockTable.HandleCollision( IceBlock, Other )
	if ( Other:GetType() == "IceBreath" ) then
		Level.DestroyGameObject( Other )
	end
end

return IceBlockTable
