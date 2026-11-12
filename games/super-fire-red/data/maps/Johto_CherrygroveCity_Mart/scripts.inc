@ Johto batch 1: FireRed Mart layout; HGSS Cherrygrove Mart list.
Johto_CherrygroveCity_Mart_MapScripts::
	.byte 0

Johto_CherrygroveCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Johto_CherrygroveCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Johto_CherrygroveCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_NONE
	release
	end
