@ Hoenn batch 2: FireRed Pokemon Center layout (scripts/install_hoenn_batch2.py).
Hoenn_VerdanturfTown_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Hoenn_VerdanturfTown_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Hoenn_VerdanturfTown_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VERDANTURF_TOWN
	end

Hoenn_VerdanturfTown_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
