-- Slime's Script

local count = 0;

function AILogic( Slime )
	--print( "RUN THIS BITCHES!" );
	--Slime:MoveDir( 0 );
	--Make the slime move in circles(err a square)
	if count < 4 then
		Slime:MoveDir( 0 );
	elseif count < 8 then
		Slime:MoveDir( 2 );
	elseif count < 12 then
		Slime:MoveDir( 1 );
	elseif count < 16 then
		Slime:MoveDir( 3 );
	else
		count = 0;
	end
	count = count + 1;
end
