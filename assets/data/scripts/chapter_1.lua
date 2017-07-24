Eric = Character {dialogName = "Eric"}
Jeff = Character {dialogName = "Jeff"}
Judge = Character {dialogName = "The Great Judgini"}

START = Scene {
    background = "background_coridor_1",

    Jeff "Isn't this copyright infringement again?",
    Eric "Would you just shut up...",
    Judge "I have reviewed the evidence.  How do you plead?",
    
    Menu {
        {
            "Not Guilty", function (scene)
                return "NOT_GUILTY"
            end 
        }, {
            "Obviously Guilty", function (scene)
                return "THE_TRUTH"
            end 
        }
    },
}

NOT_GUILTY = Scene {
    Eric "Not guilty, your Honor.",
    Judge "Denied!  It's opposite day!",
    Jeff "Wait, what the...",
    ChangeToScene "THE_TRUTH",
}

THE_TRUTH = Scene {
    Eric "Ok, so we infringed on copyrighted material...",
    Jeff "You did.",
    Judge "Then I hearby sentence you to hard labor in that prison from Rambo 2.",
    Jeff "What?!?",
    Eric "Oh cool, do we get a visit from Richard Crenna?",
    Judge "No.  He died in 2003, remember?",
    Eric "Oh yeah...",
    ChangeToScene "END",
}

END = Scene {
    Jeff "I hate you.  I hate you so much.",
    Eric "I love you too buddy.",
    Jeff "So much hate..."
}
