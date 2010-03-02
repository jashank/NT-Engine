-- Kickle's Script

function HandleUserInput( input, Kickle )
  -- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT
  local whyplusfour = 4;
  if ( input == 0 ) then
	Kickle:MoveDir( 0 );
	Kickle:SetAnimation( 0 + whyplusfour );
  elseif ( input == 1 ) then
	Kickle:MoveDir( 1 );
	Kickle:SetAnimation( 1 + whyplusfour );
  elseif ( input == 2 ) then
	Kickle:MoveDir( 2 );
	Kickle:SetAnimation( 2 + whyplusfour );
  elseif ( input == 3 ) then
	Kickle:MoveDir( 3 );
	Kickle:SetAnimation( 3 + whyplusfour );
  end
end
