NOTE: THIS MOD IS DEPRECATED! I MADE A TOTAL RE-WRITE VERSION 2 OF THIS MOD AVAILABLE HERE: https://github.com/ZenarchistCode/ZenSleepV2/

What Is This?

This mod adds an immersive sleeping feature to the game.

It is heavily based on and inspired by Lucian's old Medical Attention mod, so most of the credit goes to him.

It has been designed to be as intuitive and immersive as possible, but keep reading to learn about all the various features and behaviors of this mod.

Player Guide:

The following information might vary depending on the server settings you're playing on, but with the default settings the mod will behave like this:
You have approximately 4 hours of full energy on a new character.
If your "tiredness" meter falls below 20%, your character will start yawning randomly. 

These yawns are audible by other players like the vanilla sickness sounds and can give your position away, so stay rested!

If you allow your tiredness meter to fall below 10%, then you run the risk of randomly falling unconscious from tiredness.

If you hit zero then your character will fall asleep on the spot.

You can recover your rest using the "Lie Down" emote (NOT the "Lie Back" emote!).

The longer you let your player stay sleeping, the faster your energy recovers.

Sleeping next to a fire will not only protect you from potentially catching influenza, but will help you rest faster and recover more energy. You must be within 3 meters of the fire to feel its warmth. The heat buffer will also protect you from influenza.

Sleeping inside a building or tent will also speed up your rest time.

During the day you can only recover 50% of your energy sleeping without a fire, or 75% with a fire.
During the night you can only recover 80% of your energy sleeping without a fire, or 100% with a fire.
Sleeping inside buildings and tents (especially with a fire lit nearby) is the best way to recover your energy quickly.

If you are wet you might not be able to reach the maximum rest level, so make sure to dry your clothes out before sleeping.

Certain drinks can help you recover your energy (soda cans). Certain meds will make you more sleepy (PO-X injectors especially, but also morphine & epinephrine).

There are also some syringes you might find which you can use to keep yourself awake, or put other survivors to sleep...

You can also fall asleep while driving (unless the server you play on has turned that option off), so make sure to utilize rest stops often on long drives.

All of these features are adjustable on the server-side config so your experience may vary, make sure to check with your server operator for more info on how they set their server up with this mod.





Server Owner Guide:

Main Sleep Mod Features (All Configurable in ZenSleepConfig.json):
- Sleep meter (configurable: always show, only show on inventory screen, or only show above certain tiredness %)
- Use "Lie Down" emote to start sleeping and regenerating tiredness energy
- Random yawn sound when player gets tired (male & female sounds - audible to nearby players)
- Chance to randomly fall unconscious when the player gets too tired
- Random sleep sounds when player is resting (male & female sounds - audible to nearby players)

Max Rest While Sleeping:
- 50% during the day with no fire
- 75% during the day with a fire
- 80% at night with no fire
- 100% at night with a fire
- Beware - if you sleep without a fire you might catch influenza (configurable)
- Being wet affects your max rest level and influenza as well.

Other Notes:
The mod will detect fires within 3m of the player, and if the fire is lit then it counts as a heat source.

If you have issues with fires on certain maps or sleeping next to modded heat sources, try turning "HeatBuffCountsAsFire=true" on.

Set DebugOn=1 in the config to get detailed messages explaining what the mod is doing and what values it is detecting.

Rest Accelerators:
Depending on where you are sleeping will affect how quickly you regenerate energy. The mod can tell when you are sleeping under a roof and if you have a fire lit nearby.
- Asleep Outside With No Fire = 1x
- Asleep Outside With A Fire = 5% faster
- Asleep Inside House or Building With No Fire = 10% faster
- Asleep Inside House or Building With A Fire = 20% faster

Energy Drinks / Consumption Items:
You can add your own energy drinks/consumption items in the json config. The default ones are:
- Cola: +25% energy
- Pipsi: +20% energy
- Fronta: + 15% energy
- Spite: +10% energy
- Kvass: +5% energy

The mod checks for energy drink items and rest object types with a "contains" check and is not case-sensitive. A negative value increases energy, a positive value decreases energy. So in other words, if you want to make all soda cans restore 20% of your energy you can just set the JSON config to have this entry:

{
    "ItemType": "sodacan",
    "EnergyGained": -20
}

Some meds will also negatively affect your energy:
- AntiChemInjector (PO-X): -100% energy
- Morphine: -20% energy
- Epinephrine: -15% energy

Anaesthetic & Stimulant Syringes:
This mod also comes with 4 custom items:
- Anaesthetic Syringe (and empty version)
- Stimulant Syringe (and empty version)

The anaesthetic syringe puts the player to sleep (can be used on self or others).
The stimulant syringe maxes out your rest energy (can be used on self or others).

These syringes are optional and can be disabled by simply not adding the types to your types.xml.

(If you want to enable them spawning, just copy the text inside the provided types.xml from the mod folder and merge it into your server's types.xml)

Rest Objects:
You can add specific objects in the json config that affect how the player sleeps.
Note: This feature is still in development and doesn't work 100% of the time on some objects.
- MediumTent: Max rest during the day with no fire is 80%, you sleep 10% faster and no chance of influenza regardless of fire status.

Default Mod Settings That Can Be Tweaked/Enabled/Disabled:

######################################################################
MAIN CONFIG
######################################################################
[MaxWakeTime=240] Maximum time the player can stay awake before they are forced uncon (in minutes - 4 hours by default)
[AllowInventoryWhileSleep=false] Allows the player to access their inventory while sleeping (they can only move things around and can't put items into their hands)
[CanUnconInVehicle=true] Whether or not players can fall asleep while driving
[SleepBlackScreen=1] Screen turns black or blurry when sleeping (0 = off, 1 = black, 2 = blurry)
[TextNotificationOn=true] Text notifications to update the player's sleep status
[TextNotificationFrequency=5] How often to send text updates while sleeping (every 5 rest ticks or 15 secs by default).
[HideHudWhileSleeping=true] Whether or not to hide parts of the HUD while sleeping (item in hands, stamina meter etc)
[OnlyShowSleepOnInventory=false] Only show tiredness meter when tabbed and in the inventory
[OnlyShowSleepAbovePercent=off] Only show tiredness meter when tiredness is over a certain % (0-100)
######################################################################
RANDOM CHANCE CONFIG
######################################################################
[SleepSoundPercentChance=8%] Percent chance to play a sleeping sound while the player is tired (0 to disable)
[YawnPercentStart=20%] The tiredness % level when random yawning can start to happen (to notify player they need to sleep soon)
[YawnPercentChance=6.5%] The base chance of playing a yawn sound when tiredness is over YawnPercentStart (slowly increases the more tired they are)
[UnconPercentStart=10%] The tiredness % level when the player can start to randomly fall unconscious
[UnconPercentChance=1%] The base chance of falling uncon when tiredness is over UnconPercentStart (slowly increases the more tired they are)
######################################################################
INFLUENZA CONFIG
######################################################################
[InfluenzaInjectNoFire=5] How much influenza to inject the player with when sleeping without a fire (random between 0 and this value - 1000 = max influenza)
[InfluenzaMultiplierNightNoFire=1.5] Multiplies InfluenzaInjectNoFire value when sleeping at night without a fire
[InfluenzaMultiplierWetNoFire] Multiplies InfluenzaInjectNoFire value when sleeping at night without a fire and with wet clothes
######################################################################
MAX REST CONFIG
######################################################################
[MaxRestDayNoFire=50%] Maximum achievable rest when sleeping during the day with no fire
[MaxRestDayWithFire=75%] Maximum achievable rest when sleeping during the day with a fire
[MaxRestNightNoFire=80%] Maximum achievable rest when sleeping during the night with no fire
[MaxRestNightWithFire=100%] Maximum achievable rest % when sleeping during the night with a fire
[MaxRestWhenWetDay=40] Maximum achievable rest % when sleeping with wet clothes during the day (regardless of fire)
[MaxRestWhenWetNight=75] Maximum achievable rest % when sleeping with wet clothes during the night (regardless of fire)
######################################################################
SLEEP ACCELERATOR CONFIG
######################################################################
[HeatBuffCountsAsFire=false] If enabled then having the heat buff is equivalent to sleeping next to a fire and will apply sleep accelerators.
[WetnessCancelsFireAccelerator=true] If player has wet clothes, don't give them any rest buffs (set to 0 to disable)
[BaseSleepAccelerator=1.0] How fast the player sleeps if no fire or rest object is detected nearby (1x speed by default)
[AsleepAccelerator=0.01%] Energy recovery speed increases by this value every few seconds while sleeping so the longer you sleep the faster you recover
[OutsideFireAcceleratorPercent=5%] You recover energy 5% faster when sleeping near a fire outside
[InsideNoFireAcceleratorPercent=10%] You recover energy 10% faster when sleeping inside a building with no fire nearby
[InsideFireAcceleratorPercent=25%] You recover energy 25% faster when sleeping inside a building with a fire nearby
######################################################################
NIGHT TIME CONFIG
######################################################################
[NightTimeStartHour=0] Use this to override the ingame night time detection (24 hour)
[NightTimeStartMin=0] Use this to override the ingame night time detection (0-59 mins)
[NightTimeEndHour=0] Use this to override the ingame night time detection (24 hour)
[NightTimeEndMin=0] Use this to override the ingame night time detection (0-59 mins)
######################################################################
ENERGY DRINKS & REST OBJECTS CONFIG
######################################################################
[EnergyDrinks] - Configurable (sodas, epi, morphine & PO-X affect energy levels by default)
[RestObjects] - Configurable (MediumTent makes you sleep 10% faster and protects from influenza by default)
######################################################################
LANGUAGE CONFIG
######################################################################
[Str_XXX] - All the text notifications are configurable in this mod (to make language translations or modifications easier)
######################################################################
GUI CONFIG
######################################################################
[TirednessHudX=0.855] The horizontal X coordinate for the tiredness meter on the ingame HUD (set to 0.0 to move the sleep meter underneath the stamina meter)
[TirednessHudY=0.03] The vertical Y coordinate for the tiredness meter on the ingame HUD (don't touch this unless you have UI conflicts with other mods)
######################################################################
DEBUG CONFIG
######################################################################
[AdminSteam64ID] Your Steam 64 ID for your admin account (allows you to reload server config with a hotkey - find your ID at www.steamidfinder.com)
[DebugOn=0] - Off by default. Set to 1 to receive all debug messages, set to 2 to only receive debug messages while sleeping
[RestUpdateTick=3] - Don't touch this unless you experience bad performance on high-pop servers. This affects how often all the above values are analyzed (3 secs by default).
######################################################################

Important Notes:
This mod is still in development and I'm not an experienced modder, so there may be issues with these early builds. Please be patient and also please let me know if you encounter any issues (and ideally as much info about what caused it as possible so I can investigate it and try to recreate it myself).

There are a handful of other minor quirks with the mod I'm still ironing out but so far I haven't discovered any major issues. 

The minor issues I know about are: sometimes the player will not close their eyes while sleeping (rare), sometimes the player will not yawn before falling unconscious from tiredness (rare), sometimes the player will not play a yawn sound when their eyes close (rare), sometimes the sleep meter will randomly appear full for a few seconds after consuming a medical item that puts them below maximum tiredness (fixes itself after a few moments), sometimes a sleep or yawn sound will play twice in a row which sounds kind of weird (rare) - I'll fix as many of these as I can when I find time. The source code is below - any suggestions from experienced modders are always welcome!

Credits:
Lucian & TRIANGLES

Repacking:
As with all my mods, you are free to repack or do whatever you like with this mod. But keep in mind that repacking will prevent my updates from being automatically applied, and this mod is still in development.

Source Code:
As with all my mods, the source code is open and free to be used, borrowed, stolen or studied in any way you like: LINK

PS.
Thanks to Lucian who mentioned this in his Medical Attention mod description:
You ARE allowed to repack, reupload, modify or reproduce any portions of this mod.
It would be appreciated if you include link to this original mod page.

In the same spirit I have the same policy, except you don't need to credit me as I don't really care - just credit Lucian for the foundation and idea.

Enjoy! I hope you guys have as much fun sleeping in the wilderness by a fire as I did when I first discovered Lucian's Medical Attention mod :)

I still remember the time I was cutting a bush to make a fire as my character was yawning his head off, and I randomly fell unconscious and was then shot to death a few seconds later by an opportunistic sniper who must've been watching me from afar. Talk about immersion!

######################################################################
CHANGELOG:
######################################################################
2nd March 2022:
----------------------------------------------------------------------
IMPORTANT PLAYER CHANGES:
- You can now use the lie-down emote ingame with a hotkey (keybind - P by default)
- Having wet clothes can now affect how well you rest and whether or not you get infected with influenza while sleeping

IMPORTANT SERVER CHANGES:
- Changed how the mod sends server-side config data to the client to be more efficient + allow reloading config without restart
- You can now update your server-side config and reload the settings from the client (keybind - Numpad 3 by default)
- To authorize your player to send reload commands, make sure to put your Steam 64 ID in the json config (AdminSteamID)
- Changed SleepBlackScreen to have two options. 0 = off, 1 = Black screen when sleeping, 2 = Blurry screen when sleeping
- Added NightTimeStartHour, NightTimeStartMin, NightTimeEndHour & NightTimeEndMin config settings - if value is above 0, this overrides night time detection (use 24 hour time)
- Added TirednessHudX and Y config settings - this can be used to move the tiredness meter to wherever you need it if the default settings conflict with other HUD elements.
    - Eg. If the default tiredness meter overlays an existing HUD element from another mod, try setting TirednessHudX=0 to move the meter to directly below the stamina bar.

MINOR SERVER CHANGES:
- Added DebugOn verbosity option (set DebugOn=1 to receive ALL messages, set it to 2 for less detailed messages to prevent cluttering the on-screen text)
- Added InfluenzaMultiplierNightNoFire config setting (this multiplies InfluenzaInjectNoFire value if sleeping with no fire at night time - set to 1.0 to disable)
- Added InfluenzaMultiplierWetNoFire config setting (this multiplies InfluenzaInjectNoFire value if sleeping with wet clothes and no fire at night time - set to 1.0 to disable)
- Added MaxRestWhenWetDay config setting (this sets the max rest % override for sleeping with wet clothes during the day - set to 100 to disable)
- Added MaxRestWhenWetNight config setting (this sets the max rest % override for sleeping with wet clothes during the night - set to 100 to disable)
- Added WetnessCancelsFireAccelerator config setting (if player is sleeping with wet clothes, they do not sleep any faster even if a fire is nearby - set to 0 to disable)
- Added BaseSleepAccelerator config setting (this is how fast the player sleeps if no fire or rest object is nearby - 1.0x by default)
- Added some new language strings for the new wetness sleeping features (Str_RestUpdate5 & Str_CantSleep5)
- Added "#define ZenSleep" so that other modders can interact with this mod in their code more easily

To implement all of these changes into your server config, update the mod and then you will need to carefully copy your existing settings into the new JSON file. The new JSON file is included in the mod folder and will also be created automatically if you run your server without an existing one (so if you haven't changed any settings, just delete your existing ZenSleepConfig.json file to apply the new changes).
----------------------------------------------------------------------
3rd March 2022:
----------------------------------------------------------------------
- Hotfix for Namalsk servers. The mod will now automatically detect Namalsk and generate the default JSON config values accordingly so that fire detection works properly.
- For other winter maps - turn DebugOn=1 and sleep by a fire to see what temperature the fire is giving the player, and adjust OutsideFireTemp and InsideFireTemp accordingly.
----------------------------------------------------------------------
27th April 2022:
----------------------------------------------------------------------
- Improved tiredness modifier to improve efficiency
- Made it so the same yawn and sleep sound can't play in a row
- Improved text notifications for how warm the player is
- Accidentally uploaded first version with 2/3 same male yawn sounds (fixed)
----------------------------------------------------------------------
15th June 2022:
----------------------------------------------------------------------
- Changed fireplace detection from temperature-based to object-based (should now work on all maps regardless of ambient temperature)
- Changed "energy drink" consumption so that it affects energy level based on gradual consumption (allows you to share drinks with other players)
- Fixed some "energy drink" items not applying their added energy/penalty to energy correctly
- Fixed hotkeys in the DayZ settings UI for 1.18 update and removed default "P" key (players might want to use the new inbuilt emote hotkeys instead)
- Made JSON config file more organized into sections
- Slightly adjusted some default settings
----------------------------------------------------------------------
19th September 2022:
----------------------------------------------------------------------
- Added new check for action-based sleeping in addition to gesture-based sleeping (now works with BoomLay's Things modded beds for example)
- Changed the immune system boost to only apply if the player has slept for at least 30 seconds, and is near a fire
- Changed config filename back to ZenSleep.json with a better config update mechanism for the future
- Converted all IsServer() calls to IsDedicatedServer() to optimize performance (only very slightly but hey, why not)
