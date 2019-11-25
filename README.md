# Project Assembly
Mod for Satisfactory (WIP)

Provides a resource-sink of sorts that edds potentially endless content to the game through unlockables and missions.
Relies on it's own progression system through currency, which can be spent to unlock everything in the mod, and is earned through rocket launches to sell goods to FICSIT.

Currently a WIP by iamLegend and Mrhid6



## How to build
* git pull
* git submodule update --init --remote --recursive

## How to compile
* Open `ModLoader/ProjectAssembly.sln`
* Rebuild Solution

## How To Pak mod
* run `Add-ModToGame.ps1` to create config file **PAK.config.xml**
* Edit **PAK.config.xml** 
* Mod Will create zip in `ZipDir`
