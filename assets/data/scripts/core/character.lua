local Character = {}
Character.__index = Character


function Character:new(properties)
    local character = {}
    setmetatable(character, Character)

    -- Отображаемое имя персонажа
    character.dialogName = ""

    -- Полное имя
    character.firstName = ""
    character.lastName = ""

    -- Спрайты персонажа
    character.images = { normal = nil }

    -- Название текущего спрайта
    character.currentImage = "normal"

    -- Положение персонажа на экране
    character.position = "center"
    

    -- переопределяем всё, что выше параметрами из конструктора
    for name,value in pairs(properties) do
        if rawget(character, name) ~= nil then
            rawset(character, name, value)
        end
    end

    if properties["position"] ~= nil then
        character.position = Core.Position[properties["position"]]
    end

    if properties["image"] ~= nil then
        character.images.normal = properties.image
    end
    
    if character.dialogName == nil or character.dialogName == "" then
        if character.firstName == nil or character.firstName == "" then
            Log("Unable to create unnamed character")
            return
        else
            character.dialogName = character.firstName
        end
    end

    return character
end

-- Упрощение создания персонажа, теперь можно писать просто Character
Core.CreateConstructorAlias("Character", Character)


-- Заставляет персонажа говорить
function Character:says(text)
    return Core.Opcode:new("say", {content=text, character=self})
end

-- Заставляет персонажа думать
function Character:thinks(thoughts)
    return Core.Opcode:new("think", {content=thoughts, character=self})
end

-- Возможные источники для отображаемого имени
local acceptableDisplayNameValues = {
    ["default"] = true,
    ["firstName"] = true,
    ["lastName"] = true,
    ["fullName"] = true,
}

-- Меняет отображаемое имя на имя с указанного источника
function Character:displayName(nameType)
    if acceptableDisplayNameValues[nameType] then
        return Core.Opcode:new("set-character-name", {character=self, name=nameType})
    else
        Engine.Log("Unacceptable character display name type: " .. nameType)
        return
    end
end

-- Возвращает полное имя персонада
function Character:getFullName()
    return self.firstName .. self.lastName
end

-- Добавляет оператоп вызова к классу персонажа
-- Теперь можно писать так: CharacterName "text he says"
Character.__call = function (character, ...)
    return character:says(...)
end

-- Устанавливает позицию персонажа
function Character:isAt(place)
    return Core.Opcode:new("move-character", {character=self, position=Core.Position[place]})
end

-- Устанавливает спрайт персонажа, добавляя его название в таблицу images
function Character:becomes(filename)
    if self.images[filename] == nil then
        self.images[filename] = filename
    end

    return Core.Opcode:new("set-character-image", {character=self, image=filename})
end

-- Возвращает первоначальный спрайт персонажа
function Character:becomesNormal()
    return self:becomes("normal")
end

-- Выкидывает персонажа со сцены
function Character:leavesTheScene()
    return Core.Opcode:new("deactivate-character", {character=self})
end


-- Стандартный обработчик персонажа
function Character.DefaultHandler(self)
    local image = self.images[self.currentImage]

    --[[
    if image ~= nil then
        image:setPosition(self.position)
        image:draw()
    end
    --]]
end

Core.Settings.Handlers.Character = Character.DefaultHandler

function Character:draw()
    Core.Settings.Handlers.Character(self)
end


return Character