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


function GetOppositeDirection( dir )
	if ( dir == 0 ) then
		return 1;
	elseif ( dir == 1 ) then
		return 0;
	elseif ( dir == 2 ) then
		return 3;
	else
		return 2;
	end
end



