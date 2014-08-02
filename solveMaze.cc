#include<bits/stdc++.h>
#include<ncurses.h>
#define x first
#define y second
#define MVPRINTW(Y,X,...) {\
	mvprintw(Y,2*X+0,__VA_ARGS__);\
	mvprintw(Y,2*X+1,__VA_ARGS__);\
}

using namespace std;
typedef pair<int,int> coord;

int height, width,
	dx[] = {0, -1, 0, 1},
	dy[] = {1, 0, -1, 0};
char **maze, **lines;

void ReadMaze(void)
{
	FILE *fp = fopen("maze.maz", "r");
	fscanf(fp, "%d%d\n", &height, &width);
	//printf("[%d %d]\n", height, width);
	maze = new char*[height];

	lines = new char*[height*2+1];
	for (int i = 0; i < 1+2*height; i++) {
		lines[i] = new char[width*2+3];
		fgets(lines[i], 2*width+3, fp);
		for (int j = 0; j < 2*width+3; j++)
			if (lines[i][j] == '\n')
				lines[i][j] = 0;
	}

	//puts("Read complete");

	for (int i = 0; i < height; i++) {
		maze[i] = new char[width];
		for (int j = 0; j < width; j++) {
			int cx = 2*j+1,
				cy = 2*i+1;
			maze[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				int nx = cx + dx[k],
					ny = cy + dy[k];
				if (lines[ny][nx] == ' ')
					maze[i][j] |= 1 << k;
			}
		}
	}

	fclose(fp);
}

void DrawField(coord current, bool **footprint)
{
	for (int i = 0; i < 1+2*height; i++) {
		for (int j = 0; j < 1+2*width; j++) {
			if (lines[i][j] == ' ') {
				attron(COLOR_PAIR(2));
				MVPRINTW(i, j, "%c", lines[i][j]);
				attroff(COLOR_PAIR(2));
			} else {
				attron(COLOR_PAIR(3));
				MVPRINTW(i, j, "%c", lines[i][j]);
				attroff(COLOR_PAIR(3));
			}
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			if (footprint[i][j]) {
				attron(COLOR_PAIR(1));
				MVPRINTW(i*2+1, j*2+2, ".");
				attroff(COLOR_PAIR(1));
			}
	}

	attron(COLOR_PAIR(4));
	MVPRINTW(current.y*2+1, current.x*2+2, "o");
	attroff(COLOR_PAIR(4));
	refresh();
	getchar();
}

int **vis;
int cur_depth;
bool **in_stack;
void DFS(int x, int y)
{
	if (vis[y][x] <= cur_depth) return;
	DrawField(coord(x, y), in_stack);
	in_stack[y][x] = 1;
	vis[y][x] = cur_depth;

	for (int i = 0; i < 4; i++) {
		if (maze[y][x] >> i & 1) {
			++cur_depth;
			DFS(x+dx[i], y+dy[i]);
			--cur_depth;
		}
	}

	in_stack[y][x] = 0;
	DrawField(coord(x, y), in_stack);
}

void BFS(int sx, int sy)
{
	queue <coord> q;
	vis[sy][sx] = 0;
	in_stack[sy][sx] = 1;
	q.push(coord(sx, sy));
	while (!q.empty()) {
		coord now = q.front(); q.pop();
		DrawField(now, in_stack);
		in_stack[now.y][now.x] = 0;
		if (now.y+1 == height && now.x+1 == width) return;
		for (int i = 0; i < 4; i++) {
			if (maze[now.y][now.x] >> i & 1) {
				int nx = now.x + dx[i],
					ny = now.y + dy[i];
				if (vis[ny][nx] > vis[now.y][now.x] + 1) {
					q.push(coord(nx, ny));
					in_stack[ny][nx] = 1;
					vis[ny][nx] = vis[now.y][now.x] + 1;
				}
			}
		}
	}

}

int main()
{
	initscr();
	curs_set(0);
	keypad(stdscr, TRUE);
	noecho();
	putenv("TERM=xterm-256color");
	start_color();
	init_pair(1, 62, 231);  //in-stack
	init_pair(2, 231, 231);  //empty
	init_pair(3, 252, 252);  //wall
	init_pair(4, 55, 231);  //current
	init_pair(5, 161, 232);  //text
	ReadMaze();

	in_stack = new bool*[height];
	vis = new int*[height];
	for (int i = 0; i < height; i++) {
		in_stack[i] = new bool[width];
		vis[i] = new int[width];
		for (int j = 0; j < width; j++) {
			in_stack[i][j] = false;
			vis[i][j] = 1 << 29;
		}
	}

	attron(COLOR_PAIR(5));
	mvprintw(3, 3, "Press 1 to run DFS");
	mvprintw(4, 3, "Press 2 to run BFS");
	attroff(COLOR_PAIR(5));
	refresh();

	char menu;
	for (menu = 0; (menu = getchar()) != '1' && menu != '2';);

	if (menu == '1') {

		attron(COLOR_PAIR(5));
		mvprintw(height*2+2, 2, "[..] : Points in stack");
		mvprintw(height*2+3, 2, "[oo] : Current Point");
		attroff(COLOR_PAIR(5));

		DFS(0, 0);
		attron(COLOR_PAIR(5));
		mvprintw(height*2+4, 2, "DFS Found: %d", vis[height-1][width-1]);
		attroff(COLOR_PAIR(5));
		refresh();
	} else {
		attron(COLOR_PAIR(5));
		mvprintw(height*2+2, 2, "[..] : Points in queue");
		mvprintw(height*2+3, 2, "[oo] : Current Point");
		attroff(COLOR_PAIR(5));
		BFS(0, 0);
		attron(COLOR_PAIR(5));
		mvprintw(height*2+5, 2, "BFS Found: %d", vis[height-1][width-1]);
		attroff(COLOR_PAIR(5));
		refresh();
	}

	getchar();
	attron(COLOR_PAIR(5));
	mvprintw(3, 3, "Press any key to exit.");
	attroff(COLOR_PAIR(5));
	refresh();
	getchar();
	system("clear");
}
