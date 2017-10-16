/*
 Copyright (C) 2011 Allen Jordan ()
 Copyright (C) 2011 Xabier Larrakoetxea (slok)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOX2DDEBUGDRAW_HPP
#define BOX2DDEBUGDRAW_HPP

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Globals.h"		//TODO: replace with Distict, and common

#include "common.hpp"

class Camera;

class Box2DDebugDraw : public b2Draw
{
public:
    Box2DDebugDraw(sf::RenderTarget &window, Camera *camera);
    virtual ~Box2DDebugDraw();
    
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    sf::Color box2dColorToSfColor(const b2Color &color, int alpha);
    
private:
    sf::RenderTarget *window;
    Camera *camera;
    
    uint32 mPreviousFlags = 0;
    
    bool isAtLeastOneVertiexOnScreen(const b2Vec2* vertices, int32 vertexCount);
    bool isCircleOnScreen(const b2Vec2& center, float32 radius);
};

#endif // DEBUGDRAW_HPP