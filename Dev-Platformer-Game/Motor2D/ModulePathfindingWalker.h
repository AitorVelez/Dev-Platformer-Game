#ifndef __MODULEPATHFINDINGWALKER_H__
#define __MODULEPATHFINDINGWALKER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "ModuleEntities.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

// --------------------------------------------------
// Recommended reading:
// Intro: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Details: http://theory.stanford.edu/~amitp/GameProgramming/
// --------------------------------------------------

class ModulePathfindingWalker : public j1Module
{
public:

	ModulePathfindingWalker();

	// Destructor
	~ModulePathfindingWalker();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	// To request all tiles involved in the last generated path
	const p2DynArray<iPoint>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

private:

	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created path here
	p2DynArray<iPoint> last_path;
};

// forward declaration
struct PathListWalker;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNodeWalker
{
	// Convenient constructors
	PathNodeWalker();
	PathNodeWalker(int g, int h, const iPoint& pos, const PathNodeWalker* parent);
	PathNodeWalker(const PathNodeWalker& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathListWalker& list_to_fill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	// -----------
	int g;
	int h;
	iPoint pos;
	const PathNodeWalker* parent; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathListWalker
{
	// Looks for a node in this list and returns it's list node or NULL
	p2List_item<PathNodeWalker>* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	p2List_item<PathNodeWalker>* GetNodeLowestScore() const;

	// -----------
	// The list itself, note they are not pointers!
	p2List<PathNodeWalker> list;
};



#endif // __MODULEPATHFINDINGWALKER_H__