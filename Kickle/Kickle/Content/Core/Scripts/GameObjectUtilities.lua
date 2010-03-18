--GameObject Utilities

function GetTileObjectFaces( GameObject, dir )
	tileX = GameObject:GetTileX();
	tileY = GameObject:GetTileY();

	-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT
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


