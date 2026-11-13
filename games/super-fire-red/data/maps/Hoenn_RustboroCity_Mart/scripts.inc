@ Hoenn batch 2: FireRed Mart layout; pokeemerald RustboroCity_Mart list.
Hoenn_RustboroCity_Mart_MapScripts::
	.byte 0

Hoenn_RustboroCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart Hoenn_RustboroCity_Mart_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
Hoenn_RustboroCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_TIMER_BALL
	.2byte ITEM_REPEAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_NONE
	release
	end
