local Opcode = {}
Opcode.__index = Opcode


-- Все обрабатываемые Opcode
Opcode.ValidOpcodes = {
    ["say"] = true,
    ["think"] = true,
    ["set-character-image"] = true,
    ["move-character"] = true,
    ["change-scene"] = true,
    ["no-op"] = true,
    ["set-scene-image"] = true,
    ["deactivate-character"] = true,
    ["add-menu"] = true,
    ["set-character-name"] = true,
    ["export-variable"] = true,
    ["import-variable"] = true,
    ["execute"] = true
}

-- Все Opcode, которые выполняются не ожидая ввода
local ImmediateOpcodes = {
    ["set-character-name"] = true,
    ["export-variable"] = true,
    ["execute"] = true
}


-- Конструктор который принимает параметрами название из ValidOpcodes
-- и таблицу аргументов которые opcode передаст позже в инструкции
function Opcode:new(name, arguments)
    local opcode = {}
    setmetatable(opcode, Opcode)
    
    if Opcode.ValidOpcodes[name] == nil then
        Engine.Log("unknown opcode name: " .. name)
        return
    end

    opcode.name = name
    opcode.arguments = arguments

    if (ImmediateOpcodes[name] == true) then
        opcode.immediate = true
    else
        opcode.immediate = false
    end

    return opcode
end


-- Обработчики opcode
local Processors = {}

Processors["say"] = function (opcode)
    if type(opcode.arguments.content) == "string" then
        return opcode
    end

    local opcodeList = {}
    for _,content in ipairs(opcode.arguments.content) do
        table.insert(opcodeList, Opcode:new("say", {content=content, character=opcode.arguments.character}))
    end
    rawset(opcodeList, "__flatten", true)

    return opcodeList
end


Processors["think"] = function (opcode)
    if type(opcode.arguments.content) == "string" then
        return opcode
    end

    local thoughtOpcodes = {}
    for _,content in ipairs(opcode.arguments.content) do
        table.insert(thoughtOpcodes, Opcode:new("think", {content=content, character=opcode.arguments.character}))
    end
    rawset(thoughtOpcodes, "__flatten", true)

    return thoughtOpcodes
end


Processors["move-character"] = function (opcode)
    opcode.arguments.target = opcode.arguments.character
    return opcode
end


Processors["set-character-image"] = function (opcode)
    opcode.arguments.target = opcode.arguments.character
    return opcode
end


Processors["set-character-name"] = function (opcode)
    opcode.arguments.target = opcode.arguments.character
    return opcode
end


Processors["set-scene-image"] = function (opcode)
    opcode.arguments.target = {}
    setmetatable(opcode.arguments.target, Core.Scene)
    return opcode
end


local returnOpcode = function (opcode)
    return opcode 
end

Processors["change-scene"] = returnOpcode
Processors["no-op"] = returnOpcode
Processors["deactivate-character"] = returnOpcode
Processors["add-menu"] = returnOpcode
Processors["export-variable"] = returnOpcode
Processors["import-variable"] = returnOpcode
Processors["execute"] = returnOpcode


function Opcode:process()
    local result = Processors[self.name](self)
    if result == nil then
        Engine.Log("Opcode processor for " .. self.name .. " did not return a value.")
    end
    return result
end


return Opcode
