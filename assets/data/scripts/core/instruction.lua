local Instruction = {}
Instruction.__index = Instruction


-- Все обрабатываемые Instruction
Instruction.ValidInstructions = {
    ["say"] = true,
    ["set-name"] = true,
    ["set-data"] = true,
    ["get-data"] = true,
    ["set-image"] = true,
    ["draw-image"] = true,
    ["set-scene"] = true,
    ["no-op"] = true,
    ["show-menu"] = true,
    ["set-position"] = true,
    ["execute"] = true
}


-- Конструктор Instruction
function Instruction:new(properties)
    local instruction = {}
    setmetatable(instruction, Instruction)

    if Instruction.ValidInstructions[properties.name] == nil then
        Engine.Log("Unknown instruction " .. properties.name)
        return
    end

    instruction.name = properties.name
    instruction.action = properties.action

    return instruction
end


Instruction.__call = function (f, ...)
    if type(f) == "function" then
        return f(...)
    else
        return f.action(...)
    end
end


local Implementations = {}

Implementations["say"] = Instruction:new {
    name = "say",
    action = function (arguments)
        local content = string.gsub(arguments.content, "<<(%w+)>>", Core.ScriptEnvironment)

        if arguments["character"] ~= nil then
            Engine.SetDialogName(arguments.character.dialogName)
        end

        Engine.SetDialogText(content)
    end 
}

Implementations["set-name"] = Instruction:new {
    name = "set-name",
    action = function (arguments)
        local character, name = arguments.target, arguments.name
        if getmetatable(character) ~= Core.Character then
            Engine.Log("set-name instruction must receive a Character.")
            return
        end
        
        if name == "default" then
            character.dialogName = character.firstName
        elseif name == "fullName" then
            character.dialogName = ("%s %s"):format(character.firstName, character.lastName)
        else
            character.dialogName = character[name]
        end
    end
}

Implementations["set-data"] = Instruction:new {
    name = "set-data",
    action = function (arguments)
        rawset(Core.ScriptEnvironment, arguments.name, arguments.value)
    end
}

Implementations["get-data"] = Instruction:new {
    name = "get-data",
    action = function (arguments)
        local content = rawget(Core.ScriptEnvironment, arguments.name)

        if type(content) == "function" then
            content = content()
        end

        if type(content) == "string" then
            Engine.SetDialogText(content)
        end
    end
}

Implementations["set-image"] = Instruction:new {
    name = "set-image",
    action = function (arguments)
        local targetType = getmetatable(arguments.target)

        if targetType == Core.Character then
            arguments.target.currentImage = arguments.image
        elseif targetType == Core.Scene then
            arguments.target = arguments.scene
            arguments.target.image = arguments.image
            Engine.SetBackground(arguments.image)
        else
            Engine.Log("Cannot set-image for " .. targetType)
        end
    end 
}

Implementations["draw-image"] = Instruction:new {
    name = "draw-image",
    action = function (arguments)
        --[[
        if getmetatable(arguments.image) == "string" then
            if arguments["position"] ~= nil then
                arguments.image:setPosition(arguments.position)
            end

            arguments.image:draw()
        else
            draw(arguments.image,
                               arguments.location[1],
                               arguments.location[2])
        end
        --]]    
    end
}

Implementations["set-scene"] = Instruction:new {
    name = "set-scene",
    action = function (arguments)
        local target = arguments.target
        local name

        if type(target) == "string" then
            name = target
        elseif type(target) == "function" then
            name = target(arguments.scene)
            if type(name) ~= "string" then
                Engine.Log("set-scene instruction must receive a string.")
                return
            end
       else
            Engine.Log("Invalid target for set-scene: " .. target)
            return
       end

       Core.ChangeToScene(name)
    end
}

Implementations["no-op"] = Instruction:new {
    name = "no-op",
    action = function (arguments)
        Engine.Log("The engine must never generate a no-op instruction.")
    end
}

Implementations["show-menu"] = Instruction:new {
    name = "show-menu",
    action = function (arguments)
        if Core.Settings.Handlers.Menu ~= nil and
            coroutine.status(Core.Settings.Handlers.Menu) == "suspended"
        then
            coroutine.resume(Core.Settings.Handlers.Menu)
        end

        if Core.Settings.Handlers.Menu == nil or
            coroutine.status(Core.Settings.Handlers.Menu) == "dead"
        then
            Core.Settings.Handlers.Menu = coroutine.create(
                Core.Settings.MenuFunction
            )
        end
        coroutine.resume(Core.Settings.Handlers.Menu, arguments)
    end
}

Implementations["set-position"] = Instruction:new {
    name = "set-position",
    action = function (arguments)
        arguments.target.position = arguments.position
    end
}

Implementations["execute"] = Instruction:new {
    name = "execute",
    action = function (arguments)
        if type(arguments.content) == "function" then
            arguments.content()
        end
    end
}

Instruction.ForOpcode = {
    ["say"]                  = Implementations["say"],
    ["think"]                = Implementations["say"],
    ["set-character-image"]  = Implementations["set-image"],
    ["set-character-name"]   = Implementations["set-name"],
    ["change-scene"]         = Implementations["set-scene"],
    ["set-scene-image"]      = Implementations["set-image"],
    ["no-op"]                = Implementations["no-op"],
    ["deactivate-character"] = Implementations["no-op"],
    ["move-character"]       = Implementations["set-position"],
    ["add-menu"]             = Implementations["show-menu"],
    ["export-variable"]      = Implementations["set-data"],
    ["import-variable"]      = Implementations["get-data"],
    ["execute"]              = Implementations["execute"]
}


return Instruction
