// y = row
// x = col

#include "all.h"

int main() {
	srand(time(NULL));
	initscr();
	checkMakeColors();
	
	noecho();
	
	int timer = 0;
	int maxY;
	int maxX;
	double a;
	double map[HEIGHT * WIDTH]; //h,w
	
	//initMap(map);
	
	for (;;) {
		if (timer >= RUNLENGTH) {
			break;
		}
		getmaxyx(stdscr, maxY, maxX); //get max coords
		
		//if (timer % 100 == 0) {
			initMap(map);
			diamondSquare(map);
		//}
		
		//erase();
		drawMap(map, maxY, maxX);
		printThatThingInTheCenter(maxY, maxX, timer);
		refresh(); //spit to screen
		
		usleep(1000 * DELAY);
		timer++;
	}
	
	endwin(); //End curses mode
	return 0;
}

void initMap(double* map) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			double setTo = (fabs(y - HEIGHT/(float)2) / (HEIGHT/(float)2)) *
						   (fabs(x - WIDTH/(float)2) / (WIDTH/(float)2));
			
			setTo = -1.0;
			//fmin(fmax(setTo, 0), 1);
			//0.5;
			//randomDouble();
			//((double)y/HEIGHT) * ((double)x/WIDTH));
			set(map, y, x, setTo);
		}
	}
	
	set(map, 0, 0, randomDouble());
	set(map, 0, WIDTH-1, randomDouble());
	set(map, HEIGHT-1, 0, randomDouble());
	set(map, HEIGHT-1, WIDTH-1, randomDouble());
}

char getRepresenter(double cellValue) {
	char* hurhur = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"; //11 (would be 12 but \)
	int len = strlen(hurhur);
	int index = (int)(cellValue * len);
	index = fmin(fmax(index, 0), len - 1);
	return hurhur[index];
}

void diamondSquare(double* map) {
	diamondSquareRecurse(map, 0, 0, WIDTH, HEIGHT, 0.5);
}

void diamondSquareRecurse(double* map, int x, int y, int w, int h, double noiseScale) {
	if (w <= 2 || h <= 2) {
		return;
	}
	
	//do diamond square for this region
	int x2 = x + w-1; //rightmost cell
	int y2 = y + h-1; //bottom cell
	double topLeft = get(map, y, x);
	double topRght = get(map, y, x2);
	double botLeft = get(map, y2, x);
	double botRght = get(map, y2, x2);
	
	//diamond step
	double averageCorners = (topLeft + topRght + botLeft + botRght)/4.0;
	double newCenter = averageCorners - ((randomDouble() - 0.5) * noiseScale * 2); //getNoise(step);
	int centerX = x + w / 2;
	int centerY = y + h / 2;
	set(map, centerY, centerX, newCenter);
	
	//square step
	int midX = x + w/2;
	int midY = y + h/2;
	
	//top = avg(tl, tr) + noise
	double top = (topLeft + topRght)/2.0 + (randomDouble() - 0.5) * noiseScale;
	if (get(map, y, midX) == -1.0)
		set(map, y, midX, top);
	//bot = avg(bl, br) + noise
	double bot = (botLeft + botRght)/2.0 + (randomDouble() - 0.5) * noiseScale;
	if (get(map, y2, midX) == -1.0)
		set(map, y2, midX, bot);
	//lef = avg(tl, bl) + noise
	double lef = (topLeft + botLeft)/2.0 + (randomDouble() - 0.5) * noiseScale;
	if (get(map, midY, x) == -1.0)
		set(map, midY, x, lef);
	//rig = avg(tr, br) + noise
	double rig = (topRght + botRght)/2.0 + (randomDouble() - 0.5) * noiseScale;
	if (get(map, midY, x2) == -1.0)
		set(map, midY, x2, rig);
	
	//if can divide, do so for each quarter
	

	int newW = w/2+1;
	int newH = h/2+1;
	if (newW < 3 && newH < 3) {
		return;
	}
	newW = max(newW, 3);
	newH = max(newH, 3);
	
	double newNoiseScale = noiseScale * pow(2.0, -0.75);
	diamondSquareRecurse(map, x,       y,       newW, newH, newNoiseScale);
	diamondSquareRecurse(map, centerX, y,       newW, newH, newNoiseScale);
	diamondSquareRecurse(map, x,       centerY, newW, newH, newNoiseScale);
	diamondSquareRecurse(map, centerX, centerY, newW, newH, newNoiseScale);
}

double randomDouble() {
	return (double)rand()/(double)RAND_MAX;
}

int max(int a, int b) {
  return a > b ? a : b;
}

double get(double* map, int y, int x) {
	int index = y * WIDTH + x;
	return map[index];
}

void set(double* map, int y, int x, double set) {
	int index = y * WIDTH + x;
	map[index] = set;
}

void printThatThingInTheCenter(int maxY, int maxX, int timer) {
	char msg[] = "Screen WxH: %d by %d || Frames made: %d";
	mvprintw(HEIGHT, maxX/2 - 40, msg, maxY, maxX, timer);
}

void checkMakeColors() {
	if (has_colors() == FALSE) {
		endwin();
		printf("no support for colors on this terminal...\n");
		exit(1);
	}
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
}

void drawMap(double* map, int maxY, int maxX) {
	fprintf(stderr, "...");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			double here = get(map, y, x);
			setColorOn(here);
			char hereDisp = getRepresenter(here);
			mvaddch(y - (int)(maxY / 2.0f), (x - (int)(maxX / 2.0f))*2, hereDisp);
			mvaddch(y - (int)(maxY / 2.0f), (x - (int)(maxX / 2.0f))*2+1, hereDisp);
			//fprintf(stderr, "x=%d\ty=%d", x - (int)(maxX / 2.0f), y - (int)(maxY / 2.0f));
			setColorOff(here);
		}
	}
}

void setColorOn(double here) {
	if (here <= 0.2)
		attron(COLOR_PAIR(1)); //ocean blue 1
	else if (here <= 0.26)
		attron(COLOR_PAIR(2)); //tide cyan 2
	else if (here <= 0.375)
		attron(COLOR_PAIR(3)); //beach yellow 3
	else if (here <= 0.875)
		attron(COLOR_PAIR(4)); //grass green 4
	else
		attron(COLOR_PAIR(5)); //mountain white 5
}

void setColorOff(double here) {
	if (here <= 0.2)
		attroff(COLOR_PAIR(1)); //ocean blue 1
	else if (here <= 0.26)
		attroff(COLOR_PAIR(2)); //tide cyan 2
	else if (here <= 0.375)
		attroff(COLOR_PAIR(3)); //beach yellow 3
	else if (here <= 0.800)
		attroff(COLOR_PAIR(4)); //grass green 4
	else
		attroff(COLOR_PAIR(5)); //mountain white 5
}
