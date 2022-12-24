modded class AnimatedActionBase
{
	// Copy of vanilla GetActionCommand() but removed protected keyword so it can be referenced elsewhere (PlayerBase.c to check if a modded sleeping action is running)
	int GetActionCommandZENSLEEP(PlayerBase player)
	{
		if (HasProneException())
		{
			if (player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return m_CommandUID;
			else
				return m_CommandUIDProne;
		}

		return m_CommandUID;
	}

	// Reset sleep vars when Lie Down emote action is detected
	override void Start(ActionData action_data)
	{
		super.Start(action_data);

		if (action_data.m_Player)
		{
			// If player has just begun a sleep action, reset their sleep parameters for a fresh sleep
			if (GetActionCommandZENSLEEP(action_data.m_Player) == DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN)
			{
				action_data.m_Player.ResetSleep();
			}
		}
	}
};