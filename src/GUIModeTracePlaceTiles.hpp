#ifndef GUIMODETRACEPLACETILES_HPP_
#define GUIMODETRACEPLACETILES_HPP_

#include "GUIMode.hpp"

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "common.hpp"

class PlayContext;

class GUIModeTracePlaceTiles: public GUIMode {
private:
	std::string mCategoryString;
	int mTileSelectedID;

	bool mIsSelecting;

	sf::Vector2f mStartLocation;
	sf::Vector2f mEndLocation;

public:
	GUIModeTracePlaceTiles(PlayContext* playContext, std::string categoryString, int tileSelectedID);
	virtual ~GUIModeTracePlaceTiles();

	virtual void initialize() override;
	virtual void handleEvent(sf::Event event) override;
	virtual void destroy() override;
	virtual std::string description() override;

private:
	void setTiles();
	void setTile(int floor, sf::Vector2f location);
};

#endif /* GUIMODETRACEPLACETILES_HPP_ */
