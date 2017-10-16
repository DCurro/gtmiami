#ifndef DRIVERMIND_HPP_
#define DRIVERMIND_HPP_

class DriverBody;

class DriverMind {
public:
	DriverMind();
	virtual ~DriverMind();

	virtual void drive() = 0;
};

#endif /* DRIVERMIND_HPP_ */
