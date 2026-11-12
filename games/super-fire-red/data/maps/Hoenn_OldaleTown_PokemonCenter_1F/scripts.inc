@ Hoenn batch 1: FireRed Pokemon Center layout (scripts/install_hoenn_batch1.py).
Hoenn_OldaleTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Hoenn_OldaleTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Hoenn_OldaleTown_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_OLDALE_TOWN
	end

Hoenn_OldaleTown_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
