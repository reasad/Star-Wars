#include"iGraphics.h"
#include<math.h>
#include<stdio.h>
#include<string.h>

#define SCREENWIDTH  1300
#define SCREENHEIGHT  700
#define MOVEMENTSPEED 20
#define BACKGROUNDSPEED 1

void allObjectCollisionChecker();
void levelChecking(int x);
void valueRsetter();
bool IsColliding(int x1, int y1, int dx1, int dy1, int x2, int y2, int dx2, int dy2);

//first level upto 60 enemy ships ,second level  upto 130  enemy ships, level 3 upto 250 enenmyships
//final boss 

//level structure
bool level1 = true;
bool level2 = false;
bool level3 = false;
bool bosslevelintro = false;
int enemycounter = 60;


//Boss Boss
int random_x = 0;
int random_y = 0;
char bossimg[40] = "image\\enemy\\boss\\boss.bmp";
char bosslonglaserimg[40] = "image\\enemy\\boss\\bosslonglaser.bmp";
char redlaserimg[40] = "image\\enemy\\boss\\bossredlaser.bmp";
char yellowlaserimg[40] = "image\\enemy\\boss\\bossyellowlaser.bmp";
char bossmissleimg[40] = "image\\enemy\\boss\\missle.bmp";

struct boss{

	int x = (SCREENWIDTH - 700) / 2, y = 800;
	int dx = 522, dy = 128;

	int longlaser_x, longlaser_y;
	int longlaser_dx = 64, longlaser_dy = 598;
	bool longlasershot = false;

	int ylaser_x[8][8], ylaser_y[8][8];
	int ylaser_dx = 10, ylaser_dy = 34;
	int rlaser_x[8][8], rlaser_y[8][8];
	int rlaser_dx = 10, rlasr_dy = 34;

	int lasershotindex = 0;
	bool lasershot[8];

	int missle_x[8], missle_y[8];
	int missle_dx = 21, missle_dy = 34;
	bool missleshot = false;

	int lifevalue_x = 1015;
	int lifevalue_y = 15;
	bool life = false;
};
struct boss boss;

void bossRandom(){
	if (boss.lifevalue_x < 100){
		random_x = rand() % 600;
		random_y = rand() % 300;
	}
	else{
		random_x = rand() % 600;
		random_y = rand() % 400 + 200;
	}
}

void showBoss(){

	iShowBMP2(boss.x, boss.y, bossimg, 0);

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			//if (boss.lasershot[i] == true){
			iShowBMP2(boss.ylaser_x[i][j], boss.ylaser_y[i][j], yellowlaserimg, 0);
			iShowBMP2(boss.rlaser_x[i][j], boss.rlaser_y[i][j], redlaserimg, 0);
			//}
		}
	}

	for (int i = 0; i < 8; i++){
		if (boss.missleshot == true){
			iShowBMP2(boss.missle_x[i], boss.missle_y[i], bossmissleimg, 0);
		}

	}
	iSetColor(0, 255, 40);
	iText(0, 688, "BOSS LIFE", GLUT_BITMAP_9_BY_15);
	iSetColor(178, 0, 255);
	iFilledRectangle(85, 685, boss.lifevalue_x, boss.lifevalue_y);
}


//menu structure
struct menu{
	bool showmenu = true;
	bool avatar = false;

	bool avatar1select = false;
	int avatar1_x = 85, avatar1_y = 75;
	bool avatar2select = false;
	int avatar2_x = 525, avatar2_y = 75;
	bool avatar3select = false;
	int avatar3_x = 965, avatar3_y = 75;

	bool namefield = false; bool namefieldactive = false;
	int namefield_x = 477, namefield_y = 514;

	bool play = false; int play_x = 548, play_y = 400;

	bool option = false; int option_x = 517, option_y = 286;

	int turnon_x = 500, turnon_y = 390;
	int turnoff_x = 500, turnoff_y = 260;
	int back_x = 457, back_y = 107;

	bool gameover = false;
	int gameoverquit_x = 477, gameoverquit_y = 36;
	int gameoverback_x = 387, gameoverback_y = 116;

	bool top = false;
	int top_x = 457, top_y = 190;
	int top_dx = 361, top_dy = 45;
	int top_back_x = 540, top_back_y = 23;
	int top_back_dx = 162, top_back_dy = 46;

	bool level1completion = false;
	bool level2completion = false;
	bool level3completion = false;

	bool finalstagecompletion = false;
	int finalback_x = 369, finalback_y = 96;
	int finalback_dx = 520, finalback_dy = 50;
	int finalexit_x = 522, finalexit_y = 25;
	int finalexit_dx = 214, finalexit_dy = 25;

	bool credits = false;

	bool exit = false; int exit_x = 555, exit_y = 24;
};

struct menu gamemenu;
//Game board health bar
struct health{
	int health_x = SCREENWIDTH - 200 + 20;
	int health_y = 205;
	int health_xsize = 160;
	int health_ysize = 40;
	int health_red = 0;
	int health_green = 255;
	int health_blue = 0;
	int const healthsize = 160;

};
struct health healthboard;

struct booster{
	int x, y;
	int dx = 20, dy = 20;
	int boostvalue = 40;
	bool showbooster = false;
};

struct booster lifeboosterL1;

char lifeboosterimg[30] = "image\\healthbooster.bmp";
///Game bossters and special 

//Scoreboard counter and others
//TOP SCORES
//Hall of fame


struct topScorer{
	char name[30];
	int score;

};
struct topScorer topScorerInfo[10];


struct player{
	char playerName[30];
	int name_x = 1140, name_y = 530;
	int score = 0;
	int score_x = SCREENWIDTH - 200 + 57;
	int score_y = 587;
	char scorestring[10];

	int nameindex = 0;
	bool write = false;
};
struct player playerinfo;


//Number of players read from the file

int playerNum = 0;
void PlayerScoreboard(){

	int i, j, temp;
	char tempName[30];
	FILE *fp, *fp1, *fp2;

	//append playerinfo to file
	fp = fopen("scoreboard.txt", "a");
	fprintf(fp, "%s %d ", playerinfo.playerName, playerinfo.score);
	fclose(fp);

	//reads playerinfo from file and stores it
	fp1 = fopen("scoreboard.txt", "r");
	for (i = 0; !feof(fp1); i++){
		fscanf(fp1, "%s %d *c", &topScorerInfo[i].name, &topScorerInfo[i].score);
		playerNum++;
	}
	fclose(fp1);

	//sorts player score and name
	for (i = 0; i<playerNum - 1; i++){
		for (j = 0; j<playerNum - i - 1; j++) {
			if (topScorerInfo[j].score <topScorerInfo[j + 1].score) {
				temp = topScorerInfo[j].score;
				topScorerInfo[j].score = topScorerInfo[j + 1].score;
				topScorerInfo[j + 1].score = temp;

				strcpy(tempName, topScorerInfo[j].name);
				strcpy(topScorerInfo[j].name, topScorerInfo[j + 1].name);
				strcpy(topScorerInfo[j + 1].name, tempName);
			}
		}
	}

	fp2 = fopen("scoreboard.txt", "w");

	//writes the sorted number into file
	for (i = 0; i< 5; i++){
		fprintf(fp2, "%s %d ", topScorerInfo[i].name, topScorerInfo[i].score);
	}
	fclose(fp2);
}
void ShowTopPilots(){

	int x = 500, upper_y = 450;


	if (gamemenu.top == true){
		for (int i = 0; i < 5; i++){

			//Displays Player name
			iSetColor(0, 148, 255);
			iText(x, upper_y, topScorerInfo[i].name, GLUT_BITMAP_TIMES_ROMAN_24);

			char tempstring[30];
			_itoa_s(topScorerInfo[i].score, tempstring, 10);

			//Displays Player score
			iSetColor(76, 255, 0);
			iText(x + 200, upper_y, tempstring, GLUT_BITMAP_TIMES_ROMAN_24);
			upper_y -= 50;
		}
	}
}


//Hall of fame code ends here



//Game background r
char level1backimg[100] = "image\\background\\backlevel1.bmp";
char level2backimg[100] = "image\\background\\backlevel2.bmp";
char level3backimg[100] = "image\\background\\backlevel3.bmp";
char bossbackgimg[40] = "image\\background\\boss.bmp";
void ShowBackground(){
	if (level1 == true)
		iShowBMP(0, 0, level1backimg);
	else if (level2 == true)
		iShowBMP(0, 0, level2backimg);
	else if (level3 == true)
		iShowBMP(0, 0, level3backimg);
	else if (boss.life == true){
		iShowBMP(0, 0, bossbackgimg);
	}
}


//GAME MUSIC
bool gamesound = true;
char selecttrack[25] = "sound\\menu_select.wav";
char toppilottrack[30] = "sound\\toppilot.wav";

//Plays track on demands!!!
void Playtrack(bool isplay){
	if (gamesound == true){
		if (isplay == true && gamemenu.top == true)
			PlaySound(toppilottrack, NULL, SND_ASYNC);

		else if (isplay == true && gamemenu.showmenu == true)
			PlaySound("sound\\menu_track", NULL, SND_ASYNC);
	}
}


//All playership structures and variables starts from here
//playership file name
char playership[3][35] = { "image\\playership\\ship1.bmp", "image\\playership\\ship2.bmp",
"image\\playership\\ship3.bmp" };

//player ship structure
struct playership{
	int x = SCREENWIDTH / 2 - 150;
	int y = 0;
	int shiphealth;
	int dx = 80; int dy = 80;

};
//level 1 ship 1
struct playership playership1;
//laser structure for ship 1 level 1
struct laser
{
	bool shot = false;
	int l_x = playership1.x + 28; int dx = 7;
	int r_x = playership1.x + 43; int dy = 25;
	int  y = playership1.y + 60;
};
//Laser Variables
char bluelaserimg[31] = "image\\playership\\bluelaser.bmp";
struct laser bluelaser[100];
int laserindex = 0;


//All Enemeyship structure starts from here

char greenhornimg[30] = "image\\enemy\\greenhorn.bmp";
char greenlaserimg[30] = "image\\enemy\\greenlaser.bmp";
int green_random_x[5] = { 196, 396, 596, 796, 996 };
int green_random_y[5] = { 800, 1300, 2100, 1700, 2500 };
int greenhorn_rand_shotpos[10];
int const greenhornL1size = 6;
int const greenhornL2size = 4;
struct greenhorn{
	int x;
	int y;
	int dx = 96;
	int dy = 73;

	int laser_x[10];
	int laser_y[10];
	int laser_dx = 7;
	int laser_dy = 25;

	int laserdamage = 5;

	bool lasershot[10];
	int laser_index[10];
	int total_laser = 10;

	double lifevalue = 20;
	bool life = true;
};

int greehorncounter = 0;
struct greenhorn greenhornL1[greenhornL1size];
struct greenhorn greenhornL2[greenhornL2size];


char droneimg[30] = "image\\enemy\\drone.bmp";
char dronemissleimg[30] = "image\\enemy\\dronemissle.bmp";
int drone_random_x[5] = { -1500, -1200, -200, -500, -900 };
int drone_random_y[5] = { 70, 200, 600, 350, 480 };
int drone_rand_shotpos[10];
int const droneL1size = 2;
struct drone{
	double x;
	double y;
	int dx = 63;
	int dy = 59;
	double sinx = 0;

	int missle_x[10];
	int missle_y[10];
	int missle_dx = 21;
	int missle_dy = 34;
	int missledamage = 10;
	bool missleshot[10];
	int missle_index[10];
	int total_missle = 10;

	int lifevalue = 15;
	bool life = true;
};

struct drone droneL1[droneL1size];

//Greenspike enemy structure
char greenspikeimg[30] = "image\\enemy\\greenspike.bmp";
char spikeballimg[30] = "image\\enemy\\spikeball.bmp";
int greenspike_random_x[5] = { 250, 330, 650, 870, 1000 };
int greenspike_random_y[5] = { 900, 1300, 1700, 2100, 1500 };
int greenspikeL1_rand_shotpos[6];
int greenspikeL2_rand_shotpos[9];
int const greenspikeL1size = 5;
int const greenspikeL2size = 2;
int const greenspikeL3size = 2;
struct greenspike{
	int x;
	int y;
	int dx = 45;
	int dy = 46;

	int spikeball_x[10];
	int spikeball_y[10];
	int spikeball_dx = 12;
	int spikeball_dy = 12;
	int spikeballdamage = 7;
	bool spikeballshot[10];
	int spikeball_index[10];
	int total_spikeball = 10;

	int lifevalue = 10;
	bool life = true;
};
struct greenspike greenspikeL1[greenspikeL1size];
struct greenspike greenspikeL2[greenspikeL2size];



//GreenSatan structure
char greensatanimg[30] = "image\\enemy\\greensatan.bmp";
char satanbulletimg[30] = "image\\enemy\\satanbullet.bmp";
int greensatan_random_x[5] = { 10, 130, 210, 620, 950 };
int greensatan_random_y[5] = { 2200, 3000, 3500, 3200, 2500 };
int greensatanL2_rand_shotpos[10];
int const greensatanL2size = 3;
int const greensatanL3size = 3;
struct greensatan{

	int x;
	int y;
	int dx = 62;
	int dy = 55;


	int bullet_x[10];
	int bullet_y[10];
	int bullet_dx = 17;
	int bullet_dy = 21;
	int bulletdamage = 15;
	bool bulletshot[10];
	int bullet_index[10];
	int randomshotpos[10];
	int total_bullet = 10;

	int lifevalue = 10;
	bool life = true;
};

struct greensatan greensatanL2[greensatanL2size];

char snakegunimg[30] = "image\\enemy\\snakegun.bmp";
char snakebiteimg[30] = "image\\enemy\\snakebite.bmp";
int snakegun_random_x[5] = { -1500, -1200, -200, -500, -900 };
int snakegun_random_y[5] = { 70, 200, 600, 350, 480 };
int snakegun_rand_shotpos[40];
int const snakegunL2size = 3;
int const snakegunL3size = 4;
struct snakegun{
	double x;
	double y;
	int dx = 42;
	int dy = 78;
	double sinx = 0;
	double snakebite_siny = 0;

	int snakebite_x[10];
	int snakebite_y[10];
	int snakebite_dx = 12;
	int snakebite_dy = 13;
	int snakebitedamage = 3;
	bool snakebiteshot[10];
	int snakebite_index[10];
	int randomshotpos[10];
	int total_snakebite = 10;

	int lifevalue = 10;
	bool life = true;
};

struct snakegun snakegunL2[snakegunL2size];
struct snakegun snakegunL3[snakegunL3size];

//Whitedwarf

//GreenSatan structure
char whitedwarfimg[30] = "image\\enemy\\whitedwarf.bmp";
char redballimg[30] = "image\\enemy\\redball.bmp";
int whitedwarf_random_x[5] = { 10, 130, 210, 620, 950 };
int whitedwarf_random_y[5] = { 2200, 3000, 3500, 3200, 2500 };
int whitedwarf_rand_shotpos[10];
int const whitedwarfL3size = 3;
struct whitedwarf{

	int x;
	int y;
	int dx = 96;
	int dy = 96;


	int redball_x[10];
	int redball_y[10];
	int redball_dx = 32;
	int redball_dy = 29;
	int redballdamage = 15;
	bool redballshot[10];
	int redball_index[10];
	int randomshotpos[10];
	int total_redball = 10;

	int lifevalue = 25;
	bool life = true;
};

struct whitedwarf whitedwarfL3[whitedwarfL3size];
//First Time randomly initializes enemey ships positions
void level1AllObjectsInit(){


	//booster initialization
	lifeboosterL1.x = rand() % 1000;
	lifeboosterL1.y = rand() % 3000;

	//Initializes Level 1 Greenhorn
	for (int i = 0; i < 10; i++){
		greenhorn_rand_shotpos[i] = (rand() % 700);
	}

	for (int i = 0; i < greenhornL1size; i++){
		greenhornL1[i].x = green_random_x[rand() % 5];
		greenhornL1[i].y = green_random_y[rand() % 5];
		for (int j = 0; j < greenhornL1[i].total_laser; j++){
			greenhornL1[i].laser_x[j] = greenhornL1[i].x + 18;
			greenhornL1[i].laser_y[j] = greenhornL1[i].y + 1;
			greenhornL1[i].laser_index[j] = 0;
			greenhornL1[i].lasershot[j] = false;
		}
	}

	//Initializes Level 1 Drones
	for (int i = 0; i < 10; i++){
		drone_rand_shotpos[i] = (rand() % 1100);
	}
	for (int i = 0; i < droneL1size; i++){
		droneL1[i].x = drone_random_x[rand() % 5];
		droneL1[i].y = drone_random_y[rand() % 5];
		for (int j = 0; j < droneL1[i].total_missle; j++){
			droneL1[i].missle_x[j] = droneL1[i].x + 31;
			droneL1[i].missle_y[j] = droneL1[i].y + 9;
			droneL1[i].missle_index[j] = 0;
			droneL1[i].missleshot[j] = false;
		}

	}

	//Initializes Level 1 greenspike
	for (int i = 0; i < 6; i++){
		greenspikeL1_rand_shotpos[i] = (rand() % 700);
	}
	for (int i = 0; i < greenspikeL1size; i++){
		greenspikeL1[i].x = greenspike_random_x[rand() % 5];
		greenspikeL1[i].y = greenspike_random_y[rand() % 5];
		for (int j = 0; j < greenspikeL1[i].total_spikeball; j++){
			greenspikeL1[i].spikeball_x[j] = greenspikeL1[i].x + 21;
			greenspikeL1[i].spikeball_y[j] = greenspikeL1[i].y + 2;
			greenspikeL1[i].spikeball_index[j] = 0;
			greenspikeL1[i].spikeballshot[j] = false;
		}
	}
}

//Level 2  Enemy Initializer 
void level2_level3AllObjectsInit(){

	//show booster

	if (lifeboosterL1.showbooster == true){
		iShowBMP2(lifeboosterL1.x, lifeboosterL1.y, lifeboosterimg, 0);
	}


	//Green satan init
	for (int i = 0; i < 10; i++){
		greensatanL2_rand_shotpos[i] = (rand() % 700);
	}

	for (int i = 0; i < greensatanL2size; i++){
		greensatanL2[i].x = greensatan_random_x[rand() % 5];
		greensatanL2[i].y = greensatan_random_y[rand() % 5];
		for (int j = 0; j < greensatanL2[i].total_bullet; j++){
			greensatanL2[i].bullet_x[j] = greensatanL2[i].x + 33;
			greensatanL2[i].bullet_y[j] = greensatanL2[i].y + 6;
			greensatanL2[i].bullet_index[j] = 0;
			greensatanL2[i].bulletshot[j] = false;
		}
	}

	//Initializes Level 2 Greenhorn
	for (int i = 0; i < 10; i++){
		greenhorn_rand_shotpos[i] = (rand() % 600);
	}

	for (int i = 0; i < greenhornL2size; i++){
		greenhornL2[i].x = green_random_x[rand() % 5];
		greenhornL2[i].y = green_random_y[rand() % 5];
		for (int j = 0; j < greenhornL2[i].total_laser; j++){
			greenhornL2[i].laser_x[j] = greenhornL2[i].x + 18;
			greenhornL2[i].laser_y[j] = greenhornL2[i].y + 1;
			greenhornL2[i].laser_index[j] = 0;
			greenhornL2[i].lasershot[j] = false;
		}
	}

	//Greenspike level 2 initialization

	for (int i = 0; i < 10; i++){
		greensatanL2_rand_shotpos[i] = (rand() % 650);
	}

	for (int i = 0; i < greensatanL2size; i++){
		greensatanL2[i].x = greensatan_random_x[rand() % 5];
		greensatanL2[i].y = greensatan_random_y[rand() % 5];
		for (int j = 0; j < greensatanL2[i].total_bullet; j++){
			greensatanL2[i].bullet_x[j] = greensatanL2[i].x + 18;
			greensatanL2[i].bullet_y[j] = greensatanL2[i].y + 1;
			greensatanL2[i].bullet_index[j] = 0;
			greensatanL2[i].bulletshot[j] = false;
		}
	}

	//SnakeGun Initialization

	for (int i = 0; i < 40; i++){
		snakegun_rand_shotpos[i] = (rand() % 1100);
	}


	for (int i = 0; i < snakegunL2size; i++){

		snakegunL2[i].x = snakegun_random_x[rand() % 5];
		snakegunL2[i].y = snakegun_random_y[rand() % 5];

		for (int j = 0; j < snakegunL2[i].total_snakebite; j++){

			snakegunL2[i].snakebite_x[j] = snakegunL2[i].x + 11;
			snakegunL2[i].snakebite_y[j] = snakegunL2[i].y + 3;
			snakegunL2[i].snakebite_index[j] = 0;
			snakegunL2[i].snakebiteshot[j] = false;
		}

	}


	//whitedwarf init

	//Green satan init
	for (int i = 0; i < 10; i++){
		whitedwarf_rand_shotpos[i] = (rand() % 700);
	}

	for (int i = 0; i < whitedwarfL3size; i++){
		whitedwarfL3[i].x = whitedwarf_random_x[rand() % 5];
		whitedwarfL3[i].y = whitedwarf_random_y[rand() % 5];
		for (int j = 0; j < whitedwarfL3[i].total_redball; j++){
			whitedwarfL3[i].redball_x[j] = whitedwarfL3[i].x + 40;
			whitedwarfL3[i].redball_y[j] = whitedwarfL3[i].y + 19;
			whitedwarfL3[i].redball_index[j] = 0;
			whitedwarfL3[i].redballshot[j] = false;
		}
	}

	//BOss Init

	for (int i = 0; i < 5; i++){
		boss.lasershot[i] = false;
	}
}

//All enemy drawing function starts here
//Displays Greenhorn enemyship // Called by Idraw functions
void ShowLevel1ALLObjects(){

	//booster
	if (lifeboosterL1.showbooster == true){
		iShowBMP2(lifeboosterL1.x, lifeboosterL1.y, lifeboosterimg, 0);
	}

	//Shows Level 1 Greenhorn on the screen
	for (int i = 0; i < greenhornL1size; i++){
		if (greenhornL1[i].life == true){
			iShowBMP2(greenhornL1[i].x, greenhornL1[i].y, greenhornimg, 0);
			for (int j = 0; j < 10; j++){
				if (greenhornL1[i].lasershot[j] == true){
					iShowBMP2(greenhornL1[i].laser_x[j], greenhornL1[i].laser_y[j], greenlaserimg, 0);
				}
			}
		}
	}

	//Shows level1 drones
	for (int i = 0; i < droneL1size; i++){
		if (droneL1[i].life == true){
			iShowBMP2(droneL1[i].x, droneL1[i].y, droneimg, 0);
		}
		for (int j = 0; j < droneL1[i].total_missle; j++){
			if (droneL1[i].missleshot[j] == true){
				iShowBMP2(droneL1[i].missle_x[j], droneL1[i].missle_y[j], dronemissleimg, 0);
			}
		}
	}

	//Shows level1 greenspikes
	for (int i = 0; i < greenspikeL1size; i++){
		if (greenspikeL1[i].life == true){
			iShowBMP2(greenspikeL1[i].x, greenspikeL1[i].y, greenspikeimg, 0);
		}
		for (int j = 0; j < greenspikeL1[i].total_spikeball; j++){
			if (greenspikeL1[i].spikeballshot[j] == true){
				iShowBMP2(greenspikeL1[i].spikeball_x[j], greenspikeL1[i].spikeball_y[j], spikeballimg, 0);
			}
		}
	}
}

//Level 2 all objects
void showLevel2_Level3Objects(){

	//Show booster
	if (lifeboosterL1.showbooster == true){
		iShowBMP2(lifeboosterL1.x, lifeboosterL1.y, lifeboosterimg, 0);
	}

	//Shows Level 1 Greenhorn on the screen
	for (int i = 0; i < greenhornL2size; i++){
		if (greenhornL2[i].life == true){
			iShowBMP2(greenhornL2[i].x, greenhornL2[i].y, greenhornimg, 0);
			for (int j = 0; j < 10; j++){
				if (greenhornL2[i].lasershot[j] == true){
					iShowBMP2(greenhornL2[i].laser_x[j], greenhornL2[i].laser_y[j], greenlaserimg, 0);
				}
			}
		}
	}

	//Shows leveL2 greenspikes
	for (int i = 0; i < greenspikeL2size; i++){
		if (greenspikeL2[i].life == true){
			iShowBMP2(greenspikeL2[i].x, greenspikeL2[i].y, greenspikeimg, 0);
		}
		for (int j = 0; j < greenspikeL2[i].total_spikeball; j++){
			if (greenspikeL2[i].spikeballshot[j] == true){
				iShowBMP2(greenspikeL2[i].spikeball_x[j], greenspikeL2[i].spikeball_y[j], spikeballimg, 0);
			}
		}
	}

	//Shows Greensatan
	for (int i = 0; i < greensatanL2size; i++){
		if (greensatanL2[i].life == true){
			iShowBMP2(greensatanL2[i].x, greensatanL2[i].y, greensatanimg, 0);
		}
		for (int j = 0; j < greensatanL2[i].total_bullet; j++){
			if (greensatanL2[i].bulletshot[j] == true){
				iShowBMP2(greensatanL2[i].bullet_x[j], greensatanL2[i].bullet_y[j], satanbulletimg, 0);
			}
		}
	}


	//Displays snakebite

	for (int i = 0; i < snakegunL2size; i++){
		if (snakegunL2[i].life == true){
			iShowBMP2(snakegunL2[i].x, snakegunL2[i].y, snakegunimg, 0);
		}
		for (int j = 0; j < snakegunL2[i].total_snakebite; j++){
			if (snakegunL2[i].snakebiteshot[j] == true){
				iShowBMP2(snakegunL2[i].snakebite_x[j], snakegunL2[i].snakebite_y[j], snakebiteimg, 0);
			}
		}
	}

	////Shows Greensatan
	if (level3 == true){
		for (int i = 0; i < whitedwarfL3size; i++){
			if (whitedwarfL3[i].life == true){
				iShowBMP2(whitedwarfL3[i].x, whitedwarfL3[i].y, whitedwarfimg, 0);
			}
			for (int j = 0; j < whitedwarfL3[i].total_redball; j++){
				if (whitedwarfL3[i].redballshot[j] == true){
					iShowBMP2(whitedwarfL3[i].redball_x[j], whitedwarfL3[i].redball_y[j], redballimg, 0);
				}
			}
		}
	}
}

//All functions for drawing starts from here

//ship draw func
void ShowScore(){
	_itoa_s(playerinfo.score, playerinfo.scorestring, 10);
	iSetColor(0, 255, 255);
	iText2(playerinfo.score_x, playerinfo.score_y, playerinfo.scorestring, GLUT_BITMAP_TIMES_ROMAN_24);
	iText2(playerinfo.name_x, playerinfo.name_y, playerinfo.playerName, GLUT_BITMAP_TIMES_ROMAN_24);
}

//Draws a health rectangle
void ShowHealth(){

	if (healthboard.health_xsize > 0){
		if (healthboard.health_xsize >= healthboard.healthsize / 2)
		{
			healthboard.health_blue = 0; healthboard.health_green = 255, healthboard.health_red = 0;
		}
		else if (healthboard.health_xsize < healthboard.healthsize / 2 &&
			healthboard.health_xsize >= healthboard.healthsize / 4){
			healthboard.health_blue = 17; healthboard.health_green = 255, healthboard.health_red = 251;
		}
		else{
			healthboard.health_blue = 0; healthboard.health_green = 0, healthboard.health_red = 255;
		}

		iSetColor(healthboard.health_red, healthboard.health_green, healthboard.health_blue);
		iFilledRectangle(healthboard.health_x, healthboard.health_y, healthboard.health_xsize,
			healthboard.health_ysize);
	}
}

//displays playership 1
void ShowPlayerShip1(){

	iShowBMP2(playership1.x, playership1.y, playership[0], 0);
	for (int i = 0; i < 100; i++){
		if (bluelaser[i].shot == true){
			iShowBMP2(bluelaser[i].l_x, bluelaser[i].y, bluelaserimg, 0);
			iShowBMP2(bluelaser[i].r_x, bluelaser[i].y, bluelaserimg, 0);
		}
	}
	for (int i = 0; i<100; i++){
		if (bluelaser[i].y> SCREENHEIGHT){
			bluelaser[i].shot = false;
			bluelaser[i].l_x = playership1.x + 28, bluelaser[i].r_x = playership1.x + 43,
				bluelaser[i].y = playership1.y + 60;
		}
	}
}

// Playership 1 , level 1 ,Blue laser position initializer function
void Ship1LaserXInecrease(){
	for (int i = 0; i < 100; i++){
		if (bluelaser[i].r_x < 1050 && bluelaser[i].shot == false){
			bluelaser[i].l_x += MOVEMENTSPEED;
			bluelaser[i].r_x += MOVEMENTSPEED;
		}
	}
}

void Ship1LaserXdecrease(){
	for (size_t i = 0; i < 100; i++)
	{
		if (bluelaser[i].l_x >28 && bluelaser[i].shot == false){
			bluelaser[i].l_x -= MOVEMENTSPEED;
			bluelaser[i].r_x -= MOVEMENTSPEED;
		}
	}
}

void Ship1LaserYincrease(){
	for (size_t i = 0; i <100; i++)
	{
		bluelaser[i].y += MOVEMENTSPEED;
	}

}
void Ship1LaserYdecrease(){
	for (size_t i = 0; i < 100; i++)
	{
		if (bluelaser[i].y>80 && bluelaser[i].shot == false)
			bluelaser[i].y -= MOVEMENTSPEED;
	}

}
// palyer ship 1 level lasaer position initializer ends here

void iDraw()
{
	iClear();

	if (gamemenu.play == true)
		allObjectCollisionChecker();


	if (enemycounter == 60){
		enemycounter++;
		levelChecking(2);
	}
	else if (enemycounter == 130){
		enemycounter++;
		levelChecking(3);
	}
	else if (enemycounter == 250){
		enemycounter++;
		levelChecking(4);
	}

	if (gamemenu.showmenu == true){
		iShowBMP(0, 0, "image\\gamemenu\\completemenu.bmp");
	}

	if (gamemenu.avatar == true){
		iShowBMP(0, 0, "image\\gamemenu\\charactermenu.bmp");
	}
	if (gamemenu.top == true){
		iShowBMP(0, 0, "image\\gamemenu\\toppilots.bmp");
	}
	if (gamemenu.option == true){
		iShowBMP(0, 0, "image\\gamemenu\\option.bmp");
	}

	if (gamemenu.namefield == true){
		iShowBMP(0, 0, "image\\gamemenu\\namefield.bmp");
		iSetColor(0, 197, 255);
		iText(543, 530, playerinfo.playerName, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (gamemenu.play == true){
		ShowBackground();
	}
	if (gamemenu.play == true && gamemenu.avatar1select == true){
		iShowBMP(SCREENWIDTH - 200, 0, "image\\gamemenu\\avatar1select.bmp");
		ShowScore();
		ShowHealth();
	}
	else if (gamemenu.play == true && gamemenu.avatar2select == true){

		iShowBMP(SCREENWIDTH - 200, 0, "image\\gamemenu\\avatar2select.bmp");
		ShowScore();
		ShowHealth();
	}
	else if (gamemenu.play == true && gamemenu.avatar3select == true){
		iShowBMP(SCREENWIDTH - 200, 0, "image\\gamemenu\\avatar3select.bmp");
		ShowScore();
		ShowHealth();
	}

	else if (gamemenu.top == true){
		ShowTopPilots();
	}

	//player spaceships // ship 1 level 1 // and ammos
	if (gamemenu.play == true){
		ShowPlayerShip1();
	}

	if (gamemenu.play == true && level1 == true){
		ShowLevel1ALLObjects();
	}

	//Here starts level 2

	if (gamemenu.play == true && level1 == false && gamemenu.level1completion == true){
		iShowBMP(0, 0, "image\\gamemenu\\level1completion.bmp");
	}
	else if (gamemenu.play == true && level2 == false && gamemenu.level2completion == true){
		iShowBMP(0, 0, "image\\gamemenu\\level2completion.bmp");
	}
	else if (gamemenu.play == true && level3 == false && bosslevelintro == true){
		iShowBMP(0, 0, "image\\gamemenu\\bosslevelintro.bmp");
	}
	else if (gamemenu.play == false && gamemenu.finalstagecompletion == true){
		iShowBMP(0, 0, "image\\gamemenu\\completion.bmp");
	}

	if (gamemenu.play == true && (level2 == true || level3 == true)){
		showLevel2_Level3Objects();
	}

	if (gamemenu.play == true && boss.life == true){
		showBoss();
	}

	if (gamemenu.gameover == true){
		iShowBMP(0, 0, "image\\gamemenu\\gameover3.bmp");

	}
}

void iMouseMove(int mx, int my)
{



}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){


		if (gamemenu.showmenu == true && ((mx >= gamemenu.play_x && mx <= gamemenu.play_x + 170) &&
			(my >= gamemenu.play_y && my <= gamemenu.play_y + 46))){
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamemenu.showmenu = false;
			gamemenu.avatar = true;
		}

		else if (gamemenu.showmenu == true && ((mx >= gamemenu.option_x && mx <= gamemenu.option_x + 240)
			&& (my >= gamemenu.option_y && my <= gamemenu.option_y + 52))){
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamemenu.showmenu = false;
			gamemenu.option = true;
		}

		else if (gamemenu.option == true && (mx >= gamemenu.turnon_x && mx <= gamemenu.turnon_x + 240)
			&& (my >= gamemenu.turnon_y && my <= gamemenu.turnon_y + 52)){
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamesound = true;
		}

		else  if (gamemenu.option == true && (mx >= gamemenu.turnoff_x && mx <= gamemenu.turnoff_x + 240)
			&& (my >= gamemenu.turnoff_y && my <= gamemenu.turnoff_y + 52)){
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamesound = false;
		}

		else  if (gamemenu.option == true && (mx >= gamemenu.back_x && mx <= gamemenu.back_x + 334)
			&& (my >= gamemenu.back_y && my <= gamemenu.back_y + 66)){
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamemenu.option = false;
			gamemenu.showmenu = true;
			Playtrack(true);
		}

		else if (gamemenu.showmenu == true && ((mx >= gamemenu.exit_x && mx <= gamemenu.exit_x + 165)
			&& (my >= gamemenu.exit_y && my <= gamemenu.exit_y + 73))){

			exit(0);
		}

		else if (gamemenu.avatar == true && (mx >= gamemenu.avatar1_x && mx <= gamemenu.avatar1_x + 265)
			&& (my >= gamemenu.avatar1_y && my <= gamemenu.avatar1_y + 555)){
			if (gamesound == true)
				PlaySound("sound\\menu_select.wav", NULL, SND_ASYNC);

			gamemenu.avatar = false;
			gamemenu.namefield = true;
			gamemenu.avatar1select = true;
		}

		else if
			(gamemenu.avatar == true && (mx >= gamemenu.avatar2_x && mx <= gamemenu.avatar2_x + 265)
			&& (my >= gamemenu.avatar2_y && my <= gamemenu.avatar2_y + 555)){

			if (gamesound == true)
				PlaySound("sound\\menu_select.wav", NULL, SND_ASYNC);

			gamemenu.avatar = false;
			gamemenu.namefield = true;
			gamemenu.avatar2select = true;
		}

		else  if
			(gamemenu.avatar == true && (mx >= gamemenu.avatar3_x && mx <= gamemenu.avatar3_x + 265)
			&& (my >= gamemenu.avatar3_y && my <= gamemenu.avatar3_y + 555)){
			if (gamesound == true){
				PlaySound("sound\\menu_select.wav", NULL, SND_ASYNC);
			}
			gamemenu.avatar = false;
			gamemenu.namefield = true;
			gamemenu.avatar3select = true;
		}

		else if (gamemenu.namefield == true && ((mx >= gamemenu.namefield_x && mx <= gamemenu.namefield_x + 263))
			&& ((my >= gamemenu.namefield_y && my <= gamemenu.namefield_y + 52))){
			gamemenu.namefieldactive = true;

		}

		else if (gamemenu.gameover == true && ((mx >= gamemenu.gameoverquit_x && mx <= gamemenu.gameoverquit_x + 275))
			&& ((my >= gamemenu.gameoverquit_y  && my <= gamemenu.gameoverquit_y + 32))){
			exit(0);
		}

		else if (gamemenu.gameover == true && ((mx >= gamemenu.gameoverback_x && mx <= gamemenu.gameoverback_x + 491))
			&& ((my >= gamemenu.gameoverback_y  && my <= gamemenu.gameoverback_y + 47))){

			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);

			gamemenu.gameover = false;
			gamemenu.showmenu = true;
			Playtrack(true);
			valueRsetter();
			PlayerScoreboard();
		}

		else if (gamemenu.showmenu == true &&
			((mx >= gamemenu.top_x && mx <= gamemenu.top_x + gamemenu.top_dx) &&
			(my >= gamemenu.top_y && my <= gamemenu.top_y + gamemenu.top_dy))){

			gamemenu.showmenu = false;
			gamemenu.top = true;

			if (gamesound == true){
				PlaySound(selecttrack, NULL, SND_ASYNC);
				Playtrack(true);

			}


		}
		else if (gamemenu.top == true &&
			((mx >= gamemenu.top_back_x && mx <= gamemenu.top_back_x + gamemenu.top_back_dx) &&
			(my >= gamemenu.top_back_y && my <= gamemenu.top_back_y + gamemenu.top_back_dy))){

			gamemenu.showmenu = true;
			gamemenu.top = false;

			if (gamesound == true){
				PlaySound(selecttrack, NULL, SND_ASYNC);
				Playtrack(true);

			}


		}

		else if (gamemenu.finalstagecompletion == true &&
			((mx >= gamemenu.finalback_x && mx <= gamemenu.top_back_x + gamemenu.finalback_dx) &&
			(my >= gamemenu.finalback_y && my <= gamemenu.finalback_y + gamemenu.finalback_dy))){
			gamemenu.finalstagecompletion = false;
			gamemenu.showmenu = true;
			valueRsetter();
			if (gamesound == true)
				PlaySound(selecttrack, NULL, SND_ASYNC);
			Playtrack(true);
		}
		else if (gamemenu.finalstagecompletion == true &&
			((mx >= gamemenu.finalexit_x && mx <= gamemenu.finalexit_x + gamemenu.finalexit_dx) &&
			(my >= gamemenu.finalexit_y && my <= gamemenu.finalexit_y + gamemenu.finalexit_dy))){
			exit(0);
		}
	}

}


void iKeyboard(unsigned char key)
{

	if (gamemenu.play == true && key == ' '){
		if (laserindex == 40)
			laserindex = 0;
		if (gamesound == true)
			PlaySound("sound\\lasershot.wav", NULL, SND_ASYNC);
		bluelaser[laserindex++].shot = true;
	}
	else if (gamemenu.namefieldactive == true && playerinfo.nameindex <= 9){
		if (key != '\b'){
			playerinfo.playerName[playerinfo.nameindex] = key;
			playerinfo.playerName[playerinfo.nameindex + 1] = '\0';
			playerinfo.nameindex++;
		}
		else{
			playerinfo.playerName[playerinfo.nameindex] = '\0';
			playerinfo.nameindex--;
			if (playerinfo.nameindex < 0)
				playerinfo.nameindex = 0;
		}
	}
	if (gamemenu.namefieldactive == true && key == '\r'){
		if (gamesound == true)
			PlaySound(selecttrack, NULL, SND_ASYNC);
		gamemenu.namefieldactive = false; gamemenu.namefield = false;
		gamemenu.play = true;

	}

	if (gamemenu.play == true && gamemenu.level1completion == true && key == 'c'){
		gamemenu.level1completion = false;
		level2 = true;
	}
	else if (gamemenu.play == true && gamemenu.level2completion == true && key == 'c'){
		gamemenu.level2completion = false;
		level3 = true;
	}
	else if (gamemenu.play == true && bosslevelintro == true && key == 'c'){
		level3 = false;
		bosslevelintro = false;
		boss.life = true;
	}

	if (gamemenu.play == true && (level1 == true || level2 == true || level3 == true) && key == 'p'){
		iPauseTimer(0);
	}
	if (gamemenu.play == true && (level1 == true || level2 == true || level3 == true) && key == 'r'){
		iResumeTimer(0);
	}
}


void iSpecialKeyboard(unsigned char key)
{
	//player spaceship position
	if (gamemenu.play == true){
		if (key == GLUT_KEY_UP)
		{
			if (playership1.y<SCREENHEIGHT - 80)
				playership1.y += MOVEMENTSPEED;
		}
		if (key == GLUT_KEY_DOWN){
			if (playership1.y > 0)
				playership1.y -= MOVEMENTSPEED;
		}
		if (key == GLUT_KEY_RIGHT){
			if (playership1.x < SCREENWIDTH - 290){
				playership1.x += MOVEMENTSPEED;
			}
		}
		if (key == GLUT_KEY_LEFT)
		{
			if (playership1.x> 10)
				playership1.x -= MOVEMENTSPEED;
		}


		//laser shot initial position decider
		if (key == GLUT_KEY_RIGHT){

			Ship1LaserXInecrease();
		}
		if (key == GLUT_KEY_LEFT){
			Ship1LaserXdecrease();
		}
		if (key == GLUT_KEY_UP){
			Ship1LaserYincrease();
		}
		if (key == GLUT_KEY_DOWN){
			Ship1LaserYdecrease();
		}
	}
}

//Game object changing functions
void allObjectChange(){


	//all palyership object position changing codes starts here

	for (size_t i = 0; i < 100; i++)
	{
		if (bluelaser[i].shot == true)
			bluelaser[i].y += MOVEMENTSPEED;
	}

	//all palyership object position changing codes sta here


	///health booster change

	if (gamemenu.play == true && (level1 == true || level2 == true || level3 == true)){

		if (enemycounter == 20){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 40){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 60){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 90){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 110){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 135){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 150){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 180){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 200){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 210){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 220){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 230){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}
		else if (enemycounter == 240){
			lifeboosterL1.showbooster = true;
			enemycounter++;
		}

		if (lifeboosterL1.y < 0){
			lifeboosterL1.showbooster = false;
			lifeboosterL1.x = rand() % 1000;
			lifeboosterL1.y = rand() % 2000;
		}

		if (lifeboosterL1.showbooster == true){
			lifeboosterL1.y -= 2;
		}


	}


	//All enenmy object position changing codes starts here

	//GreenHorn change functions starts here

	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < greenhornL1size; i++){
			if (greenhornL1[i].y <= 0 || greenhornL1[i].life == false){
				greenhornL1[i].y = green_random_y[rand() % 5];
				greenhornL1[i].x = green_random_x[rand() % 5];
				greenhornL1[i].lifevalue = 20;
				greenhornL1[i].life = true;
			}
			for (int j = 0; j < greenhornL1[i].total_laser; j++){

				if (greenhornL1[i].laser_y[j] <= 0 || greenhornL1[i].lasershot[j] == false){
					greenhornL1[i].laser_x[j] = greenhornL1[i].x + 48;
					greenhornL1[i].laser_y[j] = greenhornL1[i].y + 7;
					greenhornL1[i].lasershot[j] = false;
				}
				if (greenhornL1[i].y == greenhorn_rand_shotpos[j]){
					if (greenhornL1[i].laser_index[j] == 10){
						greenhornL1[i].laser_index[j] = 0;
					}
					greenhornL1[i].lasershot[greenhornL1[i].laser_index[j]++] = true;
				}
				greenhornL1[i].laser_y[j] -= 13;
			}
			greenhornL1[i].y--;
		}
	}

	//Level 3 and 2 change

	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greenhornL2size; i++){
			if (greenhornL2[i].y <= 0 || greenhornL2[i].life == false){
				greenhornL2[i].y = green_random_y[rand() % 5];
				greenhornL2[i].x = green_random_x[rand() % 5];
				greenhornL2[i].lifevalue = 20;
				greenhornL2[i].life = true;
			}
			for (int j = 0; j < greenhornL2[i].total_laser; j++){

				if (greenhornL2[i].laser_y[j] <= 0 || greenhornL2[i].lasershot[j] == false){
					greenhornL2[i].laser_x[j] = greenhornL2[i].x + 48;
					greenhornL2[i].laser_y[j] = greenhornL2[i].y + 7;
					greenhornL2[i].lasershot[j] = false;
				}
				if (greenhornL2[i].y == greenhorn_rand_shotpos[j]){
					if (greenhornL2[i].laser_index[j] == 10){
						greenhornL2[i].laser_index[j] = 0;
					}
					greenhornL2[i].lasershot[greenhornL2[i].laser_index[j]++] = true;
				}
				greenhornL2[i].laser_y[j] -= 13;
			}
			greenhornL2[i].y--;
		}
	}
	//Greenhorn Change function Ends here

	//Drone Change function Starts here
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < droneL1size; i++){
			if (droneL1[i].life == false || droneL1[i].x >= SCREENWIDTH - 263){
				droneL1[i].x = drone_random_x[rand() % 5];
				droneL1[i].y = drone_random_y[rand() % 5];
				droneL1[i].life = true;
				droneL1[i].lifevalue = 15;
			}
			for (int j = 0; j < droneL1[i].total_missle; j++){

				if (droneL1[i].missle_y[j] <= 0 || droneL1[i].missleshot[j] == false){
					droneL1[i].missle_x[j] = droneL1[i].x + 31;
					droneL1[i].missle_y[j] = droneL1[i].y + 9;
					droneL1[i].missleshot[j] = false;
				}
				if (droneL1[i].x == drone_rand_shotpos[j]){
					if (droneL1[i].missle_index[j] == 10){
						droneL1[i].missle_index[j] = 0;
					}
					droneL1[i].missleshot[droneL1[i].missle_index[j]++] = true;
				}
				droneL1[i].missle_y[j] -= 10;

			}
			droneL1[i].y = 80 * sin(droneL1[i].sinx) + 400;
			droneL1[i].sinx += 0.05;
			droneL1[i].x += 3;
		}
	}

	//Drone Change function ends here

	//Green Spike Change functions

	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < greenspikeL1size; i++){
			if (greenspikeL1[i].y <= 0 || greenspikeL1[i].life == false){
				greenspikeL1[i].y = greenspike_random_y[rand() % 5];
				greenspikeL1[i].x = greenspike_random_x[rand() % 5];
				greenspikeL1[i].lifevalue = 10;
				greenspikeL1[i].life = true;
			}
			for (int j = 0; j < greenspikeL1[i].total_spikeball; j++){

				if (greenspikeL1[i].spikeball_y[j] <= 0 || greenspikeL1[i].spikeballshot[j] == false){
					greenspikeL1[i].spikeball_x[j] = greenspikeL1[i].x + 18;
					greenspikeL1[i].spikeball_y[j] = greenspikeL1[i].y + 1;
					greenspikeL1[i].spikeballshot[j] = false;
				}
				if (greenspikeL1[i].y == greenhorn_rand_shotpos[j]){
					if (greenspikeL1[i].spikeball_index[j] == 10){
						greenspikeL1[i].spikeball_index[j] = 0;
					}
					greenspikeL1[i].spikeballshot[greenspikeL1[i].spikeball_index[j]++] = true;
				}
				greenspikeL1[i].spikeball_y[j] -= 15;
			}
			greenspikeL1[i].y -= 3;
		}
	}

	//Level 3
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greenspikeL2size; i++){
			if (greenspikeL2[i].y <= 0 || greenspikeL2[i].life == false){
				greenspikeL2[i].y = greenspike_random_y[rand() % 5];
				greenspikeL2[i].x = greenspike_random_x[rand() % 5];
				greenspikeL2[i].lifevalue = 10;
				greenspikeL2[i].life = true;
			}
			for (int j = 0; j < greenspikeL2[i].total_spikeball; j++){

				if (greenspikeL2[i].spikeball_y[j] <= 0 || greenspikeL2[i].spikeballshot[j] == false){
					greenspikeL2[i].spikeball_x[j] = greenspikeL2[i].x + 18;
					greenspikeL2[i].spikeball_y[j] = greenspikeL2[i].y + 1;
					greenspikeL2[i].spikeballshot[j] = false;
				}
				if (greenspikeL2[i].y == greenhorn_rand_shotpos[j]){
					if (greenspikeL2[i].spikeball_index[j] == 10){
						greenspikeL2[i].spikeball_index[j] = 0;
					}
					greenspikeL2[i].spikeballshot[greenspikeL2[i].spikeball_index[j]++] = true;
				}
				greenspikeL2[i].spikeball_y[j] -= 15;
			}
			greenspikeL2[i].y -= 3;
		}
	}

	//Greenspike change ends here

	//Green satan change starts here
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greensatanL2size; i++){
			if (greensatanL2[i].y <= 0 || greensatanL2[i].life == false){
				greensatanL2[i].y = greensatan_random_y[rand() % 5];
				greensatanL2[i].x = greensatan_random_x[rand() % 5];
				greensatanL2[i].lifevalue = 10;
				greensatanL2[i].life = true;
			}
			for (int j = 0; j < greensatanL2[i].total_bullet; j++){

				if (greensatanL2[i].bullet_y[j] <= 0 || greensatanL2[i].bulletshot[j] == false){
					greensatanL2[i].bullet_x[j] = greensatanL2[i].x + 18;
					greensatanL2[i].bullet_y[j] = greensatanL2[i].y + 1;
					greensatanL2[i].bulletshot[j] = false;
				}
				if (greensatanL2[i].y == greenhorn_rand_shotpos[j]){
					if (greensatanL2[i].bullet_index[j] == 10){
						greensatanL2[i].bullet_index[j] = 0;
					}
					greensatanL2[i].bulletshot[greensatanL2[i].bullet_index[j]++] = true;
				}
				greensatanL2[i].bullet_y[j] -= 15;
			}
			greensatanL2[i].y -= 4;
		}
	}
	//green satan change ends here

	//snake gun change starts here


	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < snakegunL2size; i++){
			if (snakegunL2[i].life == false || snakegunL2[i].x >= SCREENWIDTH - 242){
				snakegunL2[i].x = snakegun_random_x[rand() % 5];
				snakegunL2[i].y = snakegun_random_y[rand() % 5];
				snakegunL2[i].life = true;
				snakegunL2[i].lifevalue = 10;
			}
			for (int j = 0; j < snakegunL2[i].total_snakebite; j++){

				if (snakegunL2[i].snakebite_y[j] <= 0 || snakegunL2[i].snakebiteshot[j] == false){
					snakegunL2[i].snakebite_x[j] = snakegunL2[i].x + 12;
					snakegunL2[i].snakebite_y[j] = snakegunL2[i].y + 5;
					snakegunL2[i].snakebiteshot[j] = false;
				}
				if (snakegunL2[i].x == drone_rand_shotpos[j]){
					if (snakegunL2[i].snakebite_index[j] == 10){
						snakegunL2[i].snakebite_index[j] = 0;
					}
					snakegunL2[i].snakebiteshot[snakegunL2[i].snakebite_index[j]++] = true;
				}

				//snakegunL2[i].snakebite_x[j] = 50* sin(snakegunL2[i].snakebite_siny);
				snakegunL2[i].snakebite_y[j] -= 5;
				//snakegunL2[i].snakebite_siny -= 0.05;
			}
			snakegunL2[i].y = 80 * sin(snakegunL2[i].sinx) + 400;
			snakegunL2[i].sinx += 0.05;
			snakegunL2[i].x += 3;
		}

		//snake gun change ends here
	}


	//Whitedwarf
	if (gamemenu.play == true && level3 == true){
		for (int i = 0; i < whitedwarfL3size; i++){
			if (whitedwarfL3[i].y <= 0 || whitedwarfL3[i].life == false){
				whitedwarfL3[i].y = whitedwarf_random_y[rand() % 5];
				whitedwarfL3[i].x = whitedwarf_random_x[rand() % 5];
				whitedwarfL3[i].lifevalue = 10;
				whitedwarfL3[i].life = true;
			}
			for (int j = 0; j < whitedwarfL3[i].total_redball; j++){

				if (whitedwarfL3[i].redball_y[j] <= 0 || whitedwarfL3[i].redballshot[j] == false){
					whitedwarfL3[i].redball_x[j] = whitedwarfL3[i].x + 40;
					whitedwarfL3[i].redball_y[j] = whitedwarfL3[i].y + 19;
					whitedwarfL3[i].redballshot[j] = false;
				}
				if (whitedwarfL3[i].y == greenhorn_rand_shotpos[j]){
					if (whitedwarfL3[i].redball_index[j] == 10){
						whitedwarfL3[i].redball_index[j] = 0;
					}
					whitedwarfL3[i].redballshot[whitedwarfL3[i].redball_index[j]++] = true;
				}
				whitedwarfL3[i].redball_y[j] -= 10;
			}
			whitedwarfL3[i].y -= 2;
		}
	}
	//white dwarf change ends here



	//Boss change

	if (gamemenu.play == true && boss.life == true){
		if (boss.x == random_x || boss.y == random_y){
			bossRandom();
		}

		if (boss.y <random_y){
			boss.y++;
		}
		if (boss.y>random_y){
			boss.y--;
		}
		if (boss.x < random_x){
			boss.x++;
		}
		if (boss.x>random_x){
			boss.x--;
		}

		//Red laser initialization
		for (int i = 0; i < 8; i++){
			if (boss.lasershot[i] == false){

				boss.rlaser_x[0][i] = boss.x + 352;	boss.rlaser_x[1][i] = boss.x + 374;
				boss.rlaser_x[2][i] = boss.x + 395;	boss.rlaser_x[3][i] = boss.x + 418;
				boss.rlaser_x[4][i] = boss.x + 442;	boss.rlaser_x[5][i] = boss.x + 465;
				boss.rlaser_x[6][i] = boss.x + 488;	boss.rlaser_x[7][i] = boss.x + 510;

				//Yellow Laser initialiation

				boss.ylaser_x[0][i] = boss.x + 11;		boss.ylaser_x[1][i] = boss.x + 32;
				boss.ylaser_x[2][i] = boss.x + 56;		boss.ylaser_x[3][i] = boss.x + 76;
				boss.ylaser_x[4][i] = boss.x + 96;		boss.ylaser_x[5][i] = boss.x + 123;
				boss.ylaser_x[6][i] = boss.x + 147;		boss.ylaser_x[7][i] = boss.x + 170;

				for (int j = 0; j < 8; j++){
					boss.rlaser_y[i][j] = boss.y + 15;
					boss.ylaser_y[i][j] = boss.y + 15;
				}
			}

		}

		//Missle initialization
		boss.missle_x[0] = boss.x + 185;
		boss.missle_x[1] = boss.x + 205;
		boss.missle_x[2] = boss.x + 293;
		boss.missle_x[3] = boss.x + 313;
		boss.missle_x[4] = boss.x + 185;
		boss.missle_x[5] = boss.x + 205;
		boss.missle_x[6] = boss.x + 293;
		boss.missle_x[7] = boss.x + 313;

		for (int i = 0; i < 4; i++){
			boss.missle_y[i] = boss.y + 12;
		}

		for (int i = 4; i < 8; i++){
			boss.missle_y[i] = boss.y - 20;
		}

		boss.longlaser_x = boss.x + 220;
		boss.longlaser_y = boss.y - 565;
	}
}


//All collision fucntions starts here
//Wholla...Collision check function // Just Pass your value to it.. 
bool IsColliding(int x1, int y1, int dx1, int dy1, int x2, int y2, int dx2, int dy2){

	if ((x1 + dx1 > x2 && x2 + dx2 > x1) && (y1 + dy1 >y2 && y2 + dy2 > y1))
		return true;
	else
		return false;
}

//Checks all collsions on level 1
void allObjectCollisionChecker(){


	//Boss collision

	if (gamemenu.play == true && boss.life == true){
		if (IsColliding(boss.x, boss.y, boss.dx, boss.dy, playership1.x, playership1.y,
			playership1.dx, playership1.dy)){
			boss.lifevalue_x -= 30;
			healthboard.health_xsize -= 30;

			if (healthboard.health_xsize <= 0){
				gamemenu.gameover = true;
				gamemenu.play = false;
			}
			if (boss.lifevalue_x <= 0){
				boss.life = false;
				gamemenu.play = false;
				gamemenu.finalstagecompletion = true;
			}

		}
	}

	//Boss  VS Bluelaser
	if (gamemenu.play == true && boss.life == true){
		for (int i = 0; i < 100; i++){
			if (boss.life == true && bluelaser[i].shot == true){
				if (IsColliding(boss.x, boss.y, boss.dx, boss.dy,
					bluelaser[i].l_x, bluelaser[i].y, bluelaser[i].dx, bluelaser[i].dy)){
					bluelaser[i].shot = false;
					bluelaser[i].l_x = playership1.x + 28;
					bluelaser[i].r_x = playership1.x + 43;
					bluelaser[i].y = playership1.y + 60;
					boss.lifevalue_x -= 1;
					playerinfo.score += 1.25;
					if (boss.lifevalue_x <= 0){
						boss.life = false;
						gamemenu.play = false;
						gamemenu.finalstagecompletion = true;
					}
				}
			}
		}
	}


	//booster collision
	if (gamemenu.play == true && (level1 == true || level2 == true || level3 == true)){


		if (IsColliding(lifeboosterL1.x, lifeboosterL1.y, lifeboosterL1.dx,
			lifeboosterL1.dy, playership1.x, playership1.y, playership1.dx, playership1.dy)){

			if (healthboard.health_xsize < healthboard.healthsize){
				if (healthboard.health_xsize + lifeboosterL1.boostvalue>healthboard.healthsize){
					healthboard.health_xsize = healthboard.healthsize;
				}
				else{
					healthboard.health_xsize += lifeboosterL1.boostvalue;
				}

			}
			lifeboosterL1.showbooster = false;
			lifeboosterL1.x = rand() % 1000;
			lifeboosterL1.y = rand() % 2000;
		}
	}




	//Greenhorn and ship 1 collision 
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < greenhornL1size; i++){
			if (IsColliding(greenhornL1[i].x, greenhornL1[i].y, greenhornL1[i].dx, greenhornL1[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				healthboard.health_xsize -= 5;
				greenhornL1[i].life = false;
				playerinfo.score += 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}

			//Ship 1 and greenlaser collision
			for (int j = 0; j < greenhornL1[i].total_laser; j++){
				if (IsColliding(greenhornL1[i].laser_x[j], greenhornL1[i].laser_y[j],
					greenhornL1[i].laser_dx, greenhornL1[i].laser_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (greenhornL1[i].life == true){
						greenhornL1[i].lasershot[j] = false;
						healthboard.health_xsize -= greenhornL1[i].laserdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	//BLuelaser and Greenhorn Collision
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < greenhornL1size; j++){
				if (greenhornL1[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(greenhornL1[j].x, greenhornL1[j].y, greenhornL1[j].dx, greenhornL1[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						greenhornL1[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (greenhornL1[j].lifevalue <= 0){
							greenhornL1[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	//Drone and Ship 1 collsions
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < droneL1size; i++){
			if (IsColliding(droneL1[i].x, droneL1[i].y, droneL1[i].dx, droneL1[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				droneL1[i].life = false;
				healthboard.health_xsize -= 5;
				enemycounter++;
				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}
			}
			//Ship 1 and Drone Missle collision
			for (int j = 0; j < droneL1[i].total_missle; j++){
				if (IsColliding(droneL1[i].missle_x[j], droneL1[i].missle_y[j],
					droneL1[i].missle_dx, droneL1[i].missle_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (droneL1[i].life == true){
						droneL1[i].missleshot[j] = false;
						healthboard.health_xsize -= droneL1[i].missledamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	//Drone and blue laser Collsion
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < droneL1size; j++){
				if (droneL1[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(droneL1[j].x, droneL1[j].y, droneL1[j].dx, droneL1[j].dy,
						bluelaser[i].l_x, bluelaser[i].y, bluelaser[i].dx, bluelaser[i].dy)){

						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						droneL1[j].lifevalue -= 5;
						playerinfo.score += 1;
						if (droneL1[j].lifevalue <= 0){
							droneL1[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	//Greenspike and ship1 collision
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < greenspikeL1size; i++){
			if (IsColliding(greenspikeL1[i].x, greenspikeL1[i].y, greenspikeL1[i].dx, greenspikeL1[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				greenspikeL1[i].life = false;
				playerinfo.score += 3;
				healthboard.health_xsize -= 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}
			//spikeball and playership collsion
			for (int j = 0; j < greenspikeL1[i].total_spikeball; j++){
				if (IsColliding(greenspikeL1[i].spikeball_x[j], greenspikeL1[i].spikeball_y[j],
					greenspikeL1[i].spikeball_dx, greenspikeL1[i].spikeball_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (greenspikeL1[i].life == true){
						greenspikeL1[i].spikeballshot[j] = false;
						healthboard.health_xsize -= greenspikeL1[i].spikeballdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}
		}
	}

	//Bluelaser and greenspike colliosion
	if (gamemenu.play == true && level1 == true){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < greenspikeL1size; j++){
				if (greenspikeL1[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(greenspikeL1[j].x, greenspikeL1[j].y, greenspikeL1[j].dx, greenspikeL1[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						greenspikeL1[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (greenspikeL1[j].lifevalue <= 0){

							greenspikeL1[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	//Greenhorn and ship 1 collision 
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greenhornL2size; i++){
			if (IsColliding(greenhornL2[i].x, greenhornL2[i].y, greenhornL2[i].dx, greenhornL2[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				healthboard.health_xsize -= 5;
				greenhornL2[i].life = false;
				playerinfo.score += 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}

			//Ship 1 and greenlaser collision
			for (int j = 0; j < greenhornL2[i].total_laser; j++){
				if (IsColliding(greenhornL2[i].laser_x[j], greenhornL2[i].laser_y[j],
					greenhornL2[i].laser_dx, greenhornL2[i].laser_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (greenhornL2[i].life == true){
						greenhornL2[i].lasershot[j] = false;
						healthboard.health_xsize -= greenhornL2[i].laserdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	//BLuelaser and Greenhorn Collision
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < greenhornL2size; j++){
				if (greenhornL2[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(greenhornL2[j].x, greenhornL2[j].y, greenhornL2[j].dx, greenhornL2[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						greenhornL2[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (greenhornL2[j].lifevalue <= 0){
							greenhornL2[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	//Greenspike and ship1 collision
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greenspikeL2size; i++){
			if (IsColliding(greenspikeL2[i].x, greenspikeL2[i].y, greenspikeL2[i].dx, greenspikeL2[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				greenspikeL2[i].life = false;
				playerinfo.score += 3;
				healthboard.health_xsize -= 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}
			//spikeball and playership collsion
			for (int j = 0; j < greenspikeL2[i].total_spikeball; j++){
				if (IsColliding(greenspikeL2[i].spikeball_x[j], greenspikeL2[i].spikeball_y[j],
					greenspikeL2[i].spikeball_dx, greenspikeL2[i].spikeball_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (greenspikeL2[i].life == true){
						greenspikeL2[i].spikeballshot[j] = false;
						healthboard.health_xsize -= greenspikeL2[i].spikeballdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}
		}
	}

	//Bluelaser and greenspike colliosion
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < greenspikeL2size; j++){
				if (greenspikeL2[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(greenspikeL2[j].x, greenspikeL2[j].y, greenspikeL2[j].dx, greenspikeL2[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						greenspikeL2[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (greenspikeL2[j].lifevalue <= 0){

							greenspikeL2[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	// SnakeGun and ship 1 collision 
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < snakegunL2size; i++){
			if (IsColliding(snakegunL2[i].x, snakegunL2[i].y, snakegunL2[i].dx, snakegunL2[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				healthboard.health_xsize -= 5;
				snakegunL2[i].life = false;
				playerinfo.score += 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}

			//Ship 1 and snakebite collision
			for (int j = 0; j < snakegunL2[i].total_snakebite; j++){
				if (IsColliding(snakegunL2[i].snakebite_x[j], snakegunL2[i].snakebite_y[j],
					snakegunL2[i].snakebite_dx, snakegunL2[i].snakebite_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (snakegunL2[i].life == true){
						snakegunL2[i].snakebiteshot[j] = false;
						healthboard.health_xsize -= snakegunL2[i].snakebitedamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	//BLuelaser and snakegun collision
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < snakegunL2size; j++){
				if (snakegunL2[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(snakegunL2[j].x, snakegunL2[j].y, snakegunL2[j].dx, snakegunL2[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						snakegunL2[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (snakegunL2[j].lifevalue <= 0){
							snakegunL2[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	// GREENSATAN and ship 1 collision 
	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < greensatanL2size; i++){
			if (IsColliding(greensatanL2[i].x, greensatanL2[i].y, greensatanL2[i].dx, greensatanL2[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				healthboard.health_xsize -= 5;
				greensatanL2[i].life = false;
				playerinfo.score += 5;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}

			//Ship 1 and GREENSATAN  collision
			for (int j = 0; j < greensatanL2[i].total_bullet; j++){
				if (IsColliding(greensatanL2[i].bullet_x[j], greensatanL2[i].bullet_y[j],
					greensatanL2[i].bullet_dx, greensatanL2[i].bullet_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (greensatanL2[i].life == true){
						greensatanL2[i].bulletshot[j] = false;
						healthboard.health_xsize -= greensatanL2[i].bulletdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < greensatanL2size; j++){
				if (greensatanL2[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(greensatanL2[j].x, greensatanL2[j].y, greensatanL2[j].dx, greensatanL2[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						greensatanL2[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (greensatanL2[j].lifevalue <= 0){
							greensatanL2[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}

	//WHitedwarf
	if (gamemenu.play == true && level3 == true){
		for (int i = 0; i < whitedwarfL3size; i++){
			if (IsColliding(whitedwarfL3[i].x, whitedwarfL3[i].y, whitedwarfL3[i].dx, whitedwarfL3[i].dy,
				playership1.x, playership1.y, playership1.dx, playership1.dy)){

				healthboard.health_xsize -= 5;
				whitedwarfL3[i].life = false;
				playerinfo.score += 3;
				enemycounter++;

				if (healthboard.health_xsize <= 0){
					gamemenu.gameover = true;
					gamemenu.play = false;
				}

			}

			//Ship 1 and GREENSATAN  collision
			for (int j = 0; j < whitedwarfL3[i].total_redball; j++){
				if (IsColliding(whitedwarfL3[i].redball_x[j], whitedwarfL3[i].redball_y[j],
					whitedwarfL3[i].redball_dx, whitedwarfL3[i].redball_dy,
					playership1.x, playership1.y, playership1.dx, playership1.dy)){

					if (whitedwarfL3[i].life == true){
						whitedwarfL3[i].redballshot[j] = false;
						healthboard.health_xsize -= whitedwarfL3[i].redballdamage;
					}
					if (healthboard.health_xsize <= 0){
						gamemenu.gameover = true;
						gamemenu.play = false;
					}
				}
			}

		}
	}

	if (gamemenu.play == true && (level2 == true || level3 == true)){
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < whitedwarfL3size; j++){
				if (whitedwarfL3[j].life == true && bluelaser[i].shot == true){
					if (IsColliding(whitedwarfL3[j].x, whitedwarfL3[j].y, whitedwarfL3[j].dx, whitedwarfL3[j].dy,
						bluelaser[i].l_x, bluelaser[i].y - 8, bluelaser[i].dx, bluelaser[i].dy - 8)){
						bluelaser[i].shot = false;
						bluelaser[i].l_x = playership1.x + 28;
						bluelaser[i].r_x = playership1.x + 43;
						bluelaser[i].y = playership1.y + 60;
						whitedwarfL3[j].lifevalue -= 4;
						playerinfo.score += 1.25;
						if (whitedwarfL3[j].lifevalue <= 0){
							whitedwarfL3[j].life = false;
							enemycounter++;
						}
					}
				}
			}
		}
	}


}

//Level Checking
void levelChecking(int x){
	if (x == 2){
		level1 = false;
		gamemenu.level1completion = true;
	}
	else if (x == 3){
		level1 = false;
		level2 = false;
		gamemenu.level2completion = true;
	}
	else if (x == 4){
		level1 = false;
		level2 = false;
		level3 = false;
		bosslevelintro = true;
	}
}

//Value Resetter after game Restart
void valueRsetter(){

	level1 = true;
	level2 = false;
	level3 = false;
	bosslevelintro = false;
	//Enemy Variables level 1
	enemycounter = 0;
	level1AllObjectsInit();
	level2_level3AllObjectsInit();
	//Playership variables level 1
	int  y = playership1.y + 80;
	playership1.x = SCREENWIDTH / 2 - 150;
	playership1.y = 0;

	//blue spikeball resetting
	for (int i = 0; i < 100; i++){
		bluelaser[i].l_x = playership1.x + 25;
		bluelaser[i].r_x = playership1.x + 35;
		bluelaser[i].y = playership1.y + 80;
	}
	playerinfo.score = 0;
	healthboard.health_xsize = healthboard.healthsize;

}

int main()
{


	level1AllObjectsInit();
	PlayerScoreboard();
	PlaySound("sound\\menu_track", NULL, SND_ASYNC);

	iSetTimer(4, allObjectChange);

	iInitialize(SCREENWIDTH, SCREENHEIGHT, "SPACE WAR");
	return 0;
}