// TODO: Make the epi give you a delayed tiredness effect as it gives you more stamina first?
modded class Epinephrine
{
	override void OnApply(PlayerBase player)
	{
		super.OnApply(player);

		int penaltyPct = GetZenSleepConfig().GetEnergyDrink("Epinephrine").EnergyGained;
		if (penaltyPct == 0)
		{
			return;
		}

		float penalty = (float)player.MAX_TIREDNESS * ((float)penaltyPct / 100.0);

		if (GetZenSleepConfig().DebugOn)
		{
			player.ZS_SendMessage("Giving tiredness penalty: " + penalty);
		}
		
		player.InsertAgent(ZenSleep_Agents.TIREDNESS, penalty);
	}
}