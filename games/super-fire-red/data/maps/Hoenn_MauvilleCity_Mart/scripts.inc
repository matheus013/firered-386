@ Hoenn batch 2: FireRed Mart layout; pokeemerald MauvilleCity_Mart list.
Hoenn_MauvilleCity_Mart_MapScripts::
	.byte 0

Hoenn_MauvilleCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Hoenn_MauvilleCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Hoenn_MauvilleCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_NONE
	release
	end
