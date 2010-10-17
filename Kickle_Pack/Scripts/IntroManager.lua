local IntroMngr = {}
IntroMngr.slimeCnt = 0

-- Resets timer. Many sequences in intro are timed.
function IntroMngr.Init( self )
  self:ResetTimer()
end


-- Enters a slime from the left side of the screen at the location specified.
-- Returns true if a slime was able to enter at that location, false if it
-- couldn't (something was already there).
function IntroMngr.EnterSlime( tileX, tileY )
  slimeA = State.GetObjectOnTile( tileX, tileY )
  slimeB = State.GetObjectOnTile( tileX + 1, tileY )
  if slimeA or slimeB then
    return false
  end

  State.CreateObject( "Kickle_Pack/Objects/IntroSlime.xml", tileX, tileY )
  return true
end


-- Depending on the last scene, performs a different scene. At end
-- go to title screen.
function IntroMngr.AI( self )
  if (( IntroMngr.slimeCnt == 0 and self:GetElapsedTime() > 1 ) or 
        IntroMngr.slimeCnt > 0 and IntroMngr.slimeCnt < 3 ) then
    local entered = IntroMngr.EnterSlime( 0, 8 )
    if entered then IntroMngr.slimeCnt = IntroMngr.slimeCnt + 1 end
 end
end

return IntroMngr

