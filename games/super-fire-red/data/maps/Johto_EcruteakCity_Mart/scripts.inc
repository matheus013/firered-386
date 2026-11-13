@ Johto geography: FireRed Mart layout; HGSS EcruteakCity Mart list.
Johto_EcruteakCity_Mart_MapScripts::
	.byte 0

Johto_EcruteakCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Johto_EcruteakCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Johto_EcruteakCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_NONE
	release
	end
