/*
    Copyright (C) 2010 Allen Jordan ()
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

#include "Box2DDebugDraw.hpp"
#include <iostream>
#include "Camera.hpp"

Box2DDebugDraw::Box2DDebugDraw(sf::RenderTarget& window, Camera *camera)
{
    this->window = &window;
    this->camera = camera;
}

Box2DDebugDraw::~Box2DDebugDraw()
{

}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{
	UNUSED(xf);
//    float x,y, lineSize, lineProportion;
//    x = xf.p.x * RATIO;
//    y = xf.p.y * RATIO;
//    lineProportion = 0.15; // 0.15 ~ 10 pixels
//    b2Vec2 p1 = xf.p, p2;
//
//	//red (X axis)
//	p2 = p1 + (lineProportion * xf.r.col1);
//    sf::Shape redLine = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, sf::Color::Red);
//
//	//green (Y axis)
//	p2 = p1 - (lineProportion * xf.R.col2);
//	sf::Shape greenLine = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, sf::Color::Green);
//
//    this->window->Draw(redLine);
//    this->window->Draw(greenLine);
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	UNUSED(p1);
	UNUSED(p2);
	UNUSED(color);
//    sf::Shape line = sf::Shape::Line(p1.x*RATIO, p1.y*RATIO, p2.x*RATIO, p2.y*RATIO, 1, this->B2SFColor(color));
//    line.EnableFill(true);
//    this->window->Draw(line);
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	UNUSED(axis);
    
    if (this->isCircleOnScreen(center, radius) == false) {
        return;
    }
    
    sf::CircleShape circle;
    circle.setRadius(radius*RATIO);
    circle.setPosition(center.x*RATIO - radius*RATIO, center.y*RATIO - radius*RATIO);

    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(this->box2dColorToSfColor(color, 255));
    circle.setFillColor(this->box2dColorToSfColor(color, 127));
    this->window->draw(circle);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	UNUSED(center);
    
    if (this->isCircleOnScreen(center, radius) == false) {
        return;
    }

    sf::CircleShape circle;
    circle.setRadius(radius*RATIO);


    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(this->box2dColorToSfColor(color, 255));
    circle.setFillColor(sf::Color::Transparent);
    this->window->draw(circle);
}


void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    if (this->isAtLeastOneVertiexOnScreen(vertices, vertexCount) == false) {
        return;
    }
    
	sf::ConvexShape polygon;
    polygon.setPointCount(vertexCount);
	for (int32 i=0; i<vertexCount; i++){
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
	polygon.setOutlineThickness(1.0f);
	polygon.setOutlineColor(this->box2dColorToSfColor(color, 255));
	polygon.setFillColor(sf::Color::Transparent);
	this->window->draw(polygon);
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    if (this->isAtLeastOneVertiexOnScreen(vertices, vertexCount) == false) {
        return;
    }
    
	sf::ConvexShape polygon;
	polygon.setPointCount(vertexCount);
	for (int32 i=0; i<vertexCount; i++){
		b2Vec2 vertex = vertices[i];
		polygon.setPoint(i, sf::Vector2f(vertex.x*RATIO, vertex.y*RATIO));
	}
	polygon.setOutlineThickness(1.0f);
	polygon.setOutlineColor(this->box2dColorToSfColor(color, 255));
	polygon.setFillColor(sf::Color::Transparent);
	this->window->draw(polygon);
}

// Private Helpers

//convert a Box2D (float 0.0f - 1.0f range) color to a SFML color (uint8 0 - 255 range)
sf::Color Box2DDebugDraw::box2dColorToSfColor(const b2Color &color, int alpha = 255) {
    sf::Color result((sf::Uint8)(color.r*255), (sf::Uint8)(color.g*255), (sf::Uint8)(color.b*255), (sf::Uint8) alpha);
    return result;
}

bool Box2DDebugDraw::isAtLeastOneVertiexOnScreen(const b2Vec2* vertices, int32 vertexCount) {
    for (int32 i=0; i<vertexCount; i++){
        b2Vec2 vertex = vertices[i];
        
        sf::Vector2f viewCenter = camera->getView().getCenter();
        sf::Vector2f viewSize = camera->getView().getSize();
        
        float left = viewCenter.x - viewSize.x/2.0f;
        float right = viewCenter.x + viewSize.x/2.0f;
        float top = viewCenter.y - viewSize.y/2.0f;
        float bottom = viewCenter.y + viewSize.y/2.0f;
        
        if (vertex.x*RATIO >= left && vertex.x*RATIO <= right && vertex.y*RATIO >= top && vertex.y*RATIO <=bottom) {
            return true;
        }
    }
    
    return false;
}

bool Box2DDebugDraw::isCircleOnScreen(const b2Vec2& center, float32 radius) {
    b2Vec2 boxAroundCircleTopLeft = b2Vec2(center.x-radius, center.y-radius);
    b2Vec2 boxAroundCircleTopRight = b2Vec2(center.x+radius, center.y-radius);
    b2Vec2 boxAroundCircleBottomLeft = b2Vec2(center.x-radius, center.y+radius);
    b2Vec2 boxAroundCircleBottomRight = b2Vec2(center.x+radius, center.y+radius);
    
    b2Vec2 vertices[4] = {boxAroundCircleBottomLeft, boxAroundCircleBottomRight, boxAroundCircleTopLeft, boxAroundCircleTopRight};
    
    return this->isAtLeastOneVertiexOnScreen(vertices, 4);
}
