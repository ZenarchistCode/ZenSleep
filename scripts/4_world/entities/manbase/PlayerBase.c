modded class PlayerBase
{
	// Server-side variables
	static const int MAX_TIREDNESS = 1000; // The maximum amount our m_Tiredness reading can reach
	static const float REST_GAIN_PER_SEC = 0.023; // How much rest we recover per second when sleeping
	bool m_CancelSleep = false; // Overrides/cancels sleeping state
	bool m_IsSleeping = false; // Tracks whether the player is lying down and resting
	bool m_CantSleep = false; // When this is true, the player wakes up but stays lying down (ie. when max rest is reached)
	bool m_SleepingInside = false; // This affects our sleep accelerator modifier but is only checked when sleeping
	bool m_SleepImmunityActivated = false; // This is used to enable sleep immunity boost (if enabled in server config)
	int m_TirednessUpdateCountdown = 0; // Used to countdown ticks between sending sleep % update message
	float m_PlayerRestTick = 1; // Current tick for the player checker function
	float m_RestObjectTick = 1; // Current tick for the rest object checker function
	float m_AccumulatedRest = 1; // How much rest we have accumulated while sleeping
	float m_SleepAccumulatorModifier = 0; // This slowly increases while you sleep, making your sleep get faster (rewarding you for staying asleep longer).
	float m_TimeAsleep = 0; // How long the player has been asleep for
	PluginPlayerStatus m_ModulePlayerStatus; // The PlayerStatus Plugin object for this mod
	
	// Server-side rest object variables
	int m_RestObjectChecks = 0; // How many times we've scanned the area for rest objects
	int m_RestObjectMaxSleep = 0; // Nearby rest object max sleep % when sleeping inside of it
	float m_RestObjectAccelerator = 0; // This increases the speed at which you recover rest based on the rest object (tents etc)
	bool m_RestObjectInfluenza = true; // Whether or not you can catch influenza while sleeping in the current nearby rest object
	bool m_FireNearby = false; // Whether or not there was a fire lit nearby recently

	// Client & server sync variables
	bool m_AttenuateSound = false; // If true, the sound is attenuated when the player is asleep
	bool m_OnlyBlurScreen = false; // If true, then when the player is sleeping their screen will blur instead of going black
	bool m_AllowInventoryWhileSleep = false; // Whether or not the player is permitted to access their inventory while sleeping
	int m_OnlyShowSleepAbovePercent = 0; // From ZenConfig: Sets what sleep level to show the sleep meter at (optional - 0 = disabled)
	int m_SleepState; // Stores various sync settings related to this mod
	int m_Tiredness = 0; // Our current 'sleepiness'
	int m_CurrentYawn = 0; // Used to trigger the yawn black screen effect. If this doesn't match m_LastYawn the blink effect is triggered
	int m_PlayYawnSound = 0; // Tells the client which yawn sound to play
	int m_LastYawnSound = -1; // Saves the last yawn sound to prevent playing the same sound in a row
	int m_PlaySleepSound = 0; // Tells the client which sleep sound to play
	int m_LastSleepSound = -1; // Saves the last sleep sound to prevent playing the same sound in a row
	bool m_FallUnconsciousFromTiredness = false; // This triggers the black screen effect if the player is forced uncon from tiredness
	bool m_IsUnconsciousFromTiredness = false; // This tracks on the server & client if the player is uncon from tiredness
	bool m_OnlyShowSleepOnInventory = false; // This tracks whether or not to only show the sleep meter while tabbed
	bool m_HideHudWhileSleeping = true; // Hide the left-hand HUD while sleeping?
	float m_TirednessHudX = 0.855; // Tiredness GUI HUD x position (needs server-side instructions to move it)
	float m_TirednessHudY = 0.03; // Tiredness GUI HUD y position (needs server-side instructions to move it)

	// Client-side & server-side no sync variables
	bool m_IsAllowedToSleep = false; // Added for compatibility with my Immersive Login mod that lies down on login (prevents triggering sleep)
	bool m_ReceivedSleepConfig = false; // Tracks whether or not sleep server config was received by client yet
	bool m_WasSleeping = false; // Tracks the player's sleep state to enable/disable the black screen
	float m_YawnTime = 0; // Used for governing the speed of the black screen blink effect.
	int m_LastYawn = 0; // Used for triggering the yawn black screen effect.
	static const float YAWN_TIME_1 = 5.0; // Used for timing the blink black screen effect
	static const float YAWN_TIME_2 = 6.0; // Used for timing the blink black screen effect
	static const float YAWN_TIME_3 = 6.3; // Used for timing the blink black screen effect
	float m_TirednessVignetteValue; // Used for timing the blink black screen effect
	int m_prevTirednessPercent; // Used for staggering the text updates telling the player their rest %
	EffectSound yawnSoundEffect = NULL; // Used for playing the yawn sounds
	EffectSound sleepSoundEffect = NULL; // Used for playing the sleep sounds

	// Print a debug message (client-only)
	void ZS_DebugMessage(string message)
	{
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "[Sleep:" + GetIdentity().GetName() + "] " + message, ""));
		}
	}

	// Sends a text message to the client (ZS_ to prevent conflicts with other mods that might use the same method name)
	void ZS_SendMessage(string message)
	{
		Param1<string> m_MessageParam = new Param1<string>("");
		if (GetGame().IsDedicatedServer() && m_MessageParam && IsAlive() && message != "" && !IsPlayerDisconnected())
		{
			m_MessageParam.param1 = message;
			GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, GetIdentity());
		}
	}

	// Init player
	override void Init()
	{
		super.Init();

		// Register variables that need to be sync'd to the client
		RegisterNetSyncVariableInt("m_Tiredness", 0, MAX_TIREDNESS);
		RegisterNetSyncVariableInt("m_CurrentYawn", 0, MAX_TIREDNESS);
		RegisterNetSyncVariableInt("m_SleepState");
		RegisterNetSyncVariableInt("m_PlayYawnSound");
		RegisterNetSyncVariableInt("m_PlaySleepSound");

		// Prepare player status plugin
		m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
	}

	// Called when the player is loaded
	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();

		if (GetGame().IsDedicatedServer())
		{
			// Send server-side data to client
			ScheduleSleepDataUpdate();
		}
	}

	// (Server-side) Sets the server-side settings that the player client must sync
	void SyncServerConfig()
	{
		// Bools
		m_OnlyShowSleepOnInventory = GetZenSleepConfig().OnlyShowSleepOnInventory;
		m_HideHudWhileSleeping = GetZenSleepConfig().HideHudWhileSleeping;
		m_AllowInventoryWhileSleep = GetZenSleepConfig().AllowInventoryWhileSleep;
		m_OnlyBlurScreen = GetZenSleepConfig().SleepBlackScreen == 2;
		m_AttenuateSound = GetZenSleepConfig().AttenuateSound;

		// Ints
		m_OnlyShowSleepAbovePercent = GetZenSleepConfig().OnlyShowSleepAbovePercent;

		// Floats
		m_TirednessHudX = GetZenSleepConfig().TirednessHudX;
		m_TirednessHudY = GetZenSleepConfig().TirednessHudY;
	}

	// (Server-side) Queue sending of server-side settings
	void ScheduleSleepDataUpdate()
	{
		SyncServerConfig();
		SendSleepDataToClient();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendSleepDataToClient, 5000, false);
	}

	// (Server-side) Sends an RPC containing all of the server-side sleep config settings that the client needs to be aware of
	void SendSleepDataToClient()
	{
		if (!IsPlayerDisconnected())
		{
			GetRPCManager().SendRPC("ZS_RPC", "RPC_SendSleepDataToClient", new Param8< bool, bool, bool, bool, int, float, float, bool >(m_OnlyShowSleepOnInventory, m_HideHudWhileSleeping, m_AllowInventoryWhileSleep, m_OnlyBlurScreen, m_OnlyShowSleepAbovePercent, m_TirednessHudX, m_TirednessHudY, m_AttenuateSound), true, this.GetIdentity());
		}
	}

	// (Client-side) Sends an admin RPC requesting the server to reload the ZenSleep config
	void RequestServerConfigReload()
	{
		GetRPCManager().SendRPC("ZS_RPC", "RPC_SendReloadConfigRequestToServer", new Param1< PlayerBase >(this), true, NULL);
	}

	// Triggered when variables are synchronized from the server to the client
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		ZenSleep_ReadState();

		// Debug message
		//ZS_DebugMessage("Sleeping=" + m_IsSleeping + " YawnSound=" + m_PlayYawnSound + " SleepSound=" + m_PlaySleepSound + " CurrentYawn=" + m_CurrentYawn + " FallUnconsciousFromTiredness=" + m_FallUnconsciousFromTiredness);

		// Check for yawn sound trigger. This plays on ALL player objects, so we need to do the blink effect separately on PlayerRestCheck
		if (m_PlayYawnSound > 0 && !IsUnconscious()) // If we receive a yawn sound effect ID from server and the player is not uncon, play the sound.
		{
			PlayYawnSound(m_PlayYawnSound);
			m_PlayYawnSound = 0;
		}

		// Check for sleep sound trigger.
		if (m_PlaySleepSound > 0)
		{
			PlaySleepSound(m_PlaySleepSound);
			m_PlaySleepSound = 0;
		}
	}

	// (Client) Plays a yawn sound
	void PlayYawnSound(int id)
	{
		// Play yawn sound
		string soundSetYawn = "yawn" + id.ToString() + "_Male_SoundSet";
		if (!IsMale())
		{
			soundSetYawn = "yawn" + id.ToString() + "_Female_SoundSet";
		}

		yawnSoundEffect = SEffectManager.PlaySoundOnObject(soundSetYawn, this);
		//yawnSoundEffect.Event_OnSoundWaveEnded.Insert(StopYawnSound);
		yawnSoundEffect.SetSoundAutodestroy(true);
	}

	// (Client) Plays a sleep sound
	void PlaySleepSound(int id)
	{
		// Play sleep sound
		string soundSetSleep = "sleep" + id.ToString() + "_Male_SoundSet";
		if (!IsMale())
			soundSetSleep = "sleep" + id.ToString() + "_Female_SoundSet";

		sleepSoundEffect = SEffectManager.PlaySoundOnObject(soundSetSleep, this);
		//sleepSoundEffect.Event_OnSoundWaveEnded.Insert(StopSleepSound);
		sleepSoundEffect.SetSoundAutodestroy(true);
	}

	// Sets our sync state variables (server-side & client-side). Uses bitwise operators to be more efficient (from Lucian's original Medical Attention code)
	void ZenSleep_SetState()
	{
		m_SleepState = 0;

		if (m_IsSleeping)
			m_SleepState |= ZenSleep_SyncState.IsSleeping;

		if (m_FallUnconsciousFromTiredness)
			m_SleepState |= ZenSleep_SyncState.FallUnconsciousFromTiredness;
	}

	// (Client) read the state sent from the server
	void ZenSleep_ReadState()
	{
		m_IsSleeping = (m_SleepState & ZenSleep_SyncState.IsSleeping) != 0;
		m_FallUnconsciousFromTiredness = (m_SleepState & ZenSleep_SyncState.FallUnconsciousFromTiredness) != 0;
	}

	// Sync's the client to the server
	void ZenSleep_SyncState()
	{
		ZenSleep_SetState();
		SetSynchDirty();
	}

	// Resets all sleep variables for a fresh sleep (server-side)
	void ResetSleep()
	{
		m_IsAllowedToSleep = true;
		m_CantSleep = false;
		m_SleepingInside = false;
		m_TimeAsleep = 0;
		m_PlayerRestTick = 0;
		m_RestObjectTick = 0;
		m_AccumulatedRest = 1;
		m_TirednessUpdateCountdown = 0;
		m_SleepAccumulatorModifier = 0;
		m_RestObjectMaxSleep = 0;
		m_RestObjectAccelerator = 0;
		m_RestObjectChecks = 0;
		m_YawnTime = 0;
		m_RestObjectInfluenza = true;
		m_FireNearby = false;
		m_SleepImmunityActivated = false;

		// Check for a fire/rest object nearby
		GetNearbyRestObjectAccelerator();
	}

	// Returns how tired the player currently is (as a percentage between 0.0-100.0%)
	float GetTiredPercent()
	{
		return ((float)m_Tiredness / (float)MAX_TIREDNESS) * 100.0;
	}

	// Is the player currently in 'sleep' state?
	bool IsPlayerSleeping()
	{
		bool isSleeping = true;
		
		// Check for cancel sleep and can't sleep conditions, and sleep emote/actions
		if (m_CancelSleep || m_CantSleep || !m_EmoteManager || !m_EmoteManager.m_bEmoteIsPlaying || !m_EmoteManager.m_Callback || m_EmoteManager.m_Callback.m_callbackID != DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN)
		{
			// Check for modded actions that cause player to lie down (eg. BoomLay's Things modded beds)
			ActionManagerBase amb = GetActionManager();
			if (amb && amb.GetRunningAction() && !m_CantSleep)
			{
				AnimatedActionBase aab = AnimatedActionBase.Cast(amb.GetRunningAction());
				if (aab)
				{
					if (aab.GetActionCommandZENSLEEP(this) != DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN) // Current action is not a sleep action.
					{
						isSleeping = false;
						m_CancelSleep = false;
					}
				}
				else // No animated action found, we are not asleep.
				{
					isSleeping = false;
					m_CancelSleep = false;
				}
			}
			else // No current sleeping action found, we are not asleep.
			{
				isSleeping = false;
				m_CancelSleep = false;
			}
		}

		if (!isSleeping && m_IsUnconsciousFromTiredness)
		{
			isSleeping = true;
		}

		if (GetZenSleepConfig().SleepBlackScreen)
		{
			m_IsSleeping = isSleeping;
			ZenSleep_SetState();
		}

		return isSleeping;
	}

	// (Server) Tells the client to play a sleep sound effect (sync'd by server so that nearby players can also hear it)
	void MakeSleepSound()
	{
		m_PlaySleepSound = Math.RandomIntInclusive(1, 3); // Play random sleep sound effect out of 3 possible
		if (m_PlaySleepSound == m_LastSleepSound)
		{
			switch (m_LastSleepSound) // If we're playing the same sleep sound as last time, switch it up
			{
				case 1:
					m_PlaySleepSound = 2;
					break;
				case 2:
					m_PlaySleepSound = 3;
					break;
				case 3:
					m_PlaySleepSound = 1;
					break;
			}
		}
		m_LastSleepSound = m_PlaySleepSound;
	}

	// (Server) Tells the client to play a yawn sound (and trigger black screen effect)
	void MakeYawnSound()
	{
		m_PlayYawnSound = Math.RandomIntInclusive(1, 3); // Play random yawn sound effect out of 3 possible
		if (m_PlayYawnSound == m_LastYawnSound)
		{
			switch (m_LastYawnSound) // If we're playing the same yawn sound as last time, switch it up
			{
				case 1:
					m_PlayYawnSound = 2;
					break;
				case 2:
					m_PlayYawnSound = 3;
					break;
				case 3:
					m_PlayYawnSound = 1;
					break;
			}
		}

		m_LastYawnSound = m_PlayYawnSound;
		m_CurrentYawn++;
		if (m_CurrentYawn > 3) // 3 is just a way to cap this int below 3 as we only detect a change in its value, the value doesn't matter
		{
			m_CurrentYawn = 0;
		}

		if (!m_FallUnconsciousFromTiredness && GetZenSleepConfig().TextNotificationOn && Math.RandomIntInclusive(1, 3) == 2)
		{
			int rand = Math.RandomIntInclusive(1, 3);
			string msg = GetZenSleepConfig().Str_ITooTired1; // "I feel really tired... I should lie down soon and get some rest."
			if (rand == 2)
			{
				msg = GetZenSleepConfig().Str_ITooTired2; // "I've been awake for a long time now... I should lie down and get some rest."
			}
			else
			if (rand == 3)
			{
				msg = GetZenSleepConfig().Str_ITooTired3; // "I don't have much energy... I should lie down soon and get some rest."
			}

			ZS_SendMessage(msg);
		}
	}

	// (Client) Checks if we need to trigger the black screen yawn effect
	void PlayerYawnEffectCheck()
	{
		if (m_CurrentYawn != m_LastYawn)
		{
			m_LastYawn = m_CurrentYawn;
			if (m_YawnTime == 0) // Trigger yawn black screen effect
			{
				m_YawnTime = 0.01;
			}
		}
	}

	// Checks if the player is underneath a roof & inside (I made this check because when sleeping really close to a fire, IsSoundInsideBuilding() was returning true)
	void CheckIfPlayerIsInside()
	{
		// If inside vehicle return immediately
		if (IsInVehicle())
		{
			m_SleepingInside = false;
			return;
		}

		m_SleepingInside = MiscGameplayFunctions.IsUnderRoof(this) && IsSoundInsideBuilding();
	}

	// Returns the standard sleep accelerator modifier based on the player's current environment
	float GetFireSleepAccelerator()
	{
		if (GetStatWet().Get() == GetZenSleepConfig().WetnessCancelsFireAccelerator)
		{
			return GetZenSleepConfig().BaseSleepAccelerator; // Fire acceleration buff doesn't count while wet
		}

		if (m_SleepingInside) // If player is inside a building (or a medium tent)
		{
			if (m_FireNearby || (m_HasHeatBuffer && GetZenSleepConfig().HeatBuffCountsAsFire)) // Fire detected
			{
				return (GetZenSleepConfig().InsideFireAcceleratorPercent / 100) + 1; // Inside + fire buff
			}

			return (GetZenSleepConfig().InsideNoFireAcceleratorPercent / 100) + 1; // Inside buff
		}
		else // If player is outside in the wild
		{
			if (m_FireNearby || (m_HasHeatBuffer && GetZenSleepConfig().HeatBuffCountsAsFire)) // Fire detected
			{
				return (GetZenSleepConfig().OutsideFireAcceleratorPercent / 100) + 1;
			}
		}

		return GetZenSleepConfig().BaseSleepAccelerator; // No fire or building detected, no sleep accelerator.
	}

	// Check for nearby objects in the config list, and apply the rest accelerator.
	void GetNearbyRestObjectAccelerator()
	{
		if (GetZenSleepConfig().RestObjects.Count() == 0)
		{
			return;
		}
		
		array<Object> nearest_objects = new array<Object>;
		array<CargoBase> proxy_cargos = new array<CargoBase>;

		// Get all objects in a 3 meter radius from our position
		GetGame().GetObjectsAtPosition(this.GetPosition(), 3.0, nearest_objects, proxy_cargos);
		RestObject ro = NULL;
		bool litFireFound = false;

		// TODO: Find out why large tents and random other objects don't work sometimes???
		for (int i = 0; i < nearest_objects.Count(); i++)
		{
			Object nearest_object = nearest_objects.Get(i);

			if (nearest_object != this) // If found object is not our player...
			{
				FireplaceBase fire = FireplaceBase.Cast(nearest_object);

				if (fire) // If this object is a fireplace, check if it's burning
				{
					if (fire.IsBurning())
						litFireFound = true;
				}
				else
				{
					// If the distance to the rest object is > 1 meter and it's not a fire, ignore it as we're not inside or ontop of it.
					if (vector.Distance(nearest_object.GetPosition(), this.GetPosition()) > 1.0)
						continue;
				}

				string nearbyObjectType = nearest_object.GetType();

				if (nearbyObjectType.Contains("ClutterCutter")) // Ignore tent clutter-cutters that just remove grass as some tent cluttercutters contain the same classname as the tent itself
				{
					continue;
				}

				ro = GetZenSleepConfig().GetRestObject(nearbyObjectType);
				if (ro.ObjectType != "") // We found a rest object listed in the json config
				{
					if (nearbyObjectType.Contains("Tent") || nearbyObjectType.Contains("Shelter"))
					{
						TentBase tent = TentBase.Cast(nearest_object);
						if (tent && tent.GetState() != TentBase.PITCHED)
						{
							continue; // Skip this item because it's a tent or shelter, and it isn't pitched
						}
					}

					if (ro.SleepAcceleratorPercent != 0 && ro.SleepAcceleratorPercent > m_RestObjectAccelerator)
					{	// Apply highest sleep accelerator if multiple objects are found
						m_RestObjectAccelerator = ro.SleepAcceleratorPercent / 100;
						m_RestObjectInfluenza = ro.Influenza;
						if (Zen_IsNightTime())
						{
							m_RestObjectMaxSleep = ro.MaxRestNight;
						}
						else
						{
							m_RestObjectMaxSleep = ro.MaxRestDay;
						}

						if (GetZenSleepConfig().DebugOn)
						{
							ZS_SendMessage("Found nearby rest object: " + ro.ObjectType + " - MaxRestDay=" + ro.MaxRestDay + "% SleepAccelerator=" + ro.SleepAcceleratorPercent + "%");
						}
					}
				}
			}
		}

		// This ensures that if multiple fires are found we detect if one is burning
		m_FireNearby = litFireFound;

		// This is used for tracking how many times we've scanned the nearby area for rest objects / fires
		m_RestObjectChecks++;
	}

	// This method handles all of our rest checks
	void PlayerRestCheck(float deltaTime)
	{
		if (!m_IsAllowedToSleep)
			return;

		m_PlayerRestTick += deltaTime;
		m_RestObjectTick += deltaTime;

		if (GetGame().IsDedicatedServer()) // Server-side update
		{
			m_Tiredness = GetSingleAgentCount(ZenSleep_Agents.TIREDNESS);

			if (m_PlayerRestTick < GetZenSleepConfig().RestUpdateTick)
				return;

			float restTick = m_PlayerRestTick;
			m_PlayerRestTick = 0;
			
			if (IsUnconscious())
				m_FallUnconsciousFromTiredness = false;

			// If player is not asleep, stop here.
			if (!IsPlayerSleeping())
			{
				m_TimeAsleep = 0;
				return;
			}

			m_TimeAsleep += GetZenSleepConfig().RestUpdateTick;

			// If player has not been asleep for at least 5 seconds, stop here
			if (m_TimeAsleep < 5)
				return;

			// Calculate our rest as a percentage
			int restPercent = Math.Round(100 - (float)m_Tiredness / MAX_TIREDNESS * 100);

			// Check if player is sleeping near a fire and/or rest object and apply any accelerator modifiers
			if (m_RestObjectTick >= GetZenSleepConfig().RestUpdateTick * 4)
			{
				GetNearbyRestObjectAccelerator();
				m_RestObjectTick = 0;
			}

			bool asleepFor30Secs = m_TimeAsleep >= 30;
			float restAccelerator = 1.0;
			if (asleepFor30Secs)
				restAccelerator = GetFireSleepAccelerator() + m_RestObjectAccelerator;

			// If we've been asleep for at least 30 seconds, check if we should play a random sleep sound and increase sleep accelerator
			if (asleepFor30Secs)
			{
				// Regen blood & health faster while asleep (if it's turned on in config)
				RegenBlood();
				RegenHealth();

				if (GetZenSleepConfig().WetnessCancelsFireAccelerator && GetStatWet().Get() <= 0) // If player is wet, don't sleep any faster
				{
					m_SleepAccumulatorModifier += GetZenSleepConfig().AsleepAccelerator;
					restAccelerator += m_SleepAccumulatorModifier;
				}

				if (Math.RandomFloat01() <= GetZenSleepConfig().SleepSoundPercentChance)
				{
					MakeSleepSound();
				}

				// If there is no heat source nearby or adequately warm rest object (or heat buff), then inject the player with influenza randomly
				if (!m_FireNearby && m_RestObjectInfluenza && !m_HasHeatBuffer)
				{
					float randFlu = (float)Math.RandomIntInclusive(0, GetZenSleepConfig().InfluenzaInjectNoFire);

					if (Zen_IsNightTime())
					{
						randFlu *= GetZenSleepConfig().InfluenzaMultiplierNightNoFire; // If it's night time increase influenza infection by 50%
					}

					if (GetStatWet().Get() == GetZenSleepConfig().WetnessCancelsFireAccelerator)
					{
						randFlu *= GetZenSleepConfig().InfluenzaMultiplierWetNoFire; // If we're wet, increase influenza infection by 100%
					}

					InsertAgent(eAgents.INFLUENZA, randFlu);
					if (GetZenSleepConfig().DebugOn)
					{
						ZS_SendMessage("Injected with influenza for no fire: " + randFlu + "/" + GetZenSleepConfig().InfluenzaInjectNoFire + " (" + GetSingleAgentCount(eAgents.INFLUENZA) + "/1000)");
					}
				}
			}
			
			// Sync our client-side variables
			ZenSleep_SyncState();

			// If text notifications are enabled then periodically tell the player their sleep %
			if (GetZenSleepConfig().TextNotificationOn && !m_IsUnconscious && !GetZenSleepConfig().DebugOn)
			{
				m_TirednessUpdateCountdown--;
				if (m_TirednessUpdateCountdown <= 0) // Send text update every 5 rest ticks
				{
					m_TirednessUpdateCountdown = GetZenSleepConfig().TextNotificationFrequency;
					if (restPercent != m_prevTirednessPercent)
					{
						m_prevTirednessPercent = restPercent;
						string warmCold = GetZenSleepConfig().Str_RestUpdate1; // and I'm cold...
						if (!m_FireNearby && m_HasHeatBuffer) // No fire, but we have the heat buff
						{
							warmCold = GetZenSleepConfig().Str_RestUpdate2; // and I'm getting cold...
						} 
						else
						if (m_FireNearby) // Yes fire
						{
							warmCold = GetZenSleepConfig().Str_RestUpdate3; // and I'm warm
						}
						if (GetStatWet().Get() == GetZenSleepConfig().WetnessCancelsFireAccelerator)
						{
							warmCold = GetZenSleepConfig().Str_RestUpdate5; // and my clothes are wet.
						}

						// Yes fire inside + heat buff
						if (GetFireSleepAccelerator() > (((float)GetZenSleepConfig().OutsideFireAcceleratorPercent / 100.0) + 1.0) && m_SleepingInside && m_HasHeatBuffer && m_FireNearby)
						{
							warmCold = GetZenSleepConfig().Str_RestUpdate4; // and I'm comfortably warm
						}

						ZS_SendMessage(GetZenSleepConfig().Str_RestUpdate + " " + restPercent.ToString() + "% " + warmCold); // My rest level is x% and I'm warm/cold
					}
				}
			}

			// Debug message for dev purposes
			if (GetZenSleepConfig().DebugOn)
			{
				string debugStr = "HeatSource=" + m_FireNearby + " IsNight=" + Zen_IsNightTime() + " Inside=" + m_SleepingInside + " RestAcc=" + restAccelerator + " RestAccum=" + m_SleepAccumulatorModifier + " Wet=" + GetStatWet().Get();
				ZS_SendMessage("My rest level is " + restPercent.ToString() + "% - " + debugStr);
			}

			if (!m_IsUnconscious) // Don't send sleep text updates if we're unconscious
			{
				float maxRest = 100.0; // Limit our rest to this variable value based on various factors
				string msg = ""; // Text message to update the player with

				// Player is sleeping at night time
				if (Zen_IsNightTime())
				{
					// And player is wet and a max rest limit is set for wet players
					if (GetZenSleepConfig().MaxRestWhenWetNight < 100 && GetStatWet().Get() > 0)
					{
						if (restPercent >= GetZenSleepConfig().MaxRestWhenWetNight)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								msg = GetZenSleepConfig().Str_CantSleep5; // "I don't think I can sleep any longer, my clothes are wet and I'm uncomfortable..."
								ZS_SendMessage(msg);
							}

							m_CantSleep = true;
							return;
						}
					}

					if (!m_FireNearby) // And there is no fire nearby
					{
						maxRest = Math.Max(GetZenSleepConfig().MaxRestNightNoFire, m_RestObjectMaxSleep);
						if (restPercent >= maxRest && m_RestObjectChecks >= 1)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								msg = GetZenSleepConfig().Str_CantSleep1; // "I don't think I can sleep any longer..."
								if (restPercent < 100.0)
									msg = GetZenSleepConfig().Str_CantSleep2; // "I don't think I can sleep any longer, I'm too cold..."
								ZS_SendMessage(msg);
							}

							m_CantSleep = true;
							return;
						}
					}
					else // And there is a fire nearby
					{
						maxRest = Math.Max(GetZenSleepConfig().MaxRestNightWithFire, m_RestObjectMaxSleep);
						if (restPercent >= maxRest && m_RestObjectChecks >= 1)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								msg = GetZenSleepConfig().Str_CantSleep1; // "I don't think I can sleep any longer..."
								if (restPercent < 100.0)
									msg = GetZenSleepConfig().Str_CantSleep2; // "I don't think I can sleep any longer, I'm too cold..."
								ZS_SendMessage(msg);
							}

							m_CantSleep = true;
							
							if (asleepFor30Secs)
								CheckSleepImmunityBoost(restPercent);

							return;
						}
					}
				}
				else // Check day time rest limits
				{
					// And player is wet and a max rest limit is set for wet players
					if (GetZenSleepConfig().MaxRestWhenWetDay < 100 && GetStatWet().Get() > 0)
					{
						if (restPercent >= GetZenSleepConfig().MaxRestWhenWetDay)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								msg = GetZenSleepConfig().Str_CantSleep5; // "I don't think I can sleep any longer, my clothes are wet and I'm uncomfortable..."
								ZS_SendMessage(msg);
							}

							m_CantSleep = true;
							return;
						}
					}

					if (!m_FireNearby) // And there is no fire nearby
					{
						maxRest = Math.Max(GetZenSleepConfig().MaxRestDayNoFire, m_RestObjectMaxSleep);
						if (restPercent >= maxRest && m_RestObjectChecks >= 1)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								ZS_SendMessage(GetZenSleepConfig().Str_CantSleep3); // "I don't think I can sleep any longer, it's too bright and I'm cold..."
							}

							m_CantSleep = true;
							return;
						}
					}
					else // And there is a fire nearby
					{
						maxRest = Math.Max(GetZenSleepConfig().MaxRestDayWithFire, m_RestObjectMaxSleep);
						if (restPercent >= maxRest && m_RestObjectChecks >= 1)
						{
							if (GetZenSleepConfig().TextNotificationOn)
							{
								ZS_SendMessage(GetZenSleepConfig().Str_CantSleep4); // "I don't think I can sleep any longer, it's too bright..."
							}

							m_CantSleep = true;

							if (asleepFor30Secs)
								CheckSleepImmunityBoost(restPercent);

							return;
						}
					}
				}
			}

			// Increase rest (decrease tiredness) while we are not at max rest
			m_AccumulatedRest += (REST_GAIN_PER_SEC * restTick);
			int rest = m_AccumulatedRest * restTick * restAccelerator;
			if (rest > m_Tiredness)
			{
				rest = m_Tiredness;
			}

			// Recover rest energy based on calculated modifiers
			InsertAgent(ZenSleep_Agents.TIREDNESS, rest * -1);
			m_Tiredness -= rest;
		}
		else // Client-side update
		{
			if (m_ModulePlayerStatus)
			{
				m_ModulePlayerStatus.SetTiredness(MAX_TIREDNESS - m_Tiredness, MAX_TIREDNESS);
			}

			// Check if the player's sleep state has changed
			if (m_IsSleeping != m_WasSleeping)
			{
				m_WasSleeping = m_IsSleeping;

				if (m_IsSleeping && m_YawnTime <= 0)
				{
					m_YawnTime = 0.01; // Close the player's eyes
				}

				// Apply custom attentuation (TODO: Make the onset gradual, and during deep sleep increase the wackyness off audio)
				if (m_IsSleeping && m_AttenuateSound)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedSleepSoundAttenuation, Math.RandomFloatInclusive(20000, 30000), false);
				}
				
				// Cancel attenuation
				if (!m_IsSleeping && m_AttenuateSound)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DelayedSleepSoundAttenuation);
					SetMasterAttenuation("");
				}
			}

			// Check if we need to do the yawn screen effect
			PlayerYawnEffectCheck();
		}
	}

	// Delay onset of sound attenuation
	void DelayedSleepSoundAttenuation()
	{
		if (m_IsSleeping)
		{
			SetMasterAttenuation("ZenSleepAttenuation");
		}
	}

	// Checks if the player has achieved the minimum rest level for a sleep immunity boost
	void CheckSleepImmunityBoost(float restPercent)
	{
		// Activate immunity boost if enabled in config and player has reached the necessary rest level to achieve it
		if (GetZenSleepConfig().ImmunityBoostAtRestLevel != 0 && restPercent >= GetZenSleepConfig().ImmunityBoostAtRestLevel && !m_SleepImmunityActivated)
		{
			if (GetModifiersManager() && GetModifiersManager().IsModifierActive(eModifiers.MDF_IMMUNITYBOOST))
				return;

			GetModifiersManager().ActivateModifier(eModifiers.MDF_IMMUNITYBOOST);
			m_SleepImmunityActivated = true;
		}
	}

	// Checks if it is currently night time (adapted from PvZmoD night time checker code - credit to Liven! (https://steamcommunity.com/sharedfiles/filedetails/?id=2051775667)
	bool Zen_IsNightTime()
	{
		// If config for night-time override is not set, just use the default DayZ night time check
		if (GetZenSleepConfig().NightTimeStartHour == 0 && GetZenSleepConfig().NightTimeStartMin == 0 && GetZenSleepConfig().NightTimeEndHour == 0 && GetZenSleepConfig().NightTimeEndMin == 0)
		{
			return GetGame().GetWorld().IsNight();
		}

		bool isNight = false;
		int day, month, year, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		if (GetZenSleepConfig().NightTimeStartHour > GetZenSleepConfig().NightTimeEndHour || (GetZenSleepConfig().NightTimeStartHour == GetZenSleepConfig().NightTimeEndHour && GetZenSleepConfig().NightTimeStartMin < GetZenSleepConfig().NightTimeEndMin))
		{
			if (hour > GetZenSleepConfig().NightTimeStartHour || (hour == GetZenSleepConfig().NightTimeStartHour && minute >= GetZenSleepConfig().NightTimeStartMin))
			{
				isNight = true;
			}
			else 
			if (hour < GetZenSleepConfig().NightTimeEndHour || (hour == GetZenSleepConfig().NightTimeEndHour && minute <= GetZenSleepConfig().NightTimeEndMin))
			{
				isNight = true;
			}
		}
		else 
		if (GetZenSleepConfig().NightTimeStartHour < GetZenSleepConfig().NightTimeEndHour || (GetZenSleepConfig().NightTimeStartHour == GetZenSleepConfig().NightTimeEndHour && GetZenSleepConfig().NightTimeStartMin < GetZenSleepConfig().NightTimeEndMin))
		{
			if (hour > GetZenSleepConfig().NightTimeStartHour || (hour == GetZenSleepConfig().NightTimeStartHour && minute >= GetZenSleepConfig().NightTimeStartMin))
			{
				if (hour < GetZenSleepConfig().NightTimeEndHour || (hour == GetZenSleepConfig().NightTimeEndHour && minute <= GetZenSleepConfig().NightTimeEndMin))
				{
					isNight = true;
				}
			}
		}

		return isNight;
	}

	// Adds/removes energy based on consumption item
	override bool Consume(ItemBase source, float amount, EConsumeType consume_type)
	{
		if (source)
		{
			EnergyDrink drink = GetZenSleepConfig().GetEnergyDrink(source.GetType());
			if (drink.ItemType != "")
			{
				float percent = amount / source.GetQuantityMax();
				float replenish = (float)MAX_TIREDNESS * (((float)drink.EnergyGained * percent) / 100);
				InsertAgent(ZenSleep_Agents.TIREDNESS, replenish);

				if (GetZenSleepConfig().DebugOn)
				{
					ZS_SendMessage("Giving energy: " + replenish);
				}
			}
		}
		
		return super.Consume(source, amount, consume_type);
	}

	// Cancels the sleep state
	void CancelSleep()
	{
		if (!m_IsUnconscious && m_IsSleeping)
		{
			m_IsSleeping = false;
			m_CancelSleep = true;
			ZenSleep_SyncState();
		}
	}

	// Player tick - check sleep status
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		PlayerRestCheck(deltaTime);
	}

	// Regen blood faster (if enabled in config)
	void RegenBlood()
	{
		if (!IsPlayerSleeping() || GetZenSleepConfig().BloodRegenAsleep <= 0)
			return;

		AddHealth("", "Blood", GetZenSleepConfig().BloodRegenAsleep);
	}

	// Regen health faster (if enabled in config)
	void RegenHealth()
	{
		if (!IsPlayerSleeping() || GetZenSleepConfig().HealthRegenAsleep <= 0)
			return;

		AddHealth("GlobalHealth", "Health", GetZenSleepConfig().HealthRegenAsleep);
	}

	// Handles game commands
	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);

		if (GetGame().IsDedicatedServer() && GetGame().IsMultiplayer())
		{
			return;
		}

		// If the player is unconscious then disable the black screen effect
		if (m_IsUnconscious)
		{
			if (m_YawnTime > 0)
			{
				SetTirednessVignette(0);
				m_YawnTime = 0;
			}

			return;
		}

		// If the current yawn time is 0 there's no need to update the black screen effect
		if (m_YawnTime == 0)
		{
			SetTirednessVignette(0);
			return;
		}

		// If the player is not asleep then update the yawn time
		if (!m_IsSleeping || m_YawnTime < YAWN_TIME_2 - pDt)
			m_YawnTime += pDt;

		// The following code handles the black screen animated effect
		if (m_YawnTime < YAWN_TIME_1)
		{
			SetTirednessVignette(Math.Lerp(0, 2, m_YawnTime / YAWN_TIME_1));
		}
		else if (m_YawnTime < YAWN_TIME_2)
		{
			// Wait in darkness or fall asleep
			if (m_FallUnconsciousFromTiredness)
			{
				// Notify server that player is allowed to fall asleep
				SendAllowUnconsciousFromTiredness();
			}
		}
		else if (m_YawnTime < YAWN_TIME_3)
		{
			SetTirednessVignette(Math.Lerp(2, 0, (m_YawnTime - YAWN_TIME_2) / (YAWN_TIME_3 - YAWN_TIME_2)));
		}
		else
		{
			SetTirednessVignette(0);
			m_YawnTime = 0;
		}
	}

	// Handles remote procedure calls
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch(rpc_type)
		{
			// A bit ugly: We are (ab)using the shock RPC which is unused in vanilla since 1.15 so it should be safe
			// We check m_FallUnconsciousFromTiredness on server so this is not exploitable from client
			case ERPCs.RPC_SHOCK:
				if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
					break;

				// Server-side
				if (m_FallUnconsciousFromTiredness)
				{
					Zen_SetPlayerUncon();
				}

				break;
		}
	}

	// Sets the player unconscious
	void Zen_SetPlayerUncon()
	{
		// We are allowed to fall asleep from unconsciousness
		ResetSleep();
		CheckIfPlayerIsInside();
		m_FallUnconsciousFromTiredness = false;
		GiveShock(-100);

		// Prevent 'fake' sleep when player didn't receive shock damage, e.g. because 'godmode' is on
		if (GetHealth("", "Shock") < 1)
			m_IsUnconsciousFromTiredness = true;

		ZenSleep_SetState();
	}

	// Detects when the player is no longer uncon
	override void OnUnconsciousStop(int pCurrentCommandID)
	{
		m_IsUnconsciousFromTiredness = false;
		ZenSleep_SetState();
		super.OnUnconsciousStop(pCurrentCommandID);

		if (m_TirednessVignetteValue) // Disable yawn black screen effect if it's active
			SetTirednessVignette(0);
	}

	// Triggers when shock is given
	override void SpawnShockEffect(float intensity_max)
	{
		super.SpawnShockEffect(intensity_max);

		if (m_TirednessVignetteValue) // Disable yawn black screen effect if it's active
			SetTirednessVignette(0);
	}

	// Custom method to set the player unconscious from being too tired
	void SetUnconsciousFromTiredness()
	{
		m_FallUnconsciousFromTiredness = true;
		ZenSleep_SyncState();
	}

	// Custom method to set the player unconscious from being too tired
	void SendAllowUnconsciousFromTiredness()
	{
		// A bit ugly: We are (ab)using the shock RPC which is unused in vanilla since 1.15 so it should be safe
		// We check m_FallUnconsciousFromTiredness on server so this is not exploitable from client
		Param1<float> damage = new Param1<float>(0);
		GetGame().RPCSingleParam(this, ERPCs.RPC_SHOCK, damage, true, GetIdentity());
	}

	// Triggers black screen blink when yawning/falling unconscious
	bool m_StoppedSleepEffect = false;
	void SetTirednessVignette(float val)
	{
		if (!m_IsAllowedToSleep)
			return;

		m_TirednessVignetteValue = val;

		if (val > 0)
		{
			m_StoppedSleepEffect = false;
			float blur = val * 0.2;
			float vignette = val;
			float color_overlay_factor = val * 0.16;

			if (m_OnlyBlurScreen && IsPlayerSleeping()) // Overrides black screen effect if blur config setting is turned on
			{
				vignette = 0;
				color_overlay_factor = 0;
			}

			auto requester = PPERequester_SleepEffect.Cast(PPERequesterBank.GetRequester(PPERequester_SleepEffect));
			requester.ZS_SetEffectValues(blur, vignette, color_overlay_factor);
		}
		else
		{
			if (!m_StoppedSleepEffect)
			{
				PPERequesterBank.GetRequester(PPERequester_SleepEffect).Stop();
				m_StoppedSleepEffect = true;
			}
		}
	}
}
