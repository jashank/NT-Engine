--GameObject Utilities

-- Direction constants for GameObjects
UP = 0;
DOWN = 1;
LEFT = 2;
RIGHT = 3;

function GetTileObjectFaces( GameObject, dir )
	tileX = GameObject:GetTileX();
	tileY = GameObject:GetTileY();

	if ( dir == 0 ) then
		tileY = tileY - 1;
	elseif ( dir == 1 ) then
		tileY = tileY + 1;
	elseif ( dir == 2 ) then
		tileX = tileX - 1;
	else
		tileX = tileX + 1;
	end

	return tileX, tileY;

end

