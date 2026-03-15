class CfgPatches
{
	class Citadel_PC
	{
		units[]= 
		{
			"Citadel_PC"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]= 
		{
			"DZ_Data"
		};
	};
};
class CfgMods
{
	class Citadel_PC
	{
		dir="Citadel_PC";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Citadel_PC";
		credits="Citadel";
		author="Genesis";
		authorID="76561198273516768";
		version="0.1";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"Citadel_Basecameras/scripts/4_World",
					"Citadel_PC/Scripts/4_World"
				};
			};
			class imageSets
			{
				files[]= 
				{
					"Citadel_PC\GUI\ImageSets\Citadel_PC.imageset"
				};
			};
		};
	};
};
//State your sound shaders , then call the sound shaders in the sound set , call the sound set from your .C
class CfgSoundShaders
{
	class Citadel_PC_Run_SoundShader
	{
		samples[] = { {"Citadel_PC\Scripts\Sounds\FAN-NOISE",1} };
		range = 25;
		volume = 3.0;
	};
	class Citadel_PC_Start_SoundShader
	{
		samples[] = { {"Citadel_PC\Scripts\Sounds\Windows_11_Startup_sound",1} };
		range = 25;
		volume = 2.5;
	};
	class Citadel_PC_Shutdown_SoundShader
	{
		samples[] = { {"Citadel_PC\Scripts\Sounds\shutdown",1} };
		range = 25;
		volume = 4.0;
	};
};
//SoundSet
class CfgSoundSets
{
	class Citadel_FAN_SoundSet
	{
		soundShaders[] = { "Citadel_PC_Run_SoundShader" };
		frequencyRandomizer = 1;
	};
	class Citadel_BOOT_SoundSet
	{
		soundShaders[] = { "Citadel_PC_Start_SoundShader" };
		frequencyRandomizer = 1;
		volumeRandomizer = 1;
	};
	class Citadel_SHUTDOWN_SoundSet
	{
		soundShaders[] = { "Citadel_PC_Shutdown_SoundShader" };
		frequencyRandomizer = 1;
		volumeRandomizer = 1;
	};
};