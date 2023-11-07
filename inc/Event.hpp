#pragma once


class Event
{
public:
    enum EventType
    {
        Closed, 
        MouseMoved,
        MouseScrolled
    };
    EventType type;

    struct MouseMoveEvent
    {
        float x, y;
    };

    struct MouseScrollEvent
    {
        float x, y;
    };

    struct MouseButton
    {
        unsigned int code;
        float x, y;
    };

    struct KeyPressed
    {
        unsigned int code;
    };

    union
    {
        MouseMoveEvent mouseMove;
        MouseScrollEvent mouseScroll;
    };
};  