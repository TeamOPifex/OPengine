
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <myo.hpp>
myo::Myo* myoDevice;

void OPmyoConnect() {
	myo::Hub hub;
	myoDevice = hub.waitForAnyMyo(10000);
	if (!myoDevice) {
		throw std::runtime_error("Unable to find a Myo!");
	}
}