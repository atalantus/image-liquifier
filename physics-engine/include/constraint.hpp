#pragma once

struct BoxConstraint
{
    float right, left, top, bottom;

    BoxConstraint(float right, float left, float top, float bottom)
            : right{right}, left{left}, top{top}, bottom{bottom}
    {}
};