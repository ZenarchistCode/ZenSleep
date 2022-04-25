// Hide item UI in hands while sleeping?
modded class ItemActionsWidget extends ScriptedWidgetEventHandler
{
	override void Update()
	{
		super.Update();

		if (m_Player && m_Player.m_IsSleeping && m_Player.m_HideHudWhileSleeping && m_Root && m_Player.IsAlive())
		{
			m_Root.Show(false);
		}
	}
}

// Hide nearby object UI while sleeping?
modded class ActionTargetsCursor extends ScriptedWidgetEventHandler
{
	override void Update()
	{
		super.Update();

		if (m_Player && m_Player.m_IsSleeping && m_Player.m_HideHudWhileSleeping && m_Root && m_Player.IsAlive())
		{
			m_Hidden = true;
		}
	}
}