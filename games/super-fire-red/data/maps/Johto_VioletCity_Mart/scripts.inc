@ Johto geography: FireRed Mart layout; HGSS VioletCity Mart list.
Johto_VioletCity_Mart_MapScripts::
	.byte 0

Johto_VioletCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Johto_VioletCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Johto_VioletCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_X_DEFEND
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_SPEED
	.2byte ITEM_NONE
	release
	end
