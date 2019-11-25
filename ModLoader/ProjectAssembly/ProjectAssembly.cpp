#include <stdafx.h>
#include <string>
#include <game/Global.h>
#include <game/Input.h>
#include <game/SDKHooks.h>
#include <mod/Mod.h>
#include <HookLoaderInternal.h>
#include <mod/ModFunctions.h>
#include <mod/MathFunctions.h>
#include <util/JsonConfig.h>
#include <assets/AssetFunctions.h>
#include <assets/FObjectSpawnParameters.h>
#include "../Detours/src/detours.h"
#include "../SatisfactorySDK/SDK.hpp"
#include <memory/MemoryObject.h>
#include <memory/MemoryFunctions.h>

/*
EXAMPLEMOD
==========
ExampleMod is a documented mod that uses all the available features of SML and can help you mod.
If you are confused about what to do, just type Functions:: and autocomplete will give you options on some functions that you can use.
If you don't know how to do something/use something, simply hover over it to read the provided documentation. If you're still confused, come to the discord and ask in #sml-help about your issue.
SuperCoder will help you with your problem.
*/

// Use the namespaces so you have to type less stuff when adding on to your mod
using namespace SML::Mod;
using namespace SML::Objects;

// Version of SML that this mod was compiled for.
#define SML_VERSION "1.0.2"

// define the mod name for easy changing and simple use
#define MOD_NAME "ProjectAssembly"

// Define logging macros to make outputting to the log easier

//log an informational message to the console
#define LOG(msg) SML::Utility::infoMod(MOD_NAME, msg)

//log an informational message to the console
#define INFO(msg) LOG(msg)

//log a warning message to the console
#define WARN(msg) SML::Utility::warningMod(MOD_NAME, msg)

//log an error message to the console
#define ERR(msg) SML::Utility::errorMod(MOD_NAME, msg)

bool SML::debugOutput = false;

// Config
json config = SML::Utility::JsonConfig::load(MOD_NAME, {
	{
		
	}
});

// Global variables required by the mod
AFGPlayerController* player;

// A custom event handler for when ExampleMod's post setup is complete.
// Other mods can also make a handler for ExampleMod_PostSetupComplete if they want to do something when the event is broadcast.
void postSetupComplete() {
	LOG("Project Assembly post setup has been completed!");
}

// information about the mod
Mod::Info modInfo {
	// Target sml version
	SML_VERSION,

	// Name
	MOD_NAME,

	// Version
	"1.0.0",

	// Description
	"Project Assembly",

	// Authors
	"iamlegend_tv, Mrhid6",

	// Dependencies
	// Place mod names that you want to ensure is loaded in this vector. If you place an asterisk (*) before the mod name, it will be loaded as an optional dependency instead.
	{}
};

// The mod's class, put all functions inside here
class ProjectAssembly : public Mod {

	// Function to be hooked
	// Every hook has two parameters at the start, even when the target function does not have any parameters.
	// The first is a pointer to ModReturns, which allows you to disable SML calling the function after your hook.
	// The second is a pointer to an object of the base class of the function, which in this case is AFGPlayerController.
	void beginPlay(Functions::ModReturns* modReturns, AFGPlayerController* playerIn) {
		LOG("Project Assembly adding recipes...");

		LOG("Got Player");
		player = playerIn;
	}

public:
	// Constructor for SML usage. Do not put anything in here, use setup() instead.
	ProjectAssembly() : Mod(modInfo) {
	}

	// The setup function is the heart of the mod, where you hook your functions and register your commands and API functions. Do not rename!
	void setup() override {
		// Use the placeholders namespace
		using namespace std::placeholders;

		SDK::InitSDK(); //Initialize the SDK in ExampleMod so the functions work properly

		Functions::setDependsOnPak("SID_p");
		Functions::setDependsOnPak("ProjectAssembly_p");

		// More on namespaces:
		// * The functions that will be of use to you are in the SML::Mods::Functions namespace. A tip is to type Functions:: and see what functions are available for you to use. 

		// Hook a member function as handler
		::subscribe<&AFGPlayerController::BeginPlay>(std::bind(&ProjectAssembly::beginPlay, this, _1, _2)); //bind the beginPlay function, with placeholder variables
		// Because there are two inputs to the function, we use _1 and _2. If there were 3 inputs, we would use _1, _2, and _3, and so forth.

		// Hook a lambda with captured this-ptr as handler
		::subscribe<&PlayerInput::InputKey>([this](Functions::ModReturns* modReturns, PlayerInput* playerInput, FKey key, InputEvent event, float amount, bool gamePad) {
			return false;
		});

		// Tick functions are called every frame of the game. BE VERY CAREFUL WHEN USING THIS FUNCTION!!! Putting a lot of code in here will slow the game down to a crawl!
		::subscribe<&UWorld::Tick>([this](Functions::ModReturns*, UWorld* world, ELevelTick tick, float delta) {
			// If you abuse this, I will find you, and I will... uh... do something... and you won't like it
			//LOG("test");
		});
		
		//Here, we do some registring. Registring must be done in setup to make sure that the registration will be available for later use.

		//Register a custom event. This does not call the event, you have to do that with Functions::broadcastEvent.
		Functions::registerEvent("ProjectAssembly_PostSetupComplete", postSetupComplete);


		LOG("Finished ProjectAssembly setup!");
	}

	//The postSetup function is where you do things based on other mods' setup functions
	void postSetup() override {
		// Write things to be done after other mods' setup functions
		// Called after the post setup functions of mods that you depend on.

		// Check if mod "RandomMod" is loaded
		if (Functions::isModLoaded("RandomMod")) {
			LOG("Random mod is loaded!");
			// Grab the raw function pointer for "NonexistantFunction"
			PVOID nonexistantFuncRaw = Functions::getAPIFunction("NonexistantFunction");
			// Cast the raw function pointer into a usable function
			auto nonexistantFunc = (void(WINAPI*)())nonexistantFuncRaw;
			// Call the nonexistant function
			nonexistantFunc();
			// Sidenote: this code should crash because it is trying to access a function that is not registered, but it does not because we ensure the code is only run when "RandomMod" is loaded.
		}

		//Broadcast the event for ExampleMod and other mods that do something for ExampleMod_PostSetupComplete.
		Functions::broadcastEvent("ProjectAssembly_PostSetupComplete");

		//Broadcast an event from RandomMod. Since this doesn't exist, nothing will be done and no error will be thrown.
		Functions::broadcastEvent("RandomMod_SomeEvent");
	}

	~ProjectAssembly() {
		// Cleanup
		LOG("ProjectAssembly finished cleanup!");
	}
};

// Required function to create the mod, do not rename!
MOD_API Mod* ModCreate() {
	return new ProjectAssembly();
}