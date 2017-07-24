local Position = {}
Position.__index = Position

--     +----------------------------------------------------------+
--     | TopLeft               TopCenter                 TopRight |
--     |                                                          |
--     |                                                          |
--     |                                                          |
--     |                                                          |
--     | Left                   Center                      Right |
--     |                                                          |
--     |                                                          |
--     |                                                          |
--     |                                                          |
--     | BottomLeft          BottomCenter             BottomRight |
--     +----------------------------------------------------------+

local ValidPositions = {
    "BottomCenter",
    "Center",
    "TopCenter",

    "BottomRight",
    "Right",
    "TopRight",

    "BottomLeft",
    "Left",
    "TopLeft",
}

for _,name in ipairs(ValidPositions) do
    Position[name] = name
end

return Position