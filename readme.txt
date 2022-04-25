What Is This?

This mod adds an immersive sleeping feature to the game.

It is heavily based on and inspired by Lucian's old Medical Attention mod, so 90% of the credit goes to him (and TRIANGLES for updating his mod to work with recent builds of DayZ).

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

Sleeping next to a fire will not only protect you from potentially catching influenza, but will help you rest faster and recover more energy.

Keep in mind that it takes a while for the fire to generate enough heat for you to feel it, so build your fire large and let it burn for a minute or two before resting.

Having the heat buffer will protect you from influenza, but some fires are hotter than others and you will recover energy faster next to a really large & warm fire.

During the day you can only recover 50% of your energy sleeping without a fire, or 75% with a fire.
During the night you can only recover 80% of your energy sleeping without a fire, or 100% with a fire.
Sleeping inside buildings and tents (especially with a fire nearby) is the best way to recover your energy quickly.

Sleeping in a building with a fire lit is the fastest way to recover your energy, and you have no chance of catching the flu.

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

Fire Notes:
The temperature of fires varies depending on the map, and the weather. The default temperature values are configured for regular Chernarus and Livonia but might need tweaking depending on which map you have on your server.

To see the temperature of a fire, build one on your private LAN or test server and sleep next to it with DebugOn=1 in the json config to see what temp your player is getting.

If you have issues detecting fires on winter maps it might be worth just turning "HeatBuffCountsAsFire=true" on.

Rest Accelerators:
Depending on where you are sleeping will affect how quickly you regenerate energy. The mod can tell when you are sleeping under a roof and if you have a fire lit nearby.
- Asleep Outside With No Fire = 1x
- Asleep Outside With A Fire = 5% faster
- Asleep Inside House or Building With No Fire = 10% faster
- Asleep Inside House or Building With A Fire = 25% faster

Energy Drinks / Consumption Items:
You can add your own energy drinks/consumption items in the json config. The default ones are:
- Cola: +25% energy
- Pipsi: +20% energy
- Fronta: + 15% energy
- Spite: +10% energy
- Kvass: +5% energy

The mod checks for energy drink items and rest object types with a "contains" check and is not case-sensitive. So in other words, if you want to make all soda cans restore 20% of your energy you can just set the JSON config to have this entry:

{
    "ItemType": "sodacan",
    "EnergyGained": 20
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

(If you want to enable them spawning, just copy the provided types.xml from the mod folder into your server's types.xml)

Rest Objects:
You can add specific objects in the json config that affect how the player sleeps.
Note: This feature is still in development and doesn't work 100% of the time on some objects.
- MediumTent: Max rest during the day with no fire is 60%, you sleep 10% faster and no chance of influenza regardless of fire status.

Default Mod Settings That Can Be Tweaked/Enabled/Disabled:
[MaxWakeTime=240] Maximum time the player can stay awake before they are forced uncon (in minutes - 4 hours by default)
[AllowInventoryWhileSleep=false] Allows the player to access their inventory while sleeping (they can only move things around and can't put items into their hands)
[CanUnconInVehicle=true] Whether or not players can fall asleep while driving
[SleepBlackScreen=true] Screen turns black when sleeping
[TextNotificationOn=true] Text notifications to update the player's sleep status
[TextNotificationFrequency=5] How often to send text updates while sleeping (every 5 rest ticks or 15 secs by default).
[OnlyShowSleepOnInventory=false] Only show tiredness meter when tabbed and in the inventory
[OnlyShowSleepAbovePercent=off] Only show tiredness meter when tiredness is over a certain % (0-100)
[SleepSoundPercentChance=8%] Percent chance to play a sleeping sound while the player is tired (0 to disable)
[YawnPercentStart=20%] The tiredness % level when random yawning can start to happen (to notify player they need to sleep soon)
[YawnPercentChance=6.5%] The base chance of playing a yawn sound when tiredness is over YawnPercentStart (slowly increases the more tired they are)
[UnconPercentStart=10%] The tiredness % level when the player can start to randomly fall unconscious
[UnconPercentChance=1%] The base chance of falling uncon when tiredness is over UnconPercentStart (slowly increases the more tired they are)
[InfluenzaInjectNoFire=5] How much influenza to inject the player with when sleeping without a fire (random between 0 and this value - 1000 = max influenza)
[MaxRestDayNoFire=50%] Maximum achievable rest when sleeping during the day with no fire
[MaxRestDayWithFire=75%] Maximum achievable rest when sleeping during the day with a fire
[MaxRestNightNoFire=80%] Maximum achievable rest when sleeping during the night with no fire
[MaxRestNightWithFire=100%] Maximum achievable rest when sleeping during the night with a fire
[HeatBuffCountsAsFire=false] If enabled then having the heat buff is equivalent to sleeping next to a fire and will apply sleep accelerators.
[OutsideFireTemp=28] How hot a fire typically is when lit outside (this varies depending on the map & weather - use debug mode on a private server to adjust)
[InsideFireTemp=38] How hot a fire typically is when lit inside (this varies depending on the map & weather - use debug mode on a private server to adjust)
[AsleepAccelerator=0.01%] Energy recovery speed increases by this value every few seconds while sleeping so the longer you sleep the faster you recover
[OutsideFireAcceleratorPercent=5%] You recover energy 5% faster when sleeping near a fire outside
[InsideNoFireAcceleratorPercent=10%] You recover energy 10% faster when sleeping inside a building with no fire nearby
[InsideFireAcceleratorPercent=25%] You recover energy 25% faster when sleeping inside a building with a fire nearby
[EnergyDrinks] - Configurable (sodas, epi, morphine & PO-X affect energy levels by default)
[RestObjects] - Configurable (MediumTent makes you sleep 10% faster and protects from influenza by default)

[Str_XXX] - All the text notifications are configurable in this mod (to make language translations or modifications easier)

[DebugOn=false] - Don't turn this on unless you are trying to figure out why the mod is doing something wrong or tweaking stuff and want debug messages sent to your player.
[RestUpdateTick=3] - Don't touch this unless you experience bad performance on high-pop servers. This affects how often all the above values are analyzed (3 secs by default).


Important Notes:
This mod is still in development and I'm not an experienced modder, so there may be issues with these early builds. Please be patient and also please let me know if you encounter any issues (and ideally as much info about what caused it as possible so I can investigate it and try to recreate it myself).

This mod has only been tested on my own private server so far with myself and an alt account, so I really have no idea how it will perform on highly populated servers. But I've done my very best to write the code to be as optimized and efficient as I can, so it *should* theoretically run fine on most servers.

I haven't tested it on Namalsk yet, please let me know if the heat sources don't quite work there as it's likely the environment temperatures will be a lot lower than Livonia or Chernarus. I included the options to set the fire temperatures in the json config (OutsideFireTemp & InsideFireTemp) - you will probably need to tweak these on winter maps.

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

I still remember the time I was cutting a bush to make a fire as my character was yawning his head off, and I randomly fell unconscious and was shot to death by an opportunistic player who was watching me from afar. Talk about immersion!