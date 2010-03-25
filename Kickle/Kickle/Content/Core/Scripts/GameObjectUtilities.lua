--GameObject Utilities

-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT

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



