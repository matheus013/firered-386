@ Hoenn batch 2: FireRed Mart layout; pokeemerald VerdanturfTown_Mart list.
Hoenn_VerdanturfTown_Mart_MapScripts::
	.byte 0

Hoenn_VerdanturfTown_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Hoenn_VerdanturfTown_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Hoenn_VerdanturfTown_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_NEST_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_X_SPECIAL
	.2byte ITEM_FLUFFY_TAIL
	.2byte ITEM_NONE
	release
	end
