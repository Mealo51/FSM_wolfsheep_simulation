AI Programming 1 5SD806 04808 VT2026 
Assignment 3
Name: Mingxuan Guo
email: mingxuan.guo.3085@student.uu.se (school) , methanol51@gmail.com (personal)

brief description: 
Simulated world with sheep, wolf, grass, and manure. 
The window is 1024x1024, and the grass is placed in a 16x16 grid (256cells, 64.f length each)
The grass is randomly generated (growing/grown/wilting (haven't add dirt to the start yet)). There is 6 sheep in the beginning, and one wolf.
The sheep reproduce after reaching > 60 HP and when there's sheep nearby (also no wolf nearby), and defecate once it's full, leaving manure on the ground (for 10 seconds).
The wolf will wake up once the hunger reaches 40 (keeps increasing while sleeping and roaming/attacking, not when returning to den), and hitting (colliding) with the sheep kills the sheep and feeds the wolf hunger.

assignment3 updated features: 
    A* pathfinding for both sheep and wolf.
    edit mode where you can left click to created a blocked tile, and right click to change it back to a grass tile (dirt/growing)
    the edit mode can be toggled by pressing "E"
    there are debug drawing for the calculated path for both sheep and wolf

collaborators: Matej Cossic - discussed on impelmentation ways and SDA pattern use

use of AI: Google Gemini for debugging, Github Copilot in Visual Studio for repetitive work like auto complete and writring similar codes.