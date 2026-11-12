@ Johto batch 1: FireRed Pokemon Center layout (scripts/install_johto_batch1.py).
Johto_CherrygroveCity_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Johto_CherrygroveCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Johto_CherrygroveCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_CHERRYGROVE_CITY
	end

Johto_CherrygroveCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
