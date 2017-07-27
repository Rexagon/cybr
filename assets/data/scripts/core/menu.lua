local MenuChoice = {}
MenuChoice.__index = MenuChoice

-- Конструктор класса выбора
function MenuChoice:new(properties)
    local choice = {}
    setmetatable(choice, MenuChoice)

    -- Текст выбора
    choice.label = properties["label"]

    -- Функция, которыя будет вызвана при выборе этого элемента
    -- Принимает аргументом Core.Scene, в котором лежит меню с этим выбором
    -- Возвращать должна либо ничего, либо Opcode, либо массив Opcode
    choice.action = properties["action"]

    if choice.label == nil then
        Engine.Log("MenuChoice must have a label.")
        return
    elseif type(choice.action) ~= "function" then
        Engine.Log("MenuChoice action must be a function.")
        return
    end

    return choice
end

Core.MenuChoice = MenuChoice


local Menu = {}
Menu.__index = Menu


function Menu:new(properties)
    local menu = {}
    setmetatable(menu, Menu)

    if properties["name"] ~= nil then
        menu.name = properties.name
    else
        menu.name = "<Unnamed Menu>"
    end

    -- Массив из MenuChoice, которые будут выведены в этом меню
    menu.choices = {}

    -- Номер выбора, надо которым сейчас курсор
    menu.currentChoiceIndex = nil

    -- Таблица с даннымми для menu.choises
    -- Ключ - название выбора
    -- Значение - действие или строка(название сцены, куда переместиться)
    local choices = {}


    for _,value in pairs(properties) do
        if type(value) ~= "table" or #value < 2 then
            Engine.Log("Invalid data for a menu choice.")
            return
        end

        local action

        if type(value[2]) == "string" then
            action = function (scene)
                return Core.Opcode:new("change-scene", {target=value[2]})
            end
        else
            action = value[2]
        end

        table.insert(choices, Core.MenuChoice:new{label=value[1], action=action})
    end

    menu.choices = choices

    return menu
end

-- Упрощение создания меню, теперь можно писать просто Menu
Core.CreateConstructorAlias("Menu", Menu)

--[[
-- Функции для переключения между пунктами меню
function Menu:moveForward()
    if self.currentChoiceIndex == nil then
        self.currentChoiceIndex = 1
    elseif self.currentChoiceIndex < #self.choices then
        self.currentChoiceIndex = self.currentChoiceIndex + 1
    end

    return self.currentChoiceIndex
end


function Menu:moveBack()
    if self.currentChoiceIndex == nil then
        self.currentChoiceIndex = #self.choices
    elseif self.currentChoiceIndex > 1 then
        self.currentChoiceIndex = self.currentChoiceIndex - 1
    end

    return self.currentChoiceIndex
end
--]]


-- Возвращает функцию текущего действия
function Menu:getCurrentChoiceAction()
    return self.choices[self.currentChoiceIndex].action
end


return Menu
