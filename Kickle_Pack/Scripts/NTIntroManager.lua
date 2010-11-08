local NTIntroMngr = require("Entity")

NTIntroMngr.alpha = 0
NTIntroMngr.incalpha = true
NTIntroMngr.freezeLogo = false

-- Initializes intro manager to create the logo and set it to transparent
function NTIntroMngr.Init( self )
    logo = State.CreateObject( "Kickle_Pack/Objects/NTLogo.xml", 6, 7 )
    logo:SetAlpha(NTIntroMngr.alpha)
    self:ResetTimer()
end


--Skips to the game intro
function NTIntroMngr.Skip( self )
  State.LoadPath("Kickle_Pack/States/Intro.xml")
end


-- Increases the logo's alpha value, stopping once it is 100% opaque
-- Returns true once the logo has reached 100% opacity
function NTIntroMngr.IncLogoAlpha()
  NTIntroMngr.alpha = NTIntroMngr.alpha + 5
  if ( NTIntroMngr.alpha <= 255 ) then
    logo = State.GetObject( "NTLogo" )
    logo:SetAlpha( NTIntroMngr.alpha )
    return false
  end
  return true
end


-- Decreases the logo's alpha value, stopping once it is 100% transparent
-- Returns true once the logo has reached 100% transparency
function NTIntroMngr.DecLogoAlpha()
  NTIntroMngr.alpha = NTIntroMngr.alpha - 5
  if ( NTIntroMngr.alpha >= 0 ) then
    logo = State.GetObject("NTLogo")
    logo:SetAlpha( NTIntroMngr.alpha )
    return false
  end
  return true
end


--Logo fades in, holds, fades out, then title intro
function NTIntroMngr.AI( self )
  if ( NTIntroMngr.freezeLogo ) then
    if ( self:GetElapsedTime() > 1.5 ) then
      NTIntroMngr.freezeLogo = false
    end
    return
  end

  if ( self:GetElapsedTime() > 0.05 ) then
    if NTIntroMngr.incalpha then
      local opaque = NTIntroMngr.IncLogoAlpha()
      if opaque then
        NTIntroMngr.freezeLogo = true
        NTIntroMngr.incalpha = false
        self:ResetTimer()
      end

    else
      local trans = NTIntroMngr.DecLogoAlpha()
      if trans then
        NTIntroMngr.Skip()
      end
    end
    self:ResetTimer()
  end
end


return NTIntroMngr

