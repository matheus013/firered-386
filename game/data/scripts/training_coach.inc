EventScript_TrainingCoach::
	lock
	faceplayer
	special Training_IsUnlocked
	goto_if_eq VAR_RESULT, FALSE, EventScript_TrainingCoachLocked
	msgbox Text_TrainingCoachWelcome
	closemessage
	special OpenTrainingMenu
	waitstate
	release
	end

EventScript_TrainingCoachLocked::
	msgbox Text_TrainingCoachLocked
	release
	end

Text_TrainingCoachWelcome::
	.string "EV training is always free.\n"
	.string "Nature and IVs: after the League\l"
	.string "in Normal; always in Hardcore.$"

Text_TrainingCoachLocked::
	.string "Normal: return after the League.\n"
	.string "Hardcore: training from the start.$"
