#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"


#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class ModuleEntities;
class ModuleCollision;
class ModulePathfinding;
class ModulePathfindingWalker;
class j1Gui;
class j1Fonts;
class j1StartMenu;
class j1CreditsScene;
class j1SettingsScene;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

	pugi::xml_node j1App::LoadEntities(pugi::xml_document& config_file) const;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;
	

public:

	// Modules
	j1Window*					win = nullptr;
	j1Input*					input = nullptr;
	j1Render*					render = nullptr;
	j1Textures*					tex = nullptr;
	j1Audio*					audio = nullptr;
	j1Scene*					scene = nullptr;
	j1Map*						map = nullptr;
	ModuleEntities*				entities = nullptr;
	ModuleCollision*			collision = nullptr;
	ModulePathfinding*			pathfinding = nullptr;
	ModulePathfindingWalker*	pathfindingWalker= nullptr;
	j1Gui*						gui = nullptr;
	j1Fonts*					fonts = nullptr;
	j1StartMenu*				startmenu = nullptr;
	j1CreditsScene*				creditsscene = nullptr;
	j1SettingsScene*			settingsscene = nullptr;


	uint32				frame_limit = 30;

	float				dt = 0.0f;

private:

	p2List<j1Module*>	modules;
	uint				frames = 0;
	//float				dt = 0.0f;
	int					argc = 0;
	char**				args = nullptr;

	p2SString			title = nullptr;
	p2SString			organization = nullptr;
	p2SString			vsyncState = nullptr;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game = nullptr;
	mutable p2SString	save_game = nullptr;

	bool				fpshigh = false;
	j1PerfTimer			ptimer;
	j1PerfTimer			dttimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
};

extern j1App* App;

#endif