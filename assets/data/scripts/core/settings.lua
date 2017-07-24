local Settings = {}
Settings.__index = Settings


--[[
Settings.Screen = {}
Settings.Screen.Width = Engine.GetScreenWidth()
Settings.Screen.Height = Engine.GetScreenHeight()
Settings.Screen.Fullscreen = false

Settings.Screen.Center = {
    Settings.Screen.Width / 2,
    Settings.Screen.Height / 2,
}
--]]


-- Таблица с обработчиками
Settings.Handlers = {}

-- Обработчик сцены, будет вызываться при вызове
-- Core.CurrentScene:draw()
Settings.Handlers.Scene = nil

-- Обработчик смены сцены, будет вызываеться при вызове
-- ChangeToScene(..)
Settings.Handlers.ChangeScene = nil

-- Обработчик персонажа, будет вызываеться при вызове 
-- Caracter:draw()
Settings.Handlers.Character = nil


-- Обработчик меню
-- Когда игрок доходит до меню в сцене, Core автоматически вызовет
-- корутину с аргументом типа Core.Menu, который является тем самым
-- объектом меню. Core ожидает строку выбранного ответа.
function Settings.MenuFunction(data)
    local arguments = data
    for _,choice in pairs(arguments.menu.choices) do
        Engine.AddMenuItem(choice.label)
    end
    Engine.ShowMenu()
    
    coroutine.yield()

    arguments.menu.currentChoiceIndex = Engine.GetSelectedItem()
         
    content = arguments.menu:getCurrentChoiceAction()(arguments.scene)
    
    local opcodes = nil
    if type(content) == "string" then
        opcodes = Core.Opcode:new("change-scene", {target=content})
    else
        opcodes = Core.Scene.createOpcodeFromContent(content)
    end

    local newIndex = arguments.scene.opcodeIndex + 1

    if rawget(opcodes, "__flatten") == true then
        for _,op in ipairs(opcodes) do
            table.insert(arguments.scene.opcodes, newIndex, op)
            newIndex = newIndex + 1
        end
    else
        table.insert(arguments.scene.opcodes, newIndex, opcodes)
    end

    Core.Forward()
    Core.Update()
    Core.Update()
end

Settings.Handlers.Menu = coroutine.create(Settings.MenuFunction)

return Settings
