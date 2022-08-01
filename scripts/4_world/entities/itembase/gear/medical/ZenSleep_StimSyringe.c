class ZenSleep_StimSyringe : Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionInjectTarget);
		AddAction(ActionInjectSelf);
	}

	override void OnApply(PlayerBase player)
	{
		player.GiveShock(100); // Recover all shock from player (ie. immediately revive from uncon)

		int penaltyPct = GetZenSleepConfig().GetEnergyDrink("ZenSleep_StimSyringe").EnergyGained;
		if (penaltyPct == 0)
		{
			return;
		}

		float penalty = (float)player.MAX_TIREDNESS * ((float)penaltyPct / 100.0); // Copy & pasted, technically not a penalty but whatever

		// If the penalty is supposed to be positive we need to flip it to negative as otherwise we're ADDING tiredness here
		if (penaltyPct > 0)
		{
			penalty = penalty * -1;
		}

		if (GetZenSleepConfig().DebugOn)
		{
			player.ZS_SendMessage("Giving tiredness penalty: " + penalty);
		}

		player.InsertAgent(ZenSleep_Agents.TIREDNESS, penalty);

		// Drop used syringe
		ItemBase junk = ItemBase.Cast(GetGame().CreateObjectEx(GetZenSleepConfig().UsedStimSyringeType, player.GetPosition(), ECE_PLACE_ON_SURFACE));
		if (junk)
		{
			junk.SetHealth("", "Health", 1);
			junk.SetQuantity(0, false, false); // false, false overrides the game deleting edibles with <= 0 quantity.
		}
	}
}

class ZenSleep_Empty_StimSyringe : Inventory_Base {}