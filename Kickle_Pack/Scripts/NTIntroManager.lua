package.path = package.path .. ";Kickle_Pack/Scripts/?.lua"

local NTIntroMngr = require("Entity"):New()
NTIntroMngr.alpha = 0
NTIntroMngr.incalpha = true
NTIntroMngr.freezeLogo = false

-- Initializes intro manager to create the logo and set it to transparent
function NTIntroMngr:Init( obj )
  local logo = Map.CreateObject( "Kickle_Pack/Objects/NTLogo.xml", 6, 7 )
  logo:SetAlpha( self.alpha )
  obj:ResetTimer()
end


--Skips to the game intro
function NTIntroMngr:Skip( obj )
  State.LoadPath("Kickle_Pack/States/Intro.xml")
end


-- Increases the logo's alpha value, stopping once it is 100% opaque
-- Returns true once the logo has reached 100% opacity
function NTIntroMngr:IncLogoAlpha()
  self.alpha = self.alpha + 5
  if ( self.alpha <= 255 ) then
    local logo = Map.GetObject( "NTLogo" )
    logo:SetAlpha( self.alpha )
    return false
  end
  return true
end


-- Decreases the logo's alpha value, stopping once it is 100% transparent
-- Returns true once the logo has reached 100% transparency
function NTIntroMngr:DecLogoAlpha()
  self.alpha = self.alpha - 5
  if ( self.alpha >= 0 ) then
    local logo = Map.GetObject("NTLogo")
    logo:SetAlpha( self.alpha )
    return false
  end
  return true
end


--Logo fades in, holds, fades out, then title intro
function NTIntroMngr:AI( obj )
  if ( self.freezeLogo ) then
    if ( obj:GetElapsedTime() > 1.5 ) then
      self.freezeLogo = false
    end
    return
  end

  if ( obj:GetElapsedTime() > 0.05 ) then
    if self.incalpha then
      local opaque = self:IncLogoAlpha()
      if opaque then
        self.freezeLogo = true
        self.incalpha = false
        obj:ResetTimer()
      end

    else
      local trans = self:DecLogoAlpha()
      if trans then
        self:Skip()
      end
    end
    obj:ResetTimer()
  end
end


return NTIntroMngr

