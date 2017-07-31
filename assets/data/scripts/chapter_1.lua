Radio = Character {dialogName = "Радио"}
Reporter = Character {dialogName = "Репортёр" }

radioManName = "Андрей"

START = Scene {
    background="background_room_1",

    "27.08.2317",
    Radio {
        "...даём слово нашему репортёру, который находится на выставке AI-317",
        "где в данный момент идёт презентация первого робота с полноценным искусственным интеллектом."
    },

    Reporter "Да, здравствуйте <<radioManName>>."
}
