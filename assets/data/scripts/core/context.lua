local Context = {}
Context.__index = Context


function Context:new()
    local context = setmetatable({}, Context)

    -- Таблица с данными контекста, которая позволяет общаться скриптам с внешним миром
    context.data = setmetatable({}, { __mode = "v" })

    return context
end

function Context:set(name, value)
    self.data[name] = value
end

function Context:remove(name)
    self.data[name] = nil
end

function Context:get(name)
    return self.data[name]
end

return Context