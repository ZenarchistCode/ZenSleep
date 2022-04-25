// This class handles the tiredness UI stuff
modded class IngameHud
{
	// Tiredness HUD
	ProgressBarWidget m_Tiredness = NULL;
	Widget m_TirednessIconPanel = NULL;
	Widget m_TirednessBarPanel = NULL;
	Widget m_SleepModLayout = NULL;
	// Vanilla hud items (referenced for hiding them while sleeping)
	Widget m_ActionsPanel = NULL;
	Widget m_Item = null;

	Widget m_ItemActions = NULL;
	Widget m_ActionTarget = NULL;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		// Tiredness HUD
		Class.CastTo(m_SleepModLayout, GetGame().GetWorkspace().CreateWidgets("ZenSleep/data/layout/ZenSleepLayout.layout"));
		m_Tiredness = m_SleepModLayout.FindAnyWidget("SleepBar");
		m_TirednessIconPanel = m_SleepModLayout.FindAnyWidget("SleepIconPanel");
		m_TirednessBarPanel = m_SleepModLayout.FindAnyWidget("SleepBarPanel");
		// Vanilla hud items (referenced for hiding them while sleeping)
		m_ItemActions = m_HudPanelWidget.FindAnyWidget("ItemActions");
		m_ActionTarget = m_HudPanelWidget.FindAnyWidget("ActionTargetsCursorWidget");

		if (!m_Tiredness || !m_TirednessIconPanel || !m_TirednessBarPanel)
		{
			Print("[ZenSleepHUD] Failed to load UI layout!");
			return;
		}

		m_TirednessIconPanel.Show(false);
		m_TirednessBarPanel.Show(false);
	}

	override void SetTiredness(int value , int range)
	{
		if (!m_Tiredness)
		{
			return;
		}

		m_Tiredness.SetCurrent((value / range ) * 100);
		SetHudVisibility();
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if (!m_Tiredness || !m_TirednessIconPanel || !m_TirednessBarPanel)
		{ // Something has gone wrong loading UI - stop here.
			return;
		}

		SetHudVisibility();
	}

	void SetHudVisibility()
	{
		// Check if only show while tabbed is enabled from server config and if our inventory is open
		bool overrideSleep = true;
		PlayerBase player = GetGame().GetPlayer();
		if (!player)
		{
			if (m_TirednessIconPanel && m_TirednessBarPanel)
			{
				m_TirednessIconPanel.Show(false);
				m_TirednessBarPanel.Show(false);
			}
			
			return;
		}

		if (player.m_OnlyShowSleepOnInventory)
		{
			overrideSleep = player.m_IsSleeping || m_HudInventory;
		}

		if (player.m_OnlyShowSleepAbovePercent != 0)
		{
			if (((float)(player.m_Tiredness / player.MAX_TIREDNESS) * 100.0) < (float)player.m_OnlyShowSleepAbovePercent)
			{
				overrideSleep = player.m_IsSleeping || m_HudInventory;
			}
		}

		// Check if this hud should be visible with the rest of the player hud
		bool shouldShow = (((!m_HudHidePlayer && !m_HudHideUI && m_HudState) || m_HudInventory);

		// Set sleep hud visibility
		m_TirednessIconPanel.Show(shouldShow && overrideSleep);
		m_TirednessBarPanel.Show(shouldShow && overrideSleep);

		// Hide hud if config is turned on
		if (player && player.m_HideHudWhileSleeping)
		{
			if (player.m_IsSleeping)
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