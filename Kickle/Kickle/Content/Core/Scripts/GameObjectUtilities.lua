--GameObject Utilities

-- Direction constants for GameObjects
UP = 0;
DOWN = 1;
LEFT = 2;
RIGHT = 3;

-- Tile constants for setting tiles
ICE = 1;
WATER = 10;

function GetTileObjectFaces( GameObject )
	tileX = GameObject:GetTileX();
	tileY = GameObject:GetTileY();
	dir = GameObject:GetDir();

	if ( dir == UP ) then
		tileY = tileY - 1;
	elseif ( dir == DOWN ) then
		tileY = tileY + 1;
	elseif ( dir == LEFT ) then
		tileX = tileX - 1;
	else
		tileX = tileX + 1;
	end

	return tileX, tileY;

end

