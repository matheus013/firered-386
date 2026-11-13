@ Hoenn batch 2: FireRed Mart layout; pokeemerald SlateportCity_Mart list.
Hoenn_SlateportCity_Mart_MapScripts::
	.byte 0

Hoenn_SlateportCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Hoenn_SlateportCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Hoenn_SlateportCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_HARBOR_MAIL
	.2byte ITEM_NONE
	release
	end
