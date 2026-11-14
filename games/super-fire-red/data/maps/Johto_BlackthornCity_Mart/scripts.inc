@ Johto geography: FireRed Mart layout; HGSS BlackthornCity Mart list.
Johto_BlackthornCity_Mart_MapScripts::
	.byte 0

Johto_BlackthornCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Johto_BlackthornCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Johto_BlackthornCity_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_X_DEFEND
	.2byte ITEM_X_ATTACK
	.2byte ITEM_NONE
	release
	end
