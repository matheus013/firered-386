@ Johto geography: FireRed Pokemon Center layout (scripts/install_johto_batch1.py).
Johto_OlivineCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Johto_OlivineCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Johto_OlivineCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_OLIVINE_CITY
	end

Johto_OlivineCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
