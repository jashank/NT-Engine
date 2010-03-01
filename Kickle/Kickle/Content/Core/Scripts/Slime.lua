-- Slime's Script

local count = 1;
function AILogic( Slime )

	-- Lead the slime move in circles(err, a square >_<)
	if count < 3 then
		Slime:MoveDir( 2 );
		Slime:SetAnimation( 2 );
		-- print( "Direction: Left" );
	elseif count < 5 then
		Slime:MoveDir( 1 );
		Slime:SetAnimation( 1 );
		-- print( "Direction: Down" );
	elseif count < 7 then
		Slime:MoveDir( 3 );
		Slime:SetAnimation( 3 );
		-- print( "Direction: Right" );
	elseif count < 9 then
		Slime:MoveDir( 0 );
		Slime:SetAnimation( 0 );
		-- print( "Direction: Up" );
	else
		count = 0;
	end
	count = count + 1;

	-- print( "Count: " .. count );
end
