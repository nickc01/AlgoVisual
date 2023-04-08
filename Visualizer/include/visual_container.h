#pragma once

template<typename T>
struct visual_container {
    T value;
    mutable float x = 0;
    mutable float y = 0;

    mutable float targetX = 0;
    mutable float targetY = 0;

    visual_container(float Value, float X, float Y) : value(Value), x(X), y(Y), targetX(X), targetY(Y) {}
    visual_container(float Value) : value(Value), x(0), y(0), targetX(0), targetY(0) {}


    bool operator!=(const visual_container<T>& rhs) const
	{
		return value != rhs.value || targetX != rhs.targetX || targetY != rhs.targetY;
	}

	bool operator==(const visual_container<T>& rhs) const
	{
		return value == rhs.value && targetX == rhs.targetX && targetY == rhs.targetY;
	}
};