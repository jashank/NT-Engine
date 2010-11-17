package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"
local Util = require "ObjectUtilities"

local Spring = require("Entity"):New()
Spring.block = nil

function Spring:Init( spring )
  local dir = spring:GetAnim()
  spring:SetDir( Util.GetOppositeDir( dir ))
  spring:StopAnim()
end


function Spring:AI( spring )
  if self.block then
    if spring:GetFrame() <= 4 then
      self.block:AdjustSpeed( -60 )
    elseif self.block:GetSpeed() == 0 and spring:GetFrame() >= 5 then
      self.block:SetDir( Util.GetOppositeDir( self.block:GetDir()))
      self.block:SetSpeed( 240 )
      self.block = nil
    end
  end
end


function Spring:HandleCollision( spring, other )
  spring:StopAnim()
  local otherType = other:GetType()
  if otherType == "IceBlock" then
    spring:PlayAnim()
  end
end


-- Stores block to "spring it" in and back out.
function Spring:SpringBlock( block )
  self.block = block
end

return Spring
