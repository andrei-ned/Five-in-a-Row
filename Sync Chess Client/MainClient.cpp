#include "Application.h"
#include "Constants.h"

int main()
{
	Application app(Constants::windowWidth, Constants::windowHeight);
	app.run();

	return 0;
}