--MOneyBag Behavior Table

local MoneyBagTable = {}

function MoneyBagTable.HandleCollision( MoneyBag, Other )
	if ( Other:GetType() == "Kickle" ) then
		Level.DestroyGameObject( MoneyBag );
	end
end

return MoneyBagTable
