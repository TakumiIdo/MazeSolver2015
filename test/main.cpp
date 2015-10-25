#include <stdio.h>
#include <stdint.h>

#include <list>
#include <vector>
#include <unistd.h>

#include "MazeSolver_conf.h"
#include "Maze.h"
#include "mazeData.h"
#include "ShortestPath.h"
#include "Agent.h"


void test_Size()
{
	printf("%lu \n", sizeof(Direction)); //1
	printf("%lu \n", sizeof(Maze)); //512
	printf("%lu \n", sizeof(ShortestPath)); //1112
	printf("%lu \n", sizeof(Path)); //24
	printf("%lu \n", sizeof(std::vector<Path>)); //24
	printf("%lu \n", sizeof(Agent)); //1168
}

void test_Maze(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);
	field.printWall();
}

void test_ShortestPath(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);

	ShortestPath path(field);
	std::list<IndexVec> goalList;
	goalList.push_back(MAZE_GOAL1);
	goalList.push_back(MAZE_GOAL2);
	goalList.push_back(MAZE_GOAL3);
	goalList.push_back(MAZE_GOAL4);
	path.calcShortestDistancePath(IndexVec(0,0),goalList,false);
	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto index : path.getShortestDistancePath())
	{
		route[index.y][index.x] = true;
	}
	field.printWall(route);
}

void test_KShortestPath(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);

	ShortestPath path(field);
	std::list<IndexVec> goalList;
	goalList.push_back(MAZE_GOAL1);
	goalList.push_back(MAZE_GOAL2);
	goalList.push_back(MAZE_GOAL3);
	goalList.push_back(MAZE_GOAL4);
	path.calcKShortestDistancePath(IndexVec(0,0), goalList, 2, false);

	int cnt = 0;
	for (auto &p : path.getKShortestDistancePath()) {
		cnt++;
		printf("path#%d length %lu\n", cnt, p.size());
		bool route[MAZE_SIZE][MAZE_SIZE] = {false};
		for (auto &index : p) {
			route[index.y][index.x] = true;
		}
		//field.printWall(route);
	}
	printf("found %lu route\n", path.getKShortestDistancePath().size());
}

void test_ShortestPathInTime(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);

	ShortestPath path(field);
	std::list<IndexVec> goalList;
	goalList.push_back(MAZE_GOAL1);
	goalList.push_back(MAZE_GOAL2);
	goalList.push_back(MAZE_GOAL3);
	goalList.push_back(MAZE_GOAL4);
	path.calcShortestTimePath(IndexVec(0,0), goalList, 20, false);

	auto &p = path.getShortestTimePath();
	printf("length %lu\n", p.size());
	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto &index : p) {
		route[index.y][index.x] = true;
	}
	field.printWall(route);
}

void test_Agent(const char *filename)
{
	Maze field;
	Maze mazeInRobot;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);

	Agent agent(mazeInRobot);

	IndexVec cur(0,0);
	while(1) {
		bool pos[MAZE_SIZE][MAZE_SIZE] = {false};
		pos[cur.y][cur.x] = true;
		mazeInRobot.printWall(pos);

		agent.update(cur, field.getWall(cur));
		if (agent.getState() == Agent::FINISHED) break;

		Direction dir = agent.getNextDirection();
		for (int i=0;i<4;i++) {
			if (dir[i]) cur += IndexVec::vecDir[i];
		}
		//usleep(1000000/20);
	}

	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto &index : agent.getShortestPath()) {
		route[index.y][index.x] = true;
	}
	mazeInRobot.printWall(route);
}


int main(int argc, char **argv)
{
	if ( argc != 2) {
		printf("error\n");
		return 1;

	}

	//test_Size();
	test_Agent(argv[1]);
	//test_ShortestPath(argv[1]);
	//test_KShortestPath(argv[1]);
	//test_ShortestPathInTime(argv[1]);

	printf("finish\n");

	return 0;
}
