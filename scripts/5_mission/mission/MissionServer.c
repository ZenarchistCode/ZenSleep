modded class MissionServer
{
	override void OnInit()
	{
		Print("[ZenSleep] OnInit - Loading config");
		GetZenSleepConfig();
		super.OnInit();
	}
}