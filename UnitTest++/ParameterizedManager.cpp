#include "ParameterizedManager.h"


using namespace std;
using namespace UnitTest;


ParameterizedManager::ParameterizedManager()
{
}


ParameterizedManager::~ParameterizedManager()
{
}


ParameterizedManager & ParameterizedManager::getInstance()
{
	static ParameterizedManager stored;
	return stored;
}
