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
};