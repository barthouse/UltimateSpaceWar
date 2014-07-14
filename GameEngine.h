#pragma once

//
// SimulateAndDraw() gets called repeatedly.  The return value is
// true if the simulation should continue (i.e get called again).
//
bool SimulateAndDraw();

//
// Initialize the game engine getting ready to run simulation.
//

void InitializeSimulation();

//
// When the simulation is over (indicated by SimulateAndDraw returning
// false), ExitSimulation() will get called to allow the game engine to
// do any final clean up.
//
void ExitSimulation();

