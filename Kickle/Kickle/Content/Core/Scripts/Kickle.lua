-- Kickle's Script

-- 0 UP, 1 DOWN, 2 LEFT, 3 RIGHT
local dir = 0; -- Direction
local moveable = true; -- If true then kickle is moveable

-- 0 STANDING, 4 WALKING, 8 KICKING, 12 BLOWING, 16 DIEING
-- These values are determined by the layout of Kickle's animation sheet
local mode = 0;



-- [[ Function that is called to handle scripted collision response events ]]
function HandleCollision( Kickle, Other )
	if ( Other:GetType() == "Slime" ) then
		-- Show death animation
		Kickle:SetAnimation( dir + 16 );
		-- Disable kickle's movement
	     moveable = false;
	end

end



-- [[ Function that is called to handle scripted user input events ]]
function HandleUserInput( Kickle )
	if( moveable == true ) then

		if( Game.IsKeyDown( 293 ) ) then		-- 293 == value for the Up Arrow
			dir = 0;  -- set direction to UP
			mode = 4; -- set mode to Walking
		elseif( Game.IsKeyDown( 294 ) ) then	-- 294 == value for the Down Arrow
			dir = 1;  -- set direction to DOWN
			mode = 4; -- set mode to Walking
		elseif( Game.IsKeyDown( 291 ) ) then	-- 291 == value for the Left Arrow
			dir = 2;  -- set direction to LEFT
			mode = 4; -- set mode to Walking
		elseif( Game.IsKeyDown( 292 ) ) then	-- 292 == value for the Right Arrow
			dir = 3;  -- set direction to RIGHT
			mode = 4; -- set mode to Walking
		elseif( Game.IsKeyDown( 120 ) ) then
			Game.CreateGameObject( 9, 7, "Content/Core/Objects/Pillar.xml" );
		else
			-- No key was pressed so:
			mode = 0; -- set mode to STANDING
		end

		if( mode == 4 ) then
			Kickle:MoveDir( dir );
		end

		Kickle:SetAnimation( dir + mode );


		--[[
		In the future calls to Game.IsKeyDown will look more like this:
		Game.IsKeyDown( Game.Keys.Left )
		Once I learn how to expose Tables to Lua through C/C++
		]]
	end
end
