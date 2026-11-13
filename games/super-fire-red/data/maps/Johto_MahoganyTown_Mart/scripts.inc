@ Johto geography: FireRed Mart layout; HGSS MahoganyTown Mart list.
Johto_MahoganyTown_Mart_MapScripts::
	.byte 0

Johto_MahoganyTown_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Johto_MahoganyTown_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Johto_MahoganyTown_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_REVIVE
	.2byte ITEM_NONE
	release
	end
