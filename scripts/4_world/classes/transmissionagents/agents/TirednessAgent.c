class TirednessAgent: AgentBase
{
	override void Init()
	{
		m_Type 					= ZenSleep_Agents.TIREDNESS;
		m_Invasibility 			= 0.08;
		m_TransferabilityIn		= 0;
		m_TransferabilityOut	= 0;
		m_AntibioticsResistance = 1;
		m_MaxCount 				= 1000;
		m_Potency = EStatLevels.GREAT;
		m_DieOffSpeed = 1;
	}

	override float GetInvasibilityEx(PlayerBase player)
	{
		if (player.IsPlayerSleeping())
			return 0;

		return super.GetInvasibilityEx(player);
	}

	override float GetInvasibility()
	{
		if (GetZenSleepConfig().MaxWakeTime > 0)
		{
			m_Invasibility = (float)m_MaxCount / (GetZenSleepConfig().MaxWakeTime * 60);
		}

		return m_Invasibility;
	}
}