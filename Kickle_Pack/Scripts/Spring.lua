package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require "ObjectUtilities"

--Spring Behavior Table
local Spring = require("Entity")
Spring.block = nil

function Spring.Init( self )
  local dir = self:GetAnim()
  self:SetDir( Util.GetOppositeDir( dir ))
  self:StopAnim()
end

function Spring.AI( self )
  if Spring.block then
    if self:GetFrame() <= 4 then
      Spring.block:SlowDown( 60 )
    elseif Spring.block:GetSpeed() == 0 and self:GetFrame() >= 5 then
      Spring.block:SetDir( Util.GetOppositeDir( Spring.block:GetDir()))
      Spring.block:SetSpeed( 240 )
      Spring.block = nil
    end
  end
end

function Spring.HandleCollision( self, other )
  self:StopAnim()
  local otherType = other:GetType()
  if otherType == "IceBlock" then
    self:PlayAnim()
  end
end

-- Stores block to "spring it" in and back out.
function Spring.SpringBlock( block )
  Spring.block = block
end

return Spring
