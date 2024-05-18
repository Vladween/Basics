#include "OfficeBasics.h"
#include "Resources.h"


class SpaceFlyers : public basics::Application
{
protected:
	void init() override
	{
		createWindow({ 1000, 650 }, "Space Flyers");
	}
};

int main()
{
	init_resources();
	RUN(SpaceFlyers);
	return 0;
}