// This class handles the ZenSleep tiredness UI stuff
modded class IngameHud
{
	// Tiredness HUD
	ProgressBarWidget m_Tiredness = NULL;
	Widget m_TirednessIconPanel = NULL;
	Widget m_TirednessBarPanel = NULL;
	Widget m_SleepModLayout = NULL;
	Widget m_SleepModHudPanel = NULL;
	float m_SleepHudX = 0.855; // Tiredness HUD location on the GUI - can be overridden by server config
	float m_SleepHudY = 0.02; // Tiredness HUD location on the GUI - can be overridden by server config

	// Initialize the relevant widgets for this mod
	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		// Tiredness HUD
		Class.CastTo(m_SleepModLayout, GetGame().GetWorkspace().CreateWidgets("ZenSleep/data/layout/ZenSleepLayout.layout"));
		ProgressBarWidget.CastTo(m_Tiredness, m_SleepModLayout.FindAnyWidget("SleepBar"));
		m_TirednessIconPanel = m_SleepModLayout.FindAnyWidget("SleepIconPanel");
		m_TirednessBarPanel = m_SleepModLayout.FindAnyWidget("SleepBarPanel");
		m_SleepModHudPanel = m_SleepModLayout.FindAnyWidget("HudPanel_Tiredness");

		if (!m_Tiredness || !m_TirednessIconPanel || !m_TirednessBarPanel || !m_SleepModHudPanel)
		{
			Print("[ZenSleepHUD] Failed to load UI layout!");
			return;
		}

		m_SleepModHudPanel.GetPos(m_SleepHudX, m_SleepHudY);
		m_SleepModHudPanel.Show(false);
	}

	// Sets the tiredness of our meter
	override void SetTiredness(int value, int range)
	{
		if (!m_Tiredness)
		{
			return;
		}

		m_Tiredness.SetCurrent((value / range) * 100);
		SetHudVisibility();
	}

	// Refreshes entire hud visibility - we also check our tiredness meter visibility here.
	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if (!m_Tiredness || !m_TirednessIconPanel || !m_TirednessBarPanel)
		{ // Something has gone wrong loading UI - stop here.
			return;
		}

		SetHudVisibility();
	}

	// Updates the tiredness hud visibility based on the relevant conditions
	void SetHudVisibility()
	{
		// Check if only show while tabbed is enabled from server config and if our inventory is open
		bool overrideSleep = true;
		PlayerBase player;
		PlayerBase.CastTo(player, GetGame().GetPlayer());
		if (!player || !player.IsAlive())
		{
			if (m_TirednessIconPanel && m_TirednessBarPanel)
			{
				m_SleepModHudPanel.Show(false);
			}

			return;
		}

		// Check if we need to move the position of the tiredness meter based on server config
		if (m_SleepModHudPanel)
		{
			if (m_SleepHudX != player.m_TirednessHudX || m_SleepHudY != player.m_TirednessHudY)
			{
				m_SleepModHudPanel.SetPos(player.m_TirednessHudX, player.m_TirednessHudY);
				m_SleepModHudPanel.GetPos(m_SleepHudX, m_SleepHudY);
			}
		}

		// If only show on inventory is turned on, check if we're sleeping or have inventory open
		if (player.m_OnlyShowSleepOnInventory)
		{
			overrideSleep = player.m_IsSleeping || m_HudInventory;
		}

		// If we have only show above certain tiredness percent on, check the threshold
		if (player.m_OnlyShowSleepAbovePercent != 0)
		{
			if (player.GetTiredPercent() < (float)player.m_OnlyShowSleepAbovePercent)
			{
				overrideSleep = player.m_IsSleeping || m_HudInventory;
			}
		}

		// Check if this hud should be visible with the rest of the player hud
		bool shouldShow = (((!m_HudHidePlayer && !m_HudHideUI && m_HudState) || m_HudInventory);

		// Set sleep hud visibility
		m_SleepModHudPanel.Show(shouldShow && overrideSleep);

		// Hide hud if config is turned on
		if (player.m_HideHudWhileSleeping)
		{
			if (player.m_IsSleeping && player.IsAlive())
			{
				m_Stamina.Show(false);
				m_StaminaBackground.Show(false);
				m_Presence.Show(false);
				m_StanceStand.Show(false);
				m_StanceStandWalk.Show(false);
				m_StanceCrouch.Show(false);
				m_StanceProne.Show(true);
				m_StanceCar.Show(false);
			}
			else
			{
				m_Stamina.Show(shouldShow);
				m_StaminaBackground.Show(shouldShow);
				m_Presence.Show(shouldShow);
			}
		}
	}
}