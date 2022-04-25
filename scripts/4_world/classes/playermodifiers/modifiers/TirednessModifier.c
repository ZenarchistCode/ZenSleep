// This class handles the updating of the player's tiredness, makes them yawn and makes them go uncon randomly if they're too sleepy
class TirednessModifier: ModifierBase
{
	private bool m_SentUnconMessageLastUpdate = false;
	private float m_TirednessThreshold = -1;
	private float m_UnconThreshold = -1;

	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= ZenSleep_Modifiers.ZEN_TIREDNESS;
		m_TickIntervalInactive 	= 6;
		m_TickIntervalActive 	= 6;
		m_TirednessThreshold    = PlayerBase.MAX_TIREDNESS * ((float)(100.0 - GetZenSleepConfig().YawnPercentStart) / 100.0); // Converts yawn % start into a tiredness value/1000 (max tiredness)
		m_UnconThreshold        = PlayerBase.MAX_TIREDNESS * ((float)(100.0 - GetZenSleepConfig().UnconPercentStart) / 100.0); // Converts uncon % start into a tiredness value/1000 (max tiredness)
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		float tiredness = player.GetSingleAgentCount(ZenSleep_Agents.TIREDNESS);

		if (tiredness == 0)
		{
			player.InsertAgent(ZenSleep_Agents.TIREDNESS, 1);
			return;
		}

		if (player.IsPlayerSleeping() || player.IsUnconscious())
		{
			return;
		}

		// Prepare tired "depth" values (how deep are we into tiredness beyond our yawn percent start threshold? This is used to increase the chance of yawns etc)
		float tiredDepth = (player.MAX_TIREDNESS * ((GetZenSleepConfig().YawnPercentStart) / 100.0) - (player.MAX_TIREDNESS - tiredness); // How far into sleepiness are we past our yawn percentage?
		float tiredDepthStep = tiredDepth / 50.0;
		float tiredDepthPenalty = 0;
		if (tiredDepth > 0)
		{
			tiredDepthPenalty = tiredDepthStep / 100; // With the default settings this means: Convert increments of > 50 steps past 800 tiredness into +1% chance of yawn and +0.5% chance of uncon
		}

		float yawnChance = GetZenSleepConfig().YawnPercentChance + tiredDepthPenalty;
		float unconChance = GetZenSleepConfig().UnconPercentChance + (tiredDepthPenalty / 2.0);

		float rand = Math.RandomFloat01();
		bool fallUnconscious = tiredness >= m_UnconThreshold && rand <= unconChance;

		// Player is max tired
		if (tiredness >= PluginTransmissionAgents.GetAgentMaxCount(ZenSleep_Agents.TIREDNESS))
		{
			fallUnconscious = true;
			unconChance = 100.0;

			if (GetZenSleepConfig().DebugOn)
			{
				player.SendMessage("Fall uncon forced due to tiredness: " + tiredness + "/" + PluginTransmissionAgents.GetAgentMaxCount(ZenSleep_Agents.TIREDNESS));
			}

			// Sometimes the client won't tell the server it's ready to fall uncon after yawning, this overrides that if necessary (TODO: Find a better way to fix this strange issue!)
			if (fallUnconscious && m_SentUnconMessageLastUpdate && !player.IsUnconscious())
			{
				player.SetPlayerUncon();
				player.m_IsUnconsciousFromTiredness = true;
				return;
			}
		}

		// If config has disabled falling uncon in a vehicle and player is in a vehicle, override fallUnconscious
		if (fallUnconscious && player.IsInVehicle() && !GetZenSleepConfig().CanUnconInVehicle)
		{
			fallUnconscious = false;
		}

		if (tiredness >= m_TirednessThreshold || fallUnconscious)
		{
			// If random trigger, or player is falling uncon, play a yawn. 
			// But don't play the sound if player.m_FallUnconsciousFromTiredness is already true (prevents playing lots of yawns in a row).
			if ((rand <= GetZenSleepConfig().YawnPercentChance || fallUnconscious || tiredness == m_TirednessThreshold) && !player.m_FallUnconsciousFromTiredness)
			{
				// TODO: Make it so two of the same yawn can't play in a row (immersive-breaking)?
				player.MakeYawnSound();
			}
		}

		if (fallUnconscious)
		{
			if (GetZenSleepConfig().DebugOn)
			{
				player.SendMessage("Fall uncon!");
			}

			player.SetUnconsciousFromTiredness();
			m_SentUnconMessageLastUpdate = true;
		}
		else
		{
			m_SentUnconMessageLastUpdate = false;
		}

		// Debug message
		if (GetZenSleepConfig().DebugOn)
		{
			string debugStr = "";
			if (tiredness < m_TirednessThreshold)
			{
				debugStr = " Yawn/Uncon N/A";
			}
			else
			{
				debugStr = " | tiredDepthPenalty=" + tiredDepthPenalty + " | yawnChance=" + yawnChance + " | unconChance=" + unconChance + " | rand=" + rand + " yawn=" + (rand <= yawnChance) + " uncon=" + fallUnconscious + " m_FallUnconsciousFromTiredness=" + player.m_FallUnconsciousFromTiredness;
			}

			player.SendMessage("Tiredness=" + tiredness + debugStr);
		}
	}
}