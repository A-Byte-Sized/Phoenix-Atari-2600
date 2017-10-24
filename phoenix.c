/*
 * Jessica Schlesiger
 * Project 2
 * Recreates the Phoenix atari Game
 */

#include <GL/glut.h> 
#include <stdio.h> 
#include <time.h> 
#include <stdlib.h> 
#include <math.h>

#define START_WIDTH 800 // Size of the board
#define TRUE 1
#define FALSE 0
#define MAX_SHIP_BULLETS 7
#define SHIP_FIRE_RATE 10
#define SHIP_BULLET_SPEED 35
#define MAX_ENEMY_BULLETS 90
#define ENEMY_BULLET_SPEED 20
#define STARTING_ENEMIES 11 // MUST BE ODD
#define neededIndicies 88

int ENEMY_FIRE_RATE = 70;
static unsigned int indicies[neededIndicies];

typedef struct {
  int x, y, active;
}
Bullet;

typedef struct {
  int x, y, active, reload, justMoved, stopMoving;
}
Enemy;

Bullet shipBullets[MAX_SHIP_BULLETS];
Bullet enemyBullets[MAX_ENEMY_BULLETS];

Enemy enemies[STARTING_ENEMIES];

long Time, ResetTime = 0.0;
int originalSpace = 0, newSpace = (START_WIDTH / 2) - 12,
cycle = 1, enemyCycle = 1, oldCycle = 1, width = START_WIDTH,
height = START_WIDTH * .70, numOfLives = 3, reload = 0,
enemyMoveLeft = TRUE, moveEnemy = 0, bulletCycle = 0, keys[4],
numAliveEnemies = STARTING_ENEMIES, score, oldScore = -1,
leftmostEnemy, rightmostEnemy;
void initializeEnemies();
void shipMoveLeft();
void shipMoveRight();
void createLives();
void createScoreboard();
void keyRelease(unsigned char key, int x, int y);
void keyPress(unsigned char key, int x, int y);
void bullet(float r, float g, float b);
void moveBullet();
void fireShipBullet();
void shipMoveLeft();
void shipMoveRight();
void moveEnemies();
int getRandomNum(int range);
void resetLife();
void hitPlayerRecognition(int bulletToCheck);
void fireEnemyBullet(int enemyThatFired);
void hitRecognition(int bulletToCheck);
void reshape(int w, int h);
void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

static int window;
static int menu_id, subMenu_id;
static int value = 0;
int pause = 1000;
float enemyShip[] = {
  /////// STATIC PARTS /////////////
  22, 0, 0.65, 0.31, 0.85, // Top horizontal right
  22, 7, 0.65, 0.31, 0.85,
  34, 7, 0.65, 0.31, 0.85,
  34,0, 0.65, 0.31, 0.85,
  
  -3, 0, 0.65, 0.31, 0.85, // top horizontal left
  -3, 7, 0.65, 0.31, 0.85,
  8, 7, 0.65, 0.31, 0.85,
  8,0, 0.65, 0.31, 0.85,
  
  4, 3, 0.65, 0.31, 0.85, // mid horizontal
  4, -3, 0.65, 0.31, 0.85,
  26, -3, 0.65, 0.31, 0.85,
  26,3, 0.65, 0.31, 0.85,
  
  10, 0, 0.65, 0.31, 0.85, // bot horizontal
  10, -6, 0.65, 0.31, 0.85,
  20, -6, 0.65, 0.31, 0.85,
  20,0, 0.65, 0.31, 0.85,
  
  //////////// ORANGE STATIC PARTS ///////////////
  10, -10, 0.79, 0.58, 0.53, // bot horizontal
  10, -6, 0.79, 0.58, 0.53,
  20, -6, 0.79, 0.58, 0.53,
  20,-10, 0.79, 0.58, 0.53,
  
  10, -10, 0.79, 0.58, 0.53, // bot left foot
  10, -13, 0.79, 0.58, 0.53,
  3, -13, 0.79, 0.58, 0.53,
  3,-10, 0.79, 0.58, 0.53,
  
  20, -10, 0.79, 0.58, 0.53, // bot right foot
  20, -13, 0.79, 0.58, 0.53,
  27, -13, 0.79, 0.58, 0.53,
  27,-10, 0.79, 0.58, 0.53,
  
  22, 10, 0.79, 0.58, 0.53, // Head
  22, 14, 0.79, 0.58, 0.53,
  8, 14, 0.79, 0.58, 0.53,
  8,10, 0.79, 0.58, 0.53,
  
  /////////////// IDLE SHIP PARTS //////////////
  31,7, 0.65, 0.31, 0.85, // arm right
  31,-6, 0.65, 0.31, 0.85,
  38, -6, 0.65, 0.31, 0.85,
  38, 7, 0.65, 0.31, 0.85,
  
  -1, 7, 0.65, 0.31, 0.85, // arm left
  -1, -6, 0.65, 0.31, 0.85,
  -8, -6, 0.65, 0.31, 0.85,
  -8, 7, 0.65, 0.31, 0.85,
  
  //////////////// ORANGE IDLE PARTS ////////////
  31,-10, 0.79, 0.58, 0.53, // hand right
  31,-6, 0.79, 0.58, 0.53,
  38, -6, 0.79, 0.58, 0.53,
  38, -10, 0.79, 0.58, 0.53,
  
  -1, -10, 0.79, 0.58, 0.53, // hand left
  -1, -6, 0.79, 0.58, 0.53,
  -8, -6, 0.79, 0.58, 0.53,
  -8, -10, 0.79, 0.58, 0.53,
  
  27,7, 0.79, 0.58, 0.53, // shoulder right
  27,10, 0.79, 0.58, 0.53,
  38, 10, 0.79, 0.58, 0.53,
  38, 7, 0.79, 0.58, 0.53,
  
  27,7, 0.79, 0.58, 0.53, // shoulder right tip
  27,14, 0.79, 0.58, 0.53,
  31, 14, 0.79, 0.58, 0.53,
  31, 7, 0.79, 0.58, 0.53,
  
  3, 7, 0.79, 0.58, 0.53, // shoulder left
  3, 10, 0.79, 0.58, 0.53,
  -8, 10, 0.79, 0.58, 0.53,
  -8, 7, 0.79, 0.58, 0.53,
  
  3, 7, 0.79, 0.58, 0.53, // shoulder left tip
  3, 14, 0.79, 0.58, 0.53,
  -1, 14, 0.79, 0.58, 0.53,
  -1, 7, 0.79, 0.58, 0.53,
  
  //////////// ARMS IN SPRIT PARTS //////////////
  26,7, 0.65, 0.31, 0.85, // arm right
  26,-6, 0.65, 0.31, 0.85,
  34, -6, 0.65, 0.31, 0.85,
  34, 7, 0.65, 0.31, 0.85,
  
  4, 7, 0.65, 0.31, 0.85, // arm left
  4, -6, 0.65, 0.31, 0.85,
  -3, -6, 0.65, 0.31, 0.85,
  -3, 7, 0.65, 0.31, 0.85,
  /////////// ARMS IN ORANGE SPRIT PARTS ///////////
  26,-10, 0.79, 0.58, 0.53, // hand right
  26,-6, 0.79, 0.58, 0.53,
  34, -6, 0.79, 0.58, 0.53,
  34, -10, 0.79, 0.58, 0.53,
  
  4, -10, 0.79, 0.58, 0.53, // hand left
  4, -6, 0.79, 0.58, 0.53,
  -3, -6, 0.79, 0.58, 0.53,
  -3, -10, 0.79, 0.58, 0.53,
  
  26,7, 0.79, 0.58, 0.53, // shoulder right tip
  26,14, 0.79, 0.58, 0.53,
  34, 14, 0.79, 0.58, 0.53,
  34, 7, 0.79, 0.58, 0.53,
  
  4, 7, 0.79, 0.58, 0.53, // shoulder left tip
  4, 14, 0.79, 0.58, 0.53,
  -3, 14, 0.79, 0.58, 0.53,
  -3, 7, 0.79, 0.58, 0.53
};
float ship[] = {
  ////////// STATIC PARTS ///////
  5, 16, 0.97, 0.45, 0.47, // Body
  20, 16, 0.97, 0.45, 0.47,
  20, 8, 0.97, 0.45, 0.47,
  5, 8, 0.97, 0.45, 0.47,
  
  10, 27, 0.97, 0.45, 0.47, // Head
  15, 27, 0.97, 0.45, 0.47,
  15, 4, 0.97, 0.45, 0.47,
  10, 4, 0.97, 0.45, 0.47,
  
  20, 22, 0.97, 0.45, 0.47, // Arm Connector
  20, 19, 0.97, 0.45, 0.47,
  5, 19, 0.97, 0.45, 0.47,
  5, 22, 0.97, 0.45, 0.47,
  
  ////////// IDLE PARTS //////////
  0, 11, 0.97, 0.45, 0.47, // left leg
  5, 11, 0.97, 0.45, 0.47,
  5, 0, 0.97, 0.45, 0.47,
  0, 0, 0.97, 0.45, 0.47,
  
  20, 11, 0.97, 0.45, 0.47, // right leg
  25, 11, 0.97, 0.45, 0.47,
  25, 0, 0.97, 0.45, 0.47,
  20, 0, 0.97, 0.45, 0.47,
  
  0, 24, 0.97, 0.45, 0.47, // Top left arm
  5, 24, 0.97, 0.45, 0.47,
  5, 16, 0.97, 0.45, 0.47,
  0, 16, 0.97, 0.45, 0.47,
  
  20, 24, 0.97, 0.45, 0.47, // Top right arm
  25, 24, 0.97, 0.45, 0.47,
  25, 16, 0.97, 0.45, 0.47,
  20, 16, 0.97, 0.45, 0.47,
  
  ////////// MOVING PARTS /////////
  -5, 29, 0.97, 0.45, 0.47, // Top left arm top part
  0, 29, 0.97, 0.45, 0.47,
  0, 25, 0.97, 0.45, 0.47,
  -5, 25, 0.97, 0.45, 0.47,
  0, 25, 0.97, 0.45, 0.47, // Top left arm bot part
  5, 25, 0.97, 0.45, 0.47,
  5, 21, 0.97, 0.45, 0.47,
  0, 21, 0.97, 0.45, 0.47,
  
  25, 29, 0.97, 0.45, 0.47, // Top right arm top part
  30, 29, 0.97, 0.45, 0.47,
  30, 25, 0.97, 0.45, 0.47,
  25, 25, 0.97, 0.45, 0.47,
  25, 25, 0.97, 0.45, 0.47, // Top right arm bot part
  20, 25, 0.97, 0.45, 0.47,
  20, 21, 0.97, 0.45, 0.47,
  25, 21, 0.97, 0.45, 0.47,
  
  0, 11, 0.97, 0.45, 0.47, // left leg top part
  5, 11, 0.97, 0.45, 0.47,
  5, 6, 0.97, 0.45, 0.47,
  0, 6, 0.97, 0.45, 0.47,
  -5, 6, 0.97, 0.45, 0.47, // left leg bot part
  0, 6, 0.97, 0.45, 0.47,
  0, 0, 0.97, 0.45, 0.47,
  -5, 0, 0.97, 0.45, 0.47,
  
  20, 11, 0.97, 0.45, 0.47, // right leg top part
  25, 11, 0.97, 0.45, 0.47,
  25,6, 0.97, 0.45, 0.47,
  20, 6, 0.97, 0.45, 0.47,
  25, 6, 0.97, 0.45, 0.47, // right leg bot part
  30, 6, 0.97, 0.45, 0.47,
  30, 0, 0.97, 0.45, 0.47,
  25, 0, 0.97, 0.45, 0.47,
  
  20, -20, 1, 1, 1, // Shield
  0, 0, 1, 1, 1,
  -20, -20, 1, 1, 1,
  0, -40, 1, 1, 1
};
/** All small things like the ground, clear screen, etc **/
float misc[] = {
  0, 0, 0, 0, 0, // Clears the board
  800, 0, 0, 0, 0,
  800, 560, 0, 0, 0,
  0, 560, 0, 0, 0,
  
  0, 0, 0.65, 0.31, 0.85, // Clears the Ground
  800, 0, 0.65, 0.31, 0.85,
  800, 20, 0.65, 0.31, 0.85,
  0, 20, 0.65, 0.31, 0.85,
};

int instructionsOn = TRUE;
/** Handles interaction from the user **/
void menu(int num) {
  switch (num) {
  case 0: // Quits the game
    glutDestroyWindow(window);
    exit(0);
    break; // Pauses the game
  case 1:
    pause = 1000;
    break; // Starts the game
  case 2:
    instructionsOn = FALSE;
    pause = 0;
    break;
  case 3: // Shows the instructions & Pauses
    pause = 1000;
    instructionsOn = TRUE;
    break;
  case 4: // Sets game to HARD
    ENEMY_FIRE_RATE = 10;
    numOfLives = 1;
    break;
  case 5: // Sets game to MEDIUM
    ENEMY_FIRE_RATE = 30;
    numOfLives = 2;
    break;
  case 6: // Sets game to EASY
    ENEMY_FIRE_RATE = 50;
    numOfLives = 3;
    break;
  case 7: // Sets game to IMPOSSIBLE
    ENEMY_FIRE_RATE = 3;
    numOfLives = 0;
    break;
  }
}

/** Creates the Menu **/
void createMenu(void) {
  subMenu_id = glutCreateMenu(menu);
  glutAddMenuEntry("Impossible", 7); // Reduces to 0 lives and increaed fire
  glutAddMenuEntry("Hard", 4); // Reduces to 1 life and increaed fire rate
  glutAddMenuEntry("Medium", 5); // Reduces to 2 lives and increased fire rate
  glutAddMenuEntry("Easy", 6); // Default Setting

  menu_id = glutCreateMenu(menu);
  glutAddMenuEntry("Start", 2);
  glutAddMenuEntry("Pause", 1);
  glutAddSubMenu("Difficulty", subMenu_id);
  glutAddMenuEntry("Instructions", 3);
  glutAddMenuEntry("Quit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int shieldCooldown = 0, shieldDuration = 0;

/** Adds increments to various sprites (bullet, enemy) and refreshes the screen to create the animation **/
void idle(void) {
  Time = clock();
  if (Time > ResetTime) {
    ResetTime = ResetTime + 0.08 * CLOCKS_PER_SEC;
    glutPostRedisplay();
    enemyCycle++;
    if (oldCycle == cycle) { // This resets the ship to its idle sprite if not moving
      cycle = 0;
    } else if (oldCycle < cycle)
      oldCycle++;
    reload--; // Reduces how much longer it will be till the ship can shoot again
    moveBullet();
    if (pause == FALSE) { // If the game is not paused, then recognize keystrikes
      if (shieldDuration == 0) { // Player cannot move while shield is up
        if (keys[0] == TRUE)
          shipMoveLeft();
        if (keys[1] == TRUE)
          shipMoveRight();
      }
      if (keys[2] == TRUE && reload < 0) // If shoot key is pressed down and has reloaded, then fire
        fireShipBullet();
      moveEnemies();
      shieldCooldown--;
    } else
      pause--;

  }
}

int randomMover1 = 0, randomMover2 = 1, justMoved = FALSE, moveUp = 40;
/** Creates the text on the screen **/
void createText(char * text, int length, int x, int y, float size) { //pause=100;
  glColor3f(0.91, 0.95, 0.27);
  int spacing = size * 120;
  int thickness = size * 16;
  for (int i = 0; i < length; i++) {
    glPushMatrix();
    glLineWidth(thickness);
    glTranslatef(x + (i * spacing), y, 0.0);
    glScalef(size, size, 1.0);
    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text[i]);
    glPopMatrix();
  }
}

/** Creates the graphics **/
void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), & misc[2]);
  glVertexPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), & misc);
  glDrawElements(GL_QUADS, 8, GL_UNSIGNED_INT, indicies); // Clears the board & draws the ground
  createScoreboard();

  ////////// PLAYER'S SHIP ///////////
  if (numOfLives >= 0) { // Draws it only if not gameover
    glColorPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), & ship[2]);
    glVertexPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), & ship);
    if (keys[3] == TRUE && shieldCooldown < 0) { // If the shield can be used, then use it
      shieldCooldown = 70;
      shieldDuration = 30;
    }
    if (shieldDuration > 0) { // Duration from being pressed is not over
      glPushMatrix();
      glTranslatef(13 + newSpace, 60, 0);
      glScalef(1.25, 1.25, 0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indicies + 60);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glPopMatrix();
      shieldDuration--;
    }
    glPushMatrix();
    glTranslatef(0 + newSpace, 23, 0);
    if (cycle % 2 == 0) { // Draws moving ship
      glDrawElements(GL_QUADS, 28, GL_UNSIGNED_INT, indicies); // First 12 static parts, rest idle parts
    } else { // Draws idle ship
      glDrawElements(GL_QUADS, 12, GL_UNSIGNED_INT, indicies); // Static parts
      glDrawElements(GL_QUADS, 32, GL_UNSIGNED_INT, indicies + 28); // Idle Parts
    }
    glPopMatrix();
    createLives();
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) { // Draws all active ship bullets
      if (shipBullets[i].active == TRUE) {
        glPushMatrix();
        glTranslatef(shipBullets[i].x, shipBullets[i].y, 0);
        bullet(0.91, 0.95, 0.27);
        glPopMatrix();
      }
    }
  }
  ////////// ENEMY SHIP ////////
  glColorPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), & enemyShip[2]);
  glVertexPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), & enemyShip);
  if (enemies[randomMover1].active == FALSE)
    randomMover1 = getRandomNum(STARTING_ENEMIES);
  if (enemies[randomMover2].active == FALSE)
    randomMover2 = getRandomNum(STARTING_ENEMIES);
  while (randomMover1 == randomMover2 && !(numAliveEnemies < 2))
    randomMover2 = getRandomNum(STARTING_ENEMIES);
  for (int i = 0; i < STARTING_ENEMIES; i++) {
    if (enemies[i].active == TRUE) { // Only draw active enemies
      if (pause <= 0) {
        if (i == randomMover1 || i == randomMover2) { // Two randomly moving enemies
          if (enemies[i].stopMoving == FALSE) { // they are supposed to occasionally stop moving
            int randomMove = getRandomNum(20 + 1);
            enemies[i].x = enemies[i].x + randomMove - 10;
            randomMove = getRandomNum(20 + 1);
            if (moveUp >= 20) {
              enemies[i].y = enemies[i].y + randomMove;
              if (enemies[i].y > height - 100)
                enemies[i].y = height - 100;
            } else {
              enemies[i].y = enemies[i].y - randomMove;

              if (enemies[i].y < 70)
                enemies[i].y = 70;
            }
            moveUp--;
            if (moveUp == 0) { // Forces enemies to go up/down
              moveUp = 40;
            }
            enemies[i].justMoved++;
          }
          if (enemies[i].justMoved % 10 == 0 && enemies[i].stopMoving <= 10) { // Makes the enemies not constantly move
            enemies[i].stopMoving++;
          } else
            enemies[i].stopMoving = FALSE;
        }
      }
      glPushMatrix();
      glTranslatef(enemies[i].x + moveEnemy, enemies[i].y, 0); // Positions enemy
      if (pause <= 0) { // Prevents it from trying to shoot & check for left/rightmost enemy if paused
        if (enemies[i].reload == 0)
          fireEnemyBullet(i);
        else
          enemies[i].reload--;

        if (enemies[i].x + moveEnemy < leftmostEnemy) // Enemy leftmost?
          leftmostEnemy = enemies[i].x + moveEnemy;
        else if (enemies[i].x + moveEnemy > rightmostEnemy) // Enemy rightmost?
          rightmostEnemy = enemies[i].x + moveEnemy;
      }
      if (enemyCycle % 3 == 0) // Swaps sprits so they're moving
        glDrawElements(GL_QUADS, 64, GL_UNSIGNED_INT, indicies); // first 32 static parts, rest idle parts
      else {
        glDrawElements(GL_QUADS, 32, GL_UNSIGNED_INT, indicies); // Static parts
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indicies + 64); // Arms in parts
      }
      glPopMatrix();
    }
  }
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) { // Draws all active enemy ship bullets
    if (enemyBullets[i].active == TRUE) {
      glPushMatrix();
      glTranslatef(enemyBullets[i].x, enemyBullets[i].y, 0);
      if (enemyCycle % 2 == 1)
        bullet(1, 1, 1);
      else
        bullet(1, .5, .5);
      glPopMatrix();
    }
  }
  /////  TEXT ON SCREEN ///////
  if (numAliveEnemies == 0) { // Creates "you win" message
    createText("YOU WIN!", 8, (width / 2) - 100, height / 2, .25);
  } else if (numOfLives < 0) { // Creates "game over" message
    pause = 100;
    createText("GAME OVER!", 10, (width / 2) - 140, height / 2, .25);
  } else if (pause > 0) { // Creates 'paused' message
    createText("PAUSED", 6, (width / 2) - 90, height / 2, .25);
  }
  if (instructionsOn == TRUE) { // Creates instructions message
    createText("Move Right: a\tMove Left: d", 27, width / 2 - 220, height / 2 - 100, .15);
    createText("Shoot: w\tShield: Spacebar", 26, width / 2 - 210, height / 2 - 150, .15);
    createText("Right click for menu.", 21, width / 2 - 180, height / 2 - 200, .15);
  }
  glFlush();
}

/** Initializes the world **/
int main(int argc, char * * argv) {
  for (int i = 0; i < neededIndicies; i++) { // Fills the indicies array up
    indicies[i] = i;
  }
  initializeEnemies(); // Creates the starting enemies on the board
  srand(time(0)); // seeds random number generator
  randomMover1 = getRandomNum(STARTING_ENEMIES);
  randomMover2 = getRandomNum(STARTING_ENEMIES);
  while (randomMover1 == randomMover2 && !(numAliveEnemies < 2)) {
    randomMover2 = getRandomNum(STARTING_ENEMIES);
  }
  glutInit( & argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);
  window = glutCreateWindow("Phoenix Atari");
  createMenu();
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  init();
  glutKeyboardFunc(keyPress);
  glutKeyboardUpFunc(keyRelease);
  glutDisplayFunc(display);

  glutMainLoop();
  return 0;
}

/** Ensures the viewport will always be the correct ratio (height = 70% of width) **/
void reshape(int w, int h) {
  if ((w * .7) > h)
    glViewport(0, 0, (GLsizei) h / .7, (GLsizei) h);
  else
    glViewport(0, 0, (GLsizei) w, (GLsizei) w * .7);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, width, 0.0, height);
}

/** Creates however number of lives the player has **/
void createLives() {
  int i;
  glPushMatrix();
  for (i = 0; i < numOfLives; i++) {
    glTranslatef((width / 2 + 73) - (15 * i), height - 50, 1.0);
    glScalef(0.5, 0.5, 1.0);
    glDrawElements(GL_QUADS, 28, GL_UNSIGNED_INT, indicies); // First 12 static parts, rest idle parts
    glPopMatrix();
    glPushMatrix();
  }
}

/** Creates the scoreboard, max score length is 6 **/
void createScoreboard() {
  int temp = score, leadingNum;
  char arr[6];
  for (int i = 0; i < 6; i++) { // Converts the score to a char array
    arr[i] = (char)('0' + (temp % 10));
    if (temp % 10 != 0)
      leadingNum = i; // Ensures we don't have leading 0s
    temp /= 10;
  }
  char inverse[6];
  for (int i = 0; i < 6; i++) { // Fills with spaces
    inverse[i] = ' ';
  }
  for (int i = leadingNum; i > -1; i--) { // Reverses bcause createText prints left to right
    inverse[5 - i] = arr[i];
  }
  createText(inverse, 6, width / 2 - 90, height - 30, 0.25);
}

/** Creates bullets, used by ship and enemies **/
void bullet(float r, float g, float b) {
  glColor3f(r, g, b);
  glBegin(GL_QUADS);
  glVertex2i(0, 0);
  glVertex2i(0, 10);
  glVertex2i(3, 10);
  glVertex2i(3, 0);
  glEnd();
}

/** If the key is released, then it edits the array to FALSE **/
void keyRelease(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
    keys[0] = FALSE;
    break;
  case 'd':
    keys[1] = FALSE;
    break;
  case 'w':
    keys[2] = FALSE;
    break;
  case ' ':
    keys[3] = FALSE;
    break;
  }
}

/** If the key is pressed down, then edits array to TRUE **/
void keyPress(unsigned char key, int x, int y) {
  switch (key) {
  case 'a':
    keys[0] = TRUE;
    break;
  case 'd':
    keys[1] = TRUE;
    break;
  case 'w':
    keys[2] = TRUE;
    break;
  case ' ':
    keys[3] = TRUE;
    break;
  }
}

/** Moves the bullets **/
void moveBullet() {
  for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
    if (shipBullets[i].active == TRUE) {
      hitRecognition(i);
      shipBullets[i].y = shipBullets[i].y + SHIP_BULLET_SPEED;
      if (shipBullets[i].y > height) {
        shipBullets[i].active = FALSE;
      }
    }
  }
  for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
    if (enemyBullets[i].active == TRUE) {
      hitPlayerRecognition(i);
      enemyBullets[i].y = enemyBullets[i].y - ENEMY_BULLET_SPEED;
      if (enemyBullets[i].y < 0) {
        enemyBullets[i].active = FALSE;
      }
    }
  }
}

/** Generate random number for enemy's reload time **/
int getRandomNum(int range) {
  return (int)(((float) rand()) / RAND_MAX * range);

}

/** Creates the starting position of the enemies **/
void initializeEnemies() {

  enemies[0].active = TRUE;
  enemies[0].x = (width / 2) - 15;
  enemies[0].y = 450;
  enemies[0].reload = getRandomNum(ENEMY_FIRE_RATE + 1);
  int i;
  for (i = 2; i < STARTING_ENEMIES; i = i + 2) {
    enemies[i].active = TRUE;
    enemies[i - 1].active = TRUE;
    enemies[i].x = (width / 2) - 15 + (25 * i);
    enemies[i - 1].x = ((width / 2) - 15) - (25 * i);
    enemies[i].y = 450 - 25 * i;
    enemies[i - 1].y = 450 - 25 * i;
    enemies[i].reload = getRandomNum(ENEMY_FIRE_RATE + 1);
    enemies[i - 1].reload = getRandomNum(ENEMY_FIRE_RATE + 1);
  }
  leftmostEnemy = enemies[(STARTING_ENEMIES - 2)].x;
  rightmostEnemy = enemies[(STARTING_ENEMIES - 1)].x;
}

int enemybulletCycle = 0;
/** creates new bullet for firing enemy **/
void fireEnemyBullet(int enemyThatFired) {

  enemies[enemyThatFired].reload = getRandomNum(ENEMY_FIRE_RATE + 1);
  enemyBullets[enemybulletCycle].x = enemies[enemyThatFired].x + 12 + moveEnemy;
  enemyBullets[enemybulletCycle].y = enemies[enemyThatFired].y - 20;
  enemyBullets[enemybulletCycle].active = TRUE;
  enemybulletCycle++;
  if (enemybulletCycle == MAX_ENEMY_BULLETS)
    enemybulletCycle = 0;
}

/** if W is pressed, then it creates a bullet from the ship **/
void fireShipBullet() {
  reload = SHIP_FIRE_RATE;
  shipBullets[bulletCycle].x = newSpace + 11;
  shipBullets[bulletCycle].y = 50;
  shipBullets[bulletCycle].active = TRUE;
  bulletCycle++;
  if (bulletCycle == MAX_SHIP_BULLETS)
    bulletCycle = 0;
}

/** Moves the ship left if A is pressed **/
void shipMoveLeft() {
  if (!(newSpace < 10))
    newSpace = newSpace - 20;
  oldCycle = cycle;
  cycle++;
}

/** move the ship right if D is pressed **/
void shipMoveRight() {
  if (!(newSpace > width - 35))
    newSpace = newSpace + 20;
  oldCycle = cycle;
  cycle++;
}

/** Moves the enemies left/right **/
void moveEnemies() {
  if (enemyMoveLeft == TRUE) { // Moves the enemy ships left or right
    moveEnemy = moveEnemy - 5;
  } else
    moveEnemy = moveEnemy + 5;
  if (leftmostEnemy < 20) { // If the enemy ships are close to the edge, then changes the direction they're moving
    leftmostEnemy = 800; // resets it
    enemyMoveLeft = FALSE;
  } else if (rightmostEnemy > 750) {
    enemyMoveLeft = TRUE;
    rightmostEnemy = 0; // resets it
  }
}

/** When a player looses a life, this preps the game for it to restart **/
void resetLife() {
  newSpace = (START_WIDTH / 2) - 12; // Resets position for ship
  pause = 25;
  moveEnemy = 0; // Resets position for enemies
}

/** Recognizes when a bullet hits a player **/
void hitPlayerRecognition(int bulletToCheck) {
  if (pause == FALSE && shieldDuration == 0) {
    if (enemyBullets[bulletToCheck].active == TRUE) {
      int difference = abs(newSpace - (enemyBullets[bulletToCheck].x - 7));
      if (difference < 17) {
        difference = abs(23 - enemyBullets[bulletToCheck].y);
        if (difference < 20) {
          enemyBullets[bulletToCheck].active = FALSE;
          numOfLives--;
          resetLife();
        }
      }
    }
  }
}

/** Recognizes hits on enemies **/
void hitRecognition(int bulletToCheck) {
  if (pause == FALSE) {
    for (int j = 0; j < STARTING_ENEMIES; j++) {
      if (enemies[j].active == TRUE && shipBullets[bulletToCheck].active == TRUE) {
        int difference = abs(shipBullets[bulletToCheck].x - (enemies[j].x + 10 + moveEnemy)); // 5 helps shift to the middle
        if (difference < 20) {
          difference = abs(shipBullets[bulletToCheck].y + 10 - enemies[j].y);
          if (difference < 20) {
            enemies[j].active = FALSE;
            shipBullets[bulletToCheck].active = FALSE;
            score = score + 20;
            numAliveEnemies--;
          }
        }
      }
    }
  }
}