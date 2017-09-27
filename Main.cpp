#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "TimeManager.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#ifdef _DEBUG
#include "mmgr/mmgr.h"
#endif


enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_RESTART,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;


int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	int update_return = UPDATE_CONTINUE;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:

			update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}
			else if (update_return == UPDATE_STOP)
			{
				state = MAIN_FINISH;
			}
			else if (update_return == UPDATE_RESTART)
			{
				state = MAIN_RESTART;
			}

			break;

		case MAIN_RESTART:

			update_return = App->Restart();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Restart exits with ERROR");
				state = MAIN_EXIT;
			}
			else if (update_return == UPDATE_STOP)
			{
				state = MAIN_FINISH;
			}
			else if (update_return == UPDATE_CONTINUE)
			{
				state = MAIN_UPDATE;
			}

			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;
			break;

		}
	}

	delete App;

#ifdef _DEBUG
	int leaks = MAX(0, m_getMemoryStatistics().totalAllocUnitCount - 23);
	LOG("With %d memory leaks!\n", (leaks>0) ? leaks : 0);
#endif
	return main_return;
}