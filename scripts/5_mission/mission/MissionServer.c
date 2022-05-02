modded class MissionServer
{
	override void OnInit()
	{
		GetZenSleepConfig();
		super.OnInit();
		Print("[ZenSleep] OnInit - Loaded config successfully.");
	}
}