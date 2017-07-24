local Scene = {}
Scene.__index = Scene


function Scene:new(properties)
    local scene = {}
    setmetatable(scene, Scene)

    -- Массив строк или функций
    -- Строки - названия сцен, которые должны быть посещёнными
    -- Функции - должны возвращать true или false
    scene.preconditions = {}

    -- Название текстуры фона
    scene.background = ""


    -- переопределяем всё, что выше параметрами из конструктора
    for name,value in pairs(properties) do
        if rawget(scene, name) ~= nil then
            rawset(scene, name, value)
        end
    end

    if properties["background"] ~= nil then
        Engine.SetBackground(scene.background)
    end

    -- Массив с персонажами, которые находятся на сцене в текущий момент
    -- Ключ - название персонажа
    -- Значение - объект персонажа
    self.activeCharacters = {}
    setmetatable(self.activeCharacters, { __mode = "v" })

    local opcodes = {}

    for _,content in ipairs(properties) do
        local new_opcode = self.createOpcodeFromContent(content)

        if rawget(new_opcode, "__flatten") == true then
            for _,op in ipairs(new_opcode) do
                table.insert(opcodes, op)
            end
        else
            table.insert(opcodes, new_opcode)
        end
    end

    scene.opcodes = opcodes

    if #scene.opcodes < 1 then
        Engine.Log("Creating a Scene with zero opcodes.")
    end

    scene.opcodeIndex = 1

    return scene
end

-- Упрощение создания сцены, теперь можно писать просто Scene
Core.CreateConstructorAlias("Scene", Scene)


-- Функция для создания opcode из параметра конструктора
function Scene.createOpcodeFromContent(content)
    local contentType = type(content)

    if contentType == "string" then
        return Core.Opcode:new("say", {content=content})
    end

    if contentType == "function" then
        setfenv(content, Core.ScriptEnvironment)
        local contentValue = content()
        if type(contentValue) == "string" then
            return Core.Opcode:new("say", {content=contentValue})
        end
        
        if type(contentValue) ~= "table" or getmetatable(contentValue) ~= Core.Opcode then
        	Engine.Log("Functions as scene content must return a string or opcode")
       	end
        return contentValue
    end

    if contentType ~= "table" then
    	Engine.Log("Unknown content type in Scene")
    end

    if getmetatable(content) == Core.Menu then
        return Core.Opcode:new("add-menu", {menu=content})
    end

    if getmetatable(content) ~= Core.Opcode then
        for index,opcode in ipairs(content) do
            content[index] = opcode:process()
        end
        return content
    end

    return content:process()
end


-- Функция для прохода по всем Opcode в сцене
function Scene:mapOpcodeFunction(f)
    local opcode = self.opcodes[self.opcodeIndex]

    if getmetatable(opcode) == Core.Opcode then
        f(self, opcode)
    else
        for _,op in opcode do
            f(self, op)
        end
    end
end


-- Opcode которые активируют персонажа
local characterActivatingOpcodes = {
    ["say"] = true,
    ["think"] = true,
    ["move-character"] = true,
    ["set-character-image"] = true,
}

-- Функция для обновления информации о текущих персонажах
function Scene:refreshActiveCharacters()
    local function refresh(scene, opcode)
        if opcode["arguments"] == nil then return end
        
        local character = opcode.arguments["character"]

        if character == nil then return end

        if opcode.name == "deactivate-character" then
            self.activeCharacters[character.dialogName] = nil
        elseif characterActivatingOpcodes[opcode.name] == true then
            self.activeCharacters[character.dialogName] = character
        end
    end

    self:mapOpcodeFunction(refresh)
end


-- Функия для получения списка инструкций на текущий кадр
function Scene:getCurrentInstructions()
    local instructions = {}
    local noops = {
        ["no-op"] = true,
        ["deactive-character"] = true,
    }
    
    local function collectInstructions(scene, opcode)
        if noops[opcode.name] ~= true then
            table.insert(instructions, Core.Instruction.ForOpcode[opcode.name])
        end
    end
    
    self:mapOpcodeFunction(collectInstructions)
    
    return instructions
end


-- Функция для проверки возможности перехода к сцене
local function sceneSatisfiesPreconditions(scene)
    for index,requisite in pairs(scene.preconditions) do
        if type(requisite) == "string" then
            if Core.ScriptEnvironment[requisite] == nil then
                Engine.Log("Cannot find prerequisite scene " .. requisite)
            end

            if getmetatable(Core.ScriptEnvironment[requisite]) ~= Core.Scene then
                Engine.Log("Prerequsite scene " .. requisite .. " is not a valid Scene")
            end

            if not Core.VisitedScenes[requisite] then
                return false, index
            end
        elseif type(requisite) == "function" then
            if requisite(scene) == false then
                return false, index
            end
        else
            Engine.Log("Unknown type of precondition. Must be a string or function.")
      end
   end

   return true, nil
end


-- Смена сцены
local function changeToScene(name)
    local scene = Core.ScriptEnvironment[name]

    Engine.Log("Changing to scene " .. name)
    if scene == nil then
        Engine.Log("Cannot find scene with variable name " .. name)
        return
    elseif getmetatable(scene) ~= Core.Scene then
        Engine.Log(name .. " is a variable but not a Scene")
        return
    end

    if scene["preconditions"] ~= nil then
        if sceneSatisfiesPreconditions(scene) ~= true then
            Engine.log("Scene " .. name .. " fails to satisfy preconditions.")
            return
        end
    end

    Core.VisitedScenes[name] = true
    table.insert(Core.SceneHistory, name)

    Core.CurrentScene = scene
end


Core.ChangeToScene = changeToScene

-- Упрощение смены сцены, теперь можно писать просто ChangeToScene
Core.CreateFunctionAlias(
    "ChangeToScene",
    function (target)
        return Core.Opcode:new("change-scene", {target=target})
    end
)

function Scene:moveForward()
    if self.opcodeIndex < #self.opcodes then
        self.opcodeIndex = self.opcodeIndex + 1
        --print("changing " .. (self.opcodeIndex - 1) .. " to " .. self.opcodeIndex)
    end
end

function Scene:moveBack()
   while self.opcodeIndex > 1 do
        self.opcodeIndex = self.opcodeIndex - 1
        --print("changing " .. (self.opcodeIndex + 1) .. " to " .. self.opcodeIndex)

        if self.opcodes[self.opcodeIndex].immediate == false then
            return
        end
    end
end

-- Функция для смены заднего фона сцены
function Scene.changeBackgroundTo(filename)
    return Core.Opcode:new("set-scene-image", {image=filename})
end

Core.CreateFunctionAlias("ChangeBackgroundTo", Scene.changeBackgroundTo)


-- Устанавливает глобальную переменную в указанное значение
function Scene.setEnvironmentVariable(data)
    return Core.Opcode:new("export-variable", {name=data[1], value=data[2]})
end

Core.CreateFunctionAlias("Set", Scene.export)


-- Получает глобальную переменную
function Scene.getEnvironmentVariable(name)
   return Core.Opcode:new("import-variable", {name=name})
end

Core.CreateFunctionAlias("Get", Scene.getEnvironmentVariable)


-- Выполняет указанную функцию
function Scene.executeFunction(content)
   return Core.Opcode:new("execute", {content=content})
end

Core.CreateFunctionAlias("Execute", Scene.executeFunction)

Core.ScriptEnvironment["Pause"] = Core.Opcode:new("no-op")


-- Стандартный обработчик сцены
function Scene.DefaultHandler(self)
    if #self.opcodes < 1 then return end

    self:refreshActiveCharacters()

    local executeInstruction = function ()
        local opcode = self.opcodes[self.opcodeIndex]

        for _,instruction in ipairs(self:getCurrentInstructions()) do
            opcode.arguments.scene = self
            instruction(opcode.arguments)
        end

        if opcode.immediate == true then
            self:moveForward()
            return true
        else
            return false
        end
    end

    repeat
        local keepExecuting = executeInstruction()
    until
        keepExecuting == false
end

Core.Settings.Handlers.Scene = Scene.DefaultHandler

function Scene:draw()
    Core.Settings.Handlers.Scene(self)
end

return Scene
