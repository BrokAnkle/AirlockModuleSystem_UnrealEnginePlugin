# AirLockSystem_UnrealEnginePlugin
Welcome to the open sourced Airlock Module System plugin for Unreal Engine. This plugin is contains only code and a level sample for demo.
The system is entirely event based and use event dispatchers to signal the state of the module, which makes it easy to understand and to override.
Currently, the code is only Blueprint but I will soon make it C++. The few assets used are the base Unreal Engine mesh ones.

DOCUMENTATION COMING SOON !
Quick tutorial:
1-Place the doors you want in your level.
2-Select the door type in the Details pannel, category Airlock System. There 3 types : Rotating, Elevating and Sliding. The type just determine what animation the door does when opening/closing. If you want to do your own, add your type inside Utilis/DoorType enum then add your logic inside the Door Actor BP (use the current existing ones as exemple).
3-Add an Ailock Module where you will need the player to interact/see what is happening with the pressurization.
4-In the Details pannel, add all the doors that needs to be connected to this module in the Doors array.
5-Select the unlock method and other options in the Details pannel to customize your module.
6-If you don't have it already, add a Widget Interaction component to your character, as a child of the camera. 
7-Add an input action and bind the Pressed to Press Pointer Key and the Released to Release Pointer Key of the Widget Interaction component. The key should be Left Mouse Button. This is for interaction with the physical widgets.
6-Click play and VOILÀ!

If any problem, suggestion, bugs etc... Please contact me.
