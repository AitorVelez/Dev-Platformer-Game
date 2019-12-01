#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "ModulePathfindingWalker.h"
#include "Brofiler/Brofiler.h"

ModulePathfindingWalker::ModulePathfindingWalker() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

ModulePathfindingWalker::~ModulePathfindingWalker()
{
	RELEASE_ARRAY(map);
}


bool ModulePathfindingWalker::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}


void ModulePathfindingWalker::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}


bool ModulePathfindingWalker::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}


bool ModulePathfindingWalker::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}


uchar ModulePathfindingWalker::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}


const p2DynArray<iPoint>* ModulePathfindingWalker::GetLastPath() const
{
	return &last_path;
}

p2List_item<PathNodeWalker>* PathListWalker::Find(const iPoint& point) const
{
	p2List_item<PathNodeWalker>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}


p2List_item<PathNodeWalker>* PathListWalker::GetNodeLowestScore() const
{
	p2List_item<PathNodeWalker>* ret = NULL;
	int min = 65535;

	p2List_item<PathNodeWalker>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}


PathNodeWalker::PathNodeWalker() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNodeWalker::PathNodeWalker(int g, int h, const iPoint& pos, const PathNodeWalker* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNodeWalker::PathNodeWalker(const PathNodeWalker& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNodeWalker::FindWalkableAdjacents(PathListWalker& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfindingWalker->IsWalkable(cell))
		list_to_fill.list.add(PathNodeWalker(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfindingWalker->IsWalkable(cell))
		list_to_fill.list.add(PathNodeWalker(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfindingWalker->IsWalkable(cell))
		list_to_fill.list.add(PathNodeWalker(-1, -1, cell, this));

	return list_to_fill.list.count();
}


int PathNodeWalker::Score() const
{
	return g + h;
}


int PathNodeWalker::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}


int ModulePathfindingWalker::CreatePath(const iPoint& origin, const iPoint& destination)
{
	BROFILER_CATEGORY("PathfindingWalkerCreatePath", Profiler::Color::Violet)
	last_path.Clear();
	
	if (!IsWalkable(origin) || !IsWalkable(destination))
		return -1;



	PathListWalker open;
	PathListWalker close;

	PathNodeWalker node(0, origin.DistanceNoSqrt(destination), origin, NULL);

	open.list.add(node);

	while (open.list.count() > 0)
	{

		close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());
		if (close.list.end->data.pos == destination)
		{
			p2List_item<PathNodeWalker>* iterator = close.list.end;
			for (; iterator->data.parent != nullptr; iterator = close.Find(iterator->data.parent->pos))
			{
				last_path.PushBack(iterator->data.pos);
				if (iterator->data.parent == nullptr)
					last_path.PushBack(close.list.start->data.pos);
			}
			last_path.Flip();
			return last_path.Count();
		}
		else
		{
			PathListWalker neighbours;
		
			close.list.end->data.FindWalkableAdjacents(neighbours);
			p2List_item<PathNodeWalker>* iterator = neighbours.list.start;
			for (iterator; iterator != NULL; iterator = iterator->next)
			{
				if (close.Find(iterator->data.pos))
				{
					continue;
				}
				else if (open.Find(iterator->data.pos))
				{
				
					PathNodeWalker open_node = open.Find(iterator->data.pos)->data;
					iterator->data.CalculateF(destination);
					if (open_node.g > iterator->data.g)
					{
				
						open_node.parent = iterator->data.parent;
					}
				}
				else
				{
					iterator->data.CalculateF(destination);
					open.list.add(iterator->data);
				}
			}
			neighbours.list.clear();
		}
	}

	return last_path.Count();
}