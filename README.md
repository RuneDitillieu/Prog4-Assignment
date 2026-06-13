# QBert - Minigin

This project is a recreation of the Q*Bert arcade game, build on top of Minigin, a small baseline for a 2D C++ game engine using SDL3 and glm.  
This engine is based on the Game Programming Patterns book, implemented over half of the patterns discussed.  
The main pattern that was built around was the component over inheritance pattern.


# Engine Architecture

### Components

As mentioned before, this engine is entirely built on the concept of components instead of inheritance, this means every gameobject has a list of components, each defining a small part of its behavior, allowing you to easily mix match components to create new and unique behaviors.  

### Scenegraph

The scene manager holds a vector of scenes, and an index to know which one is currently active.  It then updates and renders only the active scene.  
When creating a new scene, you give it a name - an unsigned short made readable through a custom enum in the game layer - and a loadfunction.  
When switching to a new scene, it'll load the new one - initializing all objects -, call start on all those objects, and then unload the old scene.  
  
Every scene has a list of objects it holds.  It'll update them one by one, call lateupdate on them, and render them.  
To protect the vector when looping over all objects, it only deletes objects that we requested to be removed *after* going through the entire list, and stops looping when a new object was created, as that usually invalides the vector during that frame.  
It also makes use of a dirty flag to re-order objects when their render priority changes.  
  
Every object also has a list of children it updates, ensuring they get rendered and updated together.  
These children are owned by their parent, not the scene.  

### Event Handlers

This project makes use of the observer pattern, allowing you to easily notify other objects of events that happened, without needing a direct reference to them.  
This way the objects aren't in control of behavior they shouldn't be.  
When an object needs to be notified of certain events, you simply add it as an observer to the objects it's interested in.  
The sound system also makes use of an event queue: when you request to play a sound, it'll add it to the queue and load it on a seperate thread.  
  
This project also implemented the command pattern, making it very easy to connect keybinds and executions.  
The command sends an event, and the connected observer executes its behavior.  

### State

Instead of keeping a bunch of booleans to keep track of state, gameobjects that need state get an actor component.  
Each actor component holds 1 state variable.  State variables are seperate objects that return the new state when a change in state is needed.  
The actor component updates the current state, checks if it returned a new state, and then transitions to the new one when applicable.  
The same thing happens with events: the actor component receives an event, passes it along to the current state, and switches state if it returns a new one.  
This way it's a lot easier to keep track of what needs to happen when: you don't need a massive if-else statement, and every state is named.  
Because the state variable return the new state whenever it finishes, you don't need to worry about accidentally switching state too early or late.  
Each state also has an OnEnter and OnExit, to make sure state transitions happen smoothly and correctly.  

# Game 

### Gamemodes

Upon launching the game, you get to choose between 3 gamemodes: singleplayer: the normal version, coop: where you can play together at the same time, working together to complete the level while still having individual scores, 
or versus: where one player plays as normal, and one player takes control of coily, one of the enemies.  
The first player can always play on keyboard and controller, while the second player can only play on controller.  
When not in singleplayer mode, the first player will get connected to the second controller, making it so you can still play together while only having one controller.  

### Keybinds

Going or down in the selection screen or highscore screen is done by either pressed 'W' and 'S' or the up arrow and down arrow on keyboard, 
and with the D-Pad on controller.  To confirm your selection, press space or return on keyboard, and 'A' or 'B' on controller.  
To go back to the start screen from the highscore screen, press escape or start.  
Moving around during the game is done through 'wasd' or the D-Pad, which are the only controls you need.  
You can press F1 to skip a round, and F2 to mute or unmute sound.  

### Level setup

The level layouts are read from a json file.  All levels already have the same pre-defined structure, but everything that varies from level to level (or round to round) is read from a file.  
This includes the colors and mechanics of the tiles, the positions of disks, and the timings for enemy spawning.  

# Repository

https://github.com/RuneDitillieu/Prog4-Assignment