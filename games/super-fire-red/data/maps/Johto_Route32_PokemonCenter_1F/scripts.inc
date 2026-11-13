@ Johto geography: FireRed Pokemon Center layout (scripts/install_johto_batch1.py).
Johto_Route32_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Johto_Route32_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Johto_Route32_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_ROUTE32
	end

Johto_Route32_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
