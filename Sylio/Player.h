#pragma once
#include <SFML/Graphics.hpp>
#include "Trace.h"
#include "settings.h"
#include<random>
#include <array>
#include "Boost.h"


extern Settings setting;
class Player
{
private:
	friend class Boost;
	friend class SpeedUp;
	friend class SlowDown;
	friend class GrowUp;
	friend class Shrink;
	friend class LockLeft;
	friend class LockRight;
	friend class Freeze;
	friend class SwitchControls;
	friend class Blind;
	static double rScan;

	int points;
	int playerId;
	int nextGap;
	int gapSize;
	int actualGap;

	double angle;
	double angleVelocity;
	double velocity;
	double hiddenVelocity;
	double headR;
	double hiddenHeadR;
	double waitingR;
	double boostHeadTime;
	double boostTimeVel;
	bool resize;

	bool dead;
	bool lockLeft;
	bool lockRight;
	bool freeze;
	bool visible;
	bool brokeWalls;
	bool activeBoost;
	int& xmax;
	int& xmin;
	int& ymax;
	int& ymin;

	std::vector<Boost*> boosts;

	sf::Color blendColor;
	std::array<std::array<long long int, 1920>, 1080>& hitbox;
	sf::RenderWindow& window;
	sf::Clock time;
	sf::CircleShape head;
	sf::CircleShape boosthead;
	std::string nickname;
	sf::Color color;
	sf::Color headCol;
	sf::Keyboard::Key left;
	sf::Keyboard::Key right;
	sf::Vector2f position;
	sf::Vector2f oldPosition;
	sf::Vector2i gapBounds;
	sf::Vector2i NextGapounds;
	sf::Vector2i roundPosition;
	std::vector<double>& headVec;
	Trace trace;
public:
	static double getRScan() { return rScan; }
	bool update();
	void draw() { if (visible) trace.draw();	window.draw(head); /*if (activeBoost) window.draw(boosthead);*/ }
	void addPoints(int p) { points += p + trace.getFragmentsSize(); }

	Player(std::vector<double>& headVec, std::array<std::array<long long int, 1920>, 1080>& hitbox_, sf::RenderWindow& win, sf::Color col, int& ymax_, int& ymin_, int& xmax_, int& xmin_);
	void reset();
	void setGapBounds(int gbx, int gby, int ngbx, int ngby) { gapBounds = { gbx,gby }; NextGapounds = { ngbx,ngby }; setNewGap(); }
	void setParams(double angle_, double R, double angvel, double vel) { angle = angle_; headR = 0; changeRadious(R); hiddenHeadR = R; angleVelocity = angvel; velocity = vel; hiddenVelocity = vel; }
	void setNick(std::string str) { nickname = str; }
	void setControls(sf::Keyboard::Key l, sf::Keyboard::Key r) { left = l; right = r; }
	void setId(int id) { playerId = id; }
	void setColor(sf::Color col) { color = col; 	headCol = col; headCol.r *= 0.8; headCol.g *= 0.8; headCol.b *= 0.8; head.setFillColor(headCol);  boosthead.setFillColor(sf::Color::White); }

	void addBoost(Boost * bost_);
	void checkBoosts();
	void die(bool x = true) { dead = true; std::cout << "before" + std::to_string(playerId) << std::endl; clearBoosts(); }
	void clearBoosts() { for (auto& x : boosts) delete x; boosts.clear(); std::cout<<"after\n"; }
	void setNewGap();
	void drawLineOnHitBox(int x1, int y1);
	void fullFillForBoost(sf::Vector2f actR, sf::Vector2f actL, sf::Vector2f lasR, sf::Vector2f lasL);
	void fullFillResizeR(double R);
	bool Scan();
	void updateTrace();
	void erise();
	void setWaitingR(double r) { waitingR = r; resize = true; }
	void roundPos() { roundPosition.x = round(position.x); roundPosition.y = round(position.y); }
	bool changeRadious(double R);
	bool getState() { return dead; }
	bool triangleTest(sf::Vector2i s, sf::Vector2i a, sf::Vector2i b, sf::Vector2i c);
	sf::Vector2f& getPos() { return position; }
	double getRadious() { return headR; }
	int getPoints() { return points; }

	inline void setPosition(sf::Vector2f pos) {
		position = pos;
		head.setPosition(position);
		boosthead.setPosition(position);
		oldPosition = position;
		float pointlx = headR * sin(angle + NINETY_DEG);
		float pointly = headR * cos(angle + NINETY_DEG);
		sf::Vector2f xx(position.x + pointlx, position.y + pointly);
		sf::Vector2f yy(position.x - pointlx, position.y - pointly);
		trace.update(xx, yy);
		trace.edge(true, headR, angle, position);
		fullFillResizeR(headR);
		trace.start();
	}
	bool checkBounds() {
		if (position.x - headR < xmin || position.x + headR > xmax || position.y + headR > ymax || position.y - headR < ymin) { 
			die(); 
			return true; 
		} 
		return false;
	}

	~Player();
};

