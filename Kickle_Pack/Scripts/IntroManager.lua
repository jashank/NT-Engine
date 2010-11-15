package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local IntroMngr = require("Entity"):New()
IntroMngr.slimeCnt = 0
IntroMngr.scene = 0

-- Resets timer. Many sequences in intro are timed.
function IntroMngr:Init( mngr )
  State.SpanCam( 15, 15 )
  State.OffsetCam( 1, 1 )
  mngr:ResetTimer()
  State.PlayMusic( "RetroKickle" )
end


-- Enters a slime from the left side of the screen at the location specified.
-- Returns true if a slime was able to enter at that location, false if it
-- couldn't (something was already there). Slimes are set to block the tiles
-- they are on.
function IntroMngr:EnterSlime( tileX, tileY )
  local slimeA = State.GetObjectOnTile( tileX, tileY )
  local slimeB = State.GetObjectOnTile( tileX + 1, tileY )
  if slimeA or slimeB then
    return false
  end

  local s = State.CreateObject( 
    "Kickle_Pack/Objects/IntroSlime.xml", tileX, tileY )
  return true
end


-- Returns true if all slimes on screen are sitting and stops them from
-- blocking their tiles.
function IntroMngr:SlimesSitting()
  local slimes = State.GetObjects( "IntroSlime" )
  for k, v in ipairs( slimes ) do
    if not v:GetTable():IsSitting() then return false end
  end

  for k, v in ipairs( slimes ) do
    v:BlockTileRange( false )
  end
  return true
end


-- Creates ice blocks to fly towards slimes
function IntroMngr:CreateBlocks()
  local slimes = State.GetObjects( "IntroSlime" )
  for k, v in ipairs( slimes ) do
    local x, y = v:GetTile()
    State.CreateObject( "Kickle_Pack/Objects/IntroBlock.xml", x, 0 )
  end
end


-- Depending on the last scene, performs a different scene. At end
-- go to title screen.
function IntroMngr:AI( mngr )
  -- Enter slimes
  if (( self.slimeCnt == 0 and mngr:GetElapsedTime() > 1 ) or 
        self.slimeCnt > 0 and self.slimeCnt < 3 ) then
    local entered = self:EnterSlime( 0, 8 )
    if entered then self.slimeCnt = self.slimeCnt + 1 end
    if self.slimeCnt == 3 then
      self.scene = self.scene + 1
    end

  -- Wait for slimes to sit
  elseif ( self.scene == 1 ) then
    if self:SlimesSitting() then
      mngr:ResetTimer()
      self.scene = self.scene + 1
    end

  -- Throw blocks at slimes after 1 second of them sitting
  elseif ( self.scene == 2 ) then
    if mngr:GetElapsedTime() > 1 then
      self:CreateBlocks()
      self.scene = self.scene + 1 
      mngr:ResetTimer()
    end

  -- Enter Kickle once blocks are at bottom of screen.
  -- Kickle handles the rest.
  elseif ( self.scene == 3 ) then
    local block = State.GetObject( "IntroBlock" )
    local x, y = block:GetTile()
    if y == 14 then
      State.CreateObject( "Kickle_Pack/Objects/IntroKickle.xml", 8, 0 )
      self.scene = self.scene + 1
    end
  end
    
end


function IntroMngr:Skip( mngr )
  State.LoadPath( "Kickle_Pack/States/MainMenu.xml" )
end

return IntroMngr

