-- Стандартная функция для замены контекста исполнения
if not setfenv then
    local function findenv(f)
        local level = 1
        repeat
        local name, value = debug.getupvalue(f, level)
            if name == "_ENV" then return level, value end
			level = level + 1
		until name == nil
	    return nil 
	end
    
    getfenv = function (f) return(select(2, findenv(f)) or _G) end
    setfenv = function (f, t)
        local level = findenv(f)
        if level then debug.setupvalue(f, level, t) end
        return f 
    end
end


Core = {}

-- Создание локального контекста для скриптов сценария
Core.ScriptEnvironment = {
	["Core"] = Core
}

Core.ScriptEnvironment["Engine"] = Engine

Core.ScriptEnvironment["print"] = print
Core.ScriptEnvironment["tostring"] = tostring


-- Зарезервированые слова
local ReservedKeywords = {}


setmetatable(Core.ScriptEnvironment, {
	__newindex = function (table, key, value)
		if ReservedKeywords[key] ~= nil then
			error(key .. " is a reserved word.")
		else
			rawset(table, key, value)
		end
	end
})

-- Упрощение конструктора
--[[
    Например заменяет
    Core.Scene на просто Scene
--]]
function Core.CreateConstructorAlias(name, class)
    Core.ScriptEnvironment[name] = function (...)
        return class:new(...)
    end
    ReservedKeywords[name] = "function"
end


-- Упрощение вызова функции
--[[
    Например заменяет
    Core.CreateFunctionAlias на CreateFunctionAlias
--]]
function Core.CreateFunctionAlias(name, implementation)
    Core.ScriptEnvironment[name] = function (...)
        return implementation(...)
    end
    ReservedKeywords[name] = "function"
end


-- Текущая сцена
Core.CurrentScene = nil

-- Таблица с посещёнными сценами
-- Ключи - название сцены
-- Значения - true
-- Порядок может быть любым
Core.VisitedScenes = {}

-- Массив с посещёнными сценами
-- Ключи - номер посещения сцены
-- Значения - названия
Core.SceneHistory = {}


-- Функция для инициализации библиотеки
function Core.Initialize(prefix)
	local loadModule = function (name, path)
        Core[name] = require("core." .. path)
    end

    loadModule("Settings", "settings")

    loadModule("Position", "position")

    loadModule("Opcode", "opcode")
    loadModule("Instruction", "instruction")

    loadModule("Character", "character")
    loadModule("Scene", "scene")
    loadModule("Menu", "menu")
    loadModule("Context", "context")
end


-- Слияние контекстов исполнения
local function mergeContexts(...)
    local contexts = {...}

    if #contexts == 0 then return end

    for _,context in ipairs(contexts) do
        for key,value in pairs(context.data) do
            Core.ScriptEnvironment[key] = value
        end
    end
end


-- Проверка на переопределение ключевых слов
local function checkReservedKeywordTypes()
    for key,value in pairs(Core.ScriptEnvironment) do
        if ReservedKeywords[key] ~= nil then
            local environmentType = type(Core.ScriptEnvironment[key])
            if environmentType ~= ReservedKeywords[key] then
                Engine.Log("Reserved keyword " .. key .. " has the incorrect type " .. environmentType .. ".")
            end
        end
    end
end


-- Функция для загрузки скрипта с сюжетом
function Core.LoadScript(filename, ...)
    local script = loadfile(filename)

    mergeContexts(...)
    assert(script, "Unable to load script " .. filename)
    setfenv(script, Core.ScriptEnvironment)

    pcall(script)

    checkReservedKeywordTypes()

    if Core.ScriptEnvironment["START"] ~= nil then
		Core.ChangeToScene("START")
    end
end


function Core.Update()
    if Core.CurrentScene ~= nil then
        Core.Scene.DefaultHandler(Core.CurrentScene)
    end
end

function Core.Forward()
    if Core.CurrentScene ~= nil then
        Core.CurrentScene:moveForward()
    end
end


function Core.Backward()
    if Core.CurrentScene ~= nil then
        Core.CurrentScene:moveBack()
    end
end



return Core