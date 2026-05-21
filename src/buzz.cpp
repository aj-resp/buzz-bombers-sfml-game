#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

//menu
int drawMenu(RenderWindow& window);
int drawLevelMenu(RenderWindow& window);


//player
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);

//moving bullet
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void handleMovement(const Event& e, float& player_x, float& player_y, int boxPixelsX, int boxPixelsY);
void handleBullet(const Event& e, bool& bullet_exists, float& bullet_x, float& bullet_y, float player_x, float player_y, int& spraysLeft, int& sprayLevel, int& sprayCans,Music &fireMusic);

//spray thingy
void useSpray(int& spraysLeft, int& sprayLevel, int& sprayCans); 


// bullet collisions
bool checkBulletCollision(float bulletX, float bulletY, bool& bulletExists, float beeX[], float beeY[], bool beeActive[], int totalBees, 
                          float honeycombX[], float honeycombY[], bool honeycombActive[], int &score, Music &collision) ;

bool checkBulletCollision_F(float bulletX, float bulletY, bool& bulletExists, float beeX[], float beeY[], bool beeActive[], int totalBees, 
                          float honeycombX[], float honeycombY[], bool honeycombActive[], int &score, Music &collision) ;               
                              
bool checkHoneycombCollision(float bulletX, float bulletY, bool& bulletExists,float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs, int &score,
                              float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs, Music &collision);            
bool checkHoneycombCollision_F(float bulletX, float bulletY, bool& bulletExists,float honeycombX[], float honeycombY[], bool honeycombActive[], 
                              int maxHoneycombs, int &score,Music &collision);                           
                          
                          
                          
//slow bee and honey comb          
void drawBees(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], bool beeActive[], int totalBees) ;
void updateBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees, float screenWidth,float beeSpeed, float rowHeight, float honeycombX[], float honeycombY[], bool honeycombActive[], 
                int maxHoneycombs, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs);
void spawnBees(float beeX[], float beeY[], bool beeActive[], int totalBees, int &currentBee, float interval, Clock &clock);
void initializeBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees);
void drawHoneycombs(RenderWindow &window, Sprite &honeySprite,
                    float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs,
                    float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs);

//Fast bee and honey comb
void drawHoneycombs_F(RenderWindow &window, Sprite &honeySprite, float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs);
void spawnBees_Fast(float beeX[], float beeY[], bool beeActive[], int totalBees, int &currentBee, float interval, Clock &clock) ;
void initializeBees_Fast(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees);
void updateBees_F(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees, float screenWidth, 
                float beeSpeed, float rowHeight);
void drawBees_Fast(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], bool beeActive[], int totalBees);



//humming bird 
void spawnHummingbird(RenderWindow& window, Sprite& HummingSprite, Texture& HummingTexture, 
                      float& hummingbirdX, float& hummingbirdY, float screenWidth, float screenHeight, 
                      float& speedX, float& speedY, bool& hummingActive, int &toggle,bool color_condition);
void updateHummingbird(float& hummingbirdX, float& hummingbirdY, 
                       float& speedX, float& speedY, 
                       bool& hummingActive, int& hummingbirdX_direction, int& hummingbirdY_direction, 
                       float resolutionX, float resolutionY, float boxPixelsX, float boxPixelsY,
                       float honeycombX[], float honeycombY[], int totalHoneycombs, Clock& clock, bool honeycombActive[], int& score,
                       float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs, float honeycombX_F[], float honeycombY_F[], bool 	 honeycombActive_F[], int maxHoneycombs_F, int choice);
                       
                       
void pollination(RenderWindow& window, float beeX[], float beeY[], bool beeActive[], int totalBees, bool obstaclePlaced[]);                       
                       
bool level_check(bool beeActive[], int totalBees, bool beeActive_Fast[], int totalBees_F, int levelResult);
bool appendScoreToFile(const std::string& playerName, int score);


void  gameOver(RenderWindow& window);        
int drawNameInputMenu(RenderWindow& window, string& playerName);
int drawHighScores(RenderWindow& window,  string& playerName);
               
                       
void playMusic(Music &bgmusic);
void fireSound(Music &fireMusic);



int main()
{
	int score = 0;
	int levelResult = -1;
	srand(time(0));
	
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);   
	window.setPosition(Vector2i(500, 200));
	
	string playerName="";
	int menuResult = drawMenu(window);
	
	if ( menuResult == 1)				// so do not go in level menu if end is selected
	{
		 levelResult = drawLevelMenu(window);
		
		 int check =  drawNameInputMenu(window, playerName);
		 int check2 = drawHighScores(window, playerName);
	}
	
	
	// main functions
	for (int i = levelResult ; i <= 3 ; )
	{
	
		float boxX = 32;
		float boxY = 32;
		
		// Initializing Player and Player Sprites.
		float player_x = (gameRows / 2) * boxPixelsX -32;  			// minus 32 so exaclty in centre 
		float player_y = (gameColumns - 4) * boxPixelsY;			

		// Texture and Sprite are classes from sfml library which serves two different purposes
		Texture playerTexture;						// we need two objects to load something on the screen and then rotate and stuff
		Sprite playerSprite;						// visual instance of the texture that can be drawn to the screen. It allows for positioning 
		playerTexture.loadFromFile("Textures/spray.png");
		playerSprite.setTexture(playerTexture);
		
		// Initializing Bullet and Bullet Sprites
		float bullet_x = player_x;			// so bullet has same starting point of charcter
		float bullet_y = player_y;
		bool bullet_exists = false;

		Clock bulletClock;
		Texture bulletTexture;
		Sprite bulletSprite;
		bulletTexture.loadFromFile("Textures/bullet.png");
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setScale(3, 3);
		bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

		
		
		// The ground on which player moves
		RectangleShape groundRectangle(Vector2f(960, 64));			// basically 1/10 of the screen
		groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);		// name is self explanatory
		groundRectangle.setFillColor(Color::Green);
		
		
		// Bee textures and sprites
		Texture beeTexture;
		beeTexture.loadFromFile("Textures/Regular_bee.png");
		Sprite beeSprite;
		beeSprite.setTexture(beeTexture);
		beeSprite.setScale(1, 1);
		
		int totalBees = 20;
		float beeX[totalBees], beeY[totalBees];
		bool beeActive[totalBees];
		int beeDirection[totalBees];
		int currentBee = 0;  // Index of the current bee
		float spawnInterval = 2.0f; // Spawn every 2 seconds
		Clock clock;
		initializeBees(beeX, beeY, beeActive, beeDirection, totalBees);
		
		float beeSpeed = 0.2f;
		float rowHeight = 32.0f;
		

		// Fast Bee textures and sprites
		Texture FastbeeTexture;
		FastbeeTexture.loadFromFile("Textures/Fast_bee.png");
		Sprite FastbeeSprite;
		FastbeeSprite.setTexture(FastbeeTexture);
		FastbeeSprite.setScale(1, 1);
		
		int totalBees_F = 10;
		float beeX_F[totalBees_F], beeY_F[totalBees_F];
		bool beeActive_Fast[totalBees_F];
		int beeDirection_Fast[totalBees_F];
		int currentBee_Fast = 0;  // Index of the current bee
		float spawnInterval_Fast = 2.0f; // Spawn every 2 seconds
		Clock clock_fast;
		float beeSpeed_F = 0.2f;
		float rowHeight_F = 32.0f;
		initializeBees_Fast(beeX_F,beeY_F,beeActive_Fast,beeDirection_Fast,totalBees_F);
		
		
		
		// honeycomb textures and sprites
		Texture honeyTexture;
		honeyTexture.loadFromFile("Textures/honeycomb.png");
		Sprite honeySprite;
		honeySprite.setTexture(honeyTexture);
		
		int maxHoneycombs = totalBees + 3; // Maximum number of honeycombs 
		int preGeneratedHoneycombs = 3;
		
		
		// honey comb fast textures and sprites
		Texture honeyTexture_F;
		honeyTexture_F.loadFromFile("Textures/honeycomb_red.png");
		Sprite honeySprite_F;
		honeySprite_F.setTexture(honeyTexture_F);
		
		int maxHoneycombs_F = totalBees_F;
		
		

	
		// Humming Bee textures and sprites
		Texture HummingTexture;
		HummingTexture.loadFromFile("Textures/bird.png");
		Sprite HummingSprite;
		HummingSprite.setTexture(HummingTexture);
		
		
		bool color_condition = true;
	   	float speedX,speedY;
		float hummingbirdX, hummingbirdY;
		bool hummingActive = false;
		int toggle = 1;
		int hummingbirdX_direction ,hummingbirdY_direction = 1;
		
		
		//cans
		int sprayCans = 3; // Start with 3 spray cans
		int spraysLeft = 56; // Each can starts with 56 sprays
		int sprayLevel = spraysLeft / 8; // Levels drop every 8 sprays
		
		
		// for bonus cans
		bool flag_20 = true;
		bool flag_40 = true;
		int spray_count_80 = 0;
		
		
		
		
		// Sound system
		Music fireMusic;
		if (!fireMusic.openFromFile("Sound Effects/fire.wav")) 
		{
			cout << "Error: Could not load music file!" << endl;
		}
		fireMusic.setVolume(10);
		
		Music collision;
		if (!collision.openFromFile("Sound Effects/hit.wav")) 
		{
			cout << "Error: Could not load music file!" << endl;
		}
		collision.setVolume(10);
		
		
		
		
		bool startFlag = true;
	
		// for seperate music and some bees settings as well 
		Music bgMusic;
		switch(levelResult)
		{
			case 1: 
			{
				beeSpeed = .3;
				totalBees = 20;
				maxHoneycombs = totalBees ;
				preGeneratedHoneycombs = 3;
				if (!bgMusic.openFromFile("Music/Music3.ogg")) 
				{
					cout << "Error: Could not load music file!" << endl;
				}
				bgMusic.setVolume(10);
				bgMusic.setLoop(true);
				bgMusic.play();
				break;
			}
			
			case 2:
			{
				beeSpeed = .4;
				beeSpeed_F = .6;
				totalBees = 15;
				maxHoneycombs = totalBees ;
				preGeneratedHoneycombs = 9;
				totalBees_F = 5;
				maxHoneycombs_F = totalBees_F;
				if (!bgMusic.openFromFile("Music/magical_journey.ogg")) 
				{
					cout << "Error: Could not load music file!" << endl;
				}
				bgMusic.setVolume(10);
				bgMusic.setLoop(true);
				bgMusic.play();
				break;
			
			
			}
			
			case 3:
			{
				beeSpeed = .6;
				beeSpeed_F = .8;
				totalBees = 20;
				maxHoneycombs = totalBees ;
				preGeneratedHoneycombs = 15;
				totalBees_F = 10;
				maxHoneycombs_F = totalBees_F;
				if (!bgMusic.openFromFile("Music/risk.ogg")) 
				{
					cout << "Error: Could not load music file!" << endl;
				}
				bgMusic.setVolume(10);
				bgMusic.setLoop(true);
				bgMusic.play();
				break;	
			}	
				
				
			default : break;
		
		}
		
		
		bool obstaclePlaced[totalBees] = {false};
		
		// fast bee honeycomb declarations
		float honeycombX_F[maxHoneycombs];
		float honeycombY_F[maxHoneycombs];
		bool honeycombActive_F[maxHoneycombs];
		for (int i= 0 ; i < maxHoneycombs_F ; i++)
		{
			honeycombActive_F[i] = false;
		}
		
		// slow bee declarations
		float honeycombX[maxHoneycombs];
		float honeycombY[maxHoneycombs];
		bool honeycombActive[maxHoneycombs];
		
		for (int i= 0 ; i < maxHoneycombs ; i++)
		{
			honeycombActive[i] = false;
		}
		
		//random positioniong of pre generated honey combs
		float preHoneycombX[preGeneratedHoneycombs] = {1}; // Example positions
		float preHoneycombY[preGeneratedHoneycombs] = {1};
		bool preHoneycombActive[preGeneratedHoneycombs] = {1};
		switch (preGeneratedHoneycombs)
		{
			case 3:
			{	
				for (int i= 0 ; i < 3 ; i++)
				{
					int randomX = rand() % 900 ;
					int randomY = rand() % 300 + 50;
					preHoneycombX[i] = randomX; // Example positions
					preHoneycombY[i] = randomY;
					preHoneycombActive[i] = true;
				}
				break;
			}
			case 9:
			{	
				for (int i= 0 ; i < 9 ; i++)
				{
					int randomX = rand() % 900 ;
					int randomY = rand() % 300 + 50;
					preHoneycombX[i] = randomX; // Example positions
					preHoneycombY[i] = randomY;
					preHoneycombActive[i] = true;
				}
				break;
			}
			case 15: 
			{	
				for (int i= 0 ; i < 15 ; i++)
				{
					int randomX = rand() % 900 ;
					int randomY = rand() % 300 + 50;
					preHoneycombX[i] = randomX; // Example positions
					preHoneycombY[i] = randomY;
					preHoneycombActive[i] = true;
				}
				break;
			}	
			default : ;
		
		}
	
	
		int flag = true;
		while (window.isOpen() && flag) 
		{
			Event e;
			while (window.pollEvent(e)) 
			{
				if (e.type == Event::Closed) 
				{
					return 0;
				}
				
				
					if (menuResult == 1 ) {
					
						switch(levelResult)
						{
						
							case 1 : 
							{	
								
												
								handleMovement(e, player_x, player_y, boxPixelsX, boxPixelsY);
			   			     		handleBullet(e, bullet_exists, bullet_x, bullet_y, player_x, player_y, 
			   			       			spraysLeft, sprayLevel, sprayCans,fireMusic);
			   					
					   			break;
					   		}
					   		case 2 :
					   		{
								
								handleMovement(e, player_x, player_y, boxPixelsX, boxPixelsY);
			   					handleBullet(e, bullet_exists, bullet_x, bullet_y, player_x, 
			   							player_y, spraysLeft, sprayLevel, sprayCans,fireMusic);
					   			break;
					   		}
					   		case 3 :
					   		{

								handleMovement(e, player_x, player_y, boxPixelsX, boxPixelsY);
			   					handleBullet(e, bullet_exists, bullet_x, bullet_y, player_x, 
			   							player_y, spraysLeft, sprayLevel, sprayCans,fireMusic);
					   			break;
					   		}
					   		default : return 0;
					   	}	
				   			
				   	 } 
				   	 else if (menuResult == 0) 
				   	 {
						return 0;
					}
			}
			
			switch(levelResult)
			{
			
				case 1 : 
				{
					
					spawnBees(beeX, beeY, beeActive, totalBees, currentBee, spawnInterval, clock);	
					drawBees(window, beeSprite, beeX, beeY, beeActive, totalBees);			       	//const  int is basically the total bees 
	 				updateBees(beeX, beeY, beeActive, beeDirection, totalBees, resolutionX, beeSpeed, rowHeight,
		          			honeycombX, honeycombY, honeycombActive, maxHoneycombs, preHoneycombX, preHoneycombY, preHoneycombActive, preGeneratedHoneycombs);
					
					
					
					spawnHummingbird(window, HummingSprite, HummingTexture, hummingbirdX, hummingbirdY,resolutionX,
								 rowHeight, speedX, speedY, hummingActive,toggle,color_condition);
								 
					updateHummingbird(hummingbirdX, hummingbirdY, speedX, speedY, hummingActive,hummingbirdX_direction,hummingbirdY_direction,resolutionX,
							resolutionY, boxPixelsX,boxPixelsY, honeycombX, honeycombY,  maxHoneycombs, clock, honeycombActive, 
							score,preHoneycombX, preHoneycombY, preHoneycombActive, preGeneratedHoneycombs, honeycombX_F, honeycombY_F, 
							honeycombActive_F,maxHoneycombs_F, levelResult );
				
						
		   			break;
		   		}
		   		case 2 :
		   		{
		   			//slow bees
		   			spawnBees(beeX, beeY, beeActive, totalBees, currentBee, spawnInterval, clock);	
					drawBees(window, beeSprite, beeX, beeY, beeActive, totalBees);
	 				updateBees(beeX, beeY, beeActive, beeDirection, totalBees, resolutionX, beeSpeed, rowHeight,
		       				    honeycombX, honeycombY, honeycombActive, maxHoneycombs, preHoneycombX, preHoneycombY,
		       				     preHoneycombActive, preGeneratedHoneycombs);
					
					// FAST bees
					spawnBees_Fast(beeX_F,beeY_F,beeActive_Fast,totalBees_F, currentBee_Fast, spawnInterval_Fast, clock_fast);
					drawBees_Fast(window, FastbeeSprite, beeX_F, beeY_F, beeActive_Fast, totalBees_F);
					updateBees_F(beeX_F, beeY_F, beeActive_Fast, beeDirection_Fast,  totalBees_F, resolutionX, beeSpeed_F,rowHeight);
					
		   			//
		   			spawnHummingbird(window, HummingSprite, HummingTexture, hummingbirdX, hummingbirdY,resolutionX,
		   					 rowHeight, speedX, speedY, hummingActive,toggle,color_condition);
					
					updateHummingbird(hummingbirdX, hummingbirdY, speedX, speedY, hummingActive,hummingbirdX_direction,
							hummingbirdY_direction,resolutionX,resolutionY, boxPixelsX,boxPixelsY, honeycombX, honeycombY,
							  maxHoneycombs, clock, honeycombActive, score,preHoneycombX, preHoneycombY, preHoneycombActive,
							   preGeneratedHoneycombs, honeycombX_F, honeycombY_F, honeycombActive_F,maxHoneycombs_F, levelResult );
		   				
		   			break;
		   		}
		   		case 3 :
		   		{
		   			//slow bees
					spawnBees(beeX, beeY, beeActive, totalBees, currentBee, spawnInterval, clock);	
					drawBees(window, beeSprite, beeX, beeY, beeActive, totalBees);
	 				updateBees(beeX, beeY, beeActive, beeDirection, totalBees, resolutionX, beeSpeed, rowHeight,
		        			   honeycombX, honeycombY, honeycombActive, maxHoneycombs, preHoneycombX, 
		        			   preHoneycombY, preHoneycombActive, preGeneratedHoneycombs);
		        			   
		        		// FAST bees	   
					spawnBees_Fast(beeX_F,beeY_F,beeActive_Fast,totalBees_F, currentBee_Fast, spawnInterval_Fast, clock_fast);
					drawBees_Fast(window, FastbeeSprite, beeX_F, beeY_F, beeActive_Fast, totalBees_F);
					updateBees_F(beeX_F, beeY_F, beeActive_Fast, beeDirection_Fast,  totalBees_F, resolutionX, beeSpeed_F,rowHeight);
					
					
					spawnHummingbird(window, HummingSprite, HummingTexture, hummingbirdX, hummingbirdY,
							resolutionX, rowHeight, speedX, speedY, hummingActive,toggle,color_condition);
					updateHummingbird(hummingbirdX, hummingbirdY, speedX, speedY, hummingActive,hummingbirdX_direction,
							hummingbirdY_direction,resolutionX,resolutionY, boxPixelsX,boxPixelsY, honeycombX, honeycombY,  maxHoneycombs, clock, 	
							honeycombActive, score,preHoneycombX, preHoneycombY, preHoneycombActive, preGeneratedHoneycombs,
							honeycombX_F, honeycombY_F, honeycombActive_F,maxHoneycombs_F, levelResult );		
		   			break;
		   		}
		   		default : return 0;
		   	}

			
			if (bullet_exists == true)
			{
					
				moveBullet(bullet_y, bullet_exists, bulletClock);
				drawBullet(window, bullet_x, bullet_y, bulletSprite);
				
				// so honey comb does not dissappear
	    			int flag = checkBulletCollision(bullet_x, bullet_y, bullet_exists, beeX, beeY, beeActive, totalBees,
	    							 honeycombX, honeycombY, honeycombActive, score, collision);
	    			if (!flag)
	    			{
	    				flag  = checkHoneycombCollision(bullet_x, bullet_y, bullet_exists, honeycombX,
	    						 honeycombY, honeycombActive, maxHoneycombs, score,
		                			preHoneycombX, preHoneycombY, preHoneycombActive, preGeneratedHoneycombs, collision);
				}
				if (!flag)
				{
					 flag = checkBulletCollision_F(bullet_x, bullet_y, bullet_exists, beeX_F, beeY_F, beeActive_Fast, totalBees_F, 
		               	    				honeycombX_F, honeycombY_F,  honeycombActive_F, score, collision);
				}
				
				if (!flag)
				{
					flag = checkHoneycombCollision_F(bullet_x, bullet_y, bullet_exists, honeycombX_F, honeycombY_F, honeycombActive_F, maxHoneycombs_F, 
		               	    	score, collision);
				
				}			
			}
			else
			{
				bullet_x = player_x;
				bullet_y = player_y;
			}
			drawHoneycombs(window, honeySprite, honeycombX, honeycombY, honeycombActive, maxHoneycombs,
					 preHoneycombX, preHoneycombY, preHoneycombActive,  preGeneratedHoneycombs);
			drawHoneycombs_F(window, honeySprite_F, honeycombX_F, honeycombY_F,honeycombActive_F,maxHoneycombs_F);
			
			
			pollination(window, beeX, beeY, beeActive, totalBees,obstaclePlaced);

			// drawing the green rectanle before the white tanks
			window.draw(groundRectangle);
			for (int i = 0; i < sprayCans - 1 ; i++) 
			{
				RectangleShape can(Vector2f(20, 60));
				can.setFillColor(Color(255, 165, 0));
				can.setPosition(50 * (i) , 580);
				window.draw(can);
			}
			
			// Draw spray meter outline
			RectangleShape meterOutline(Vector2f(7* 10, 20));         // used constants as the border will be fixed
			meterOutline.setPosition(player_x, player_y + 68);          // Same position as spray meter
			meterOutline.setOutlineThickness(2);                       // Border thickness
			meterOutline.setOutlineColor(Color::Black);                // Border color
			meterOutline.setFillColor(Color::Transparent);             // No fill color
			window.draw(meterOutline);
			
			Font font;
			if (!font.loadFromFile("Fonts/arial.ttf")) 
			{
				cout << "Error loading font!" << endl;
				return -1; // Return -1 if font fails to load
			}

			// Create Text for Buttons
			Text scoree("Score " +  to_string(score), font, 24);
			scoree.setFillColor(Color::Black);
			scoree.setPosition(800,590);
			scoree.setStyle(Text::Bold);
			window.draw(scoree);
			
			if ( sprayCans >= 1)
			{
				// Draw spray meter
				RectangleShape sprayMeter(Vector2f(sprayLevel * 10, 20));
				sprayMeter.setFillColor(Color(255, 165, 0));
				sprayMeter.setPosition(player_x, player_y + 68);
				window.draw(sprayMeter);
			}
			
			// bonus cans
			
			if (score >= 20000 && flag_20)
			{
				sprayCans ++;
				flag_20 = false;
			}	
			else if (score >= 40000 && flag_40)
			{
				sprayCans ++;
				flag_40 = false;
			}
			else 
			{
				if ( score  >= 80000 * (spray_count_80 + 1))
				{
					sprayCans ++;
					spray_count_80 ++;	
				}
			}
			
			
			// level transitions
			
			
			drawPlayer(window, player_x, player_y, playerSprite);
			window.display();
			window.clear();
			bool level= level_check(beeActive,totalBees,beeActive_Fast,totalBees_F,levelResult);
			
			if (level)
			{
				i++;
				if (i == 2)
				{
					levelResult++;

					Texture BGtexture;
					if (!BGtexture.loadFromFile("Textures/Cowboy gaming_one.png")) 
					{
						return -1; // Exit on texture load failure
					}

					// Create a sprite for the background
					sf::Sprite BGsprite;
					BGsprite.setTexture(BGtexture);

					// Main game loop
					bool continueExecution = false; // Flag to exit the main loop

					while (window.isOpen()) 
					{
					    sf::Event event;

					    // Process events
					    while (window.pollEvent(event)) 
					    {
						// Check if the Enter key is pressed
						if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) 
						{
						    continueExecution = true; // Set flag to exit the loop
						    break; // Exit the inner event loop
						} 
						else if (event.type == sf::Event::Closed) 
						{
						    window.close(); // Handle window close event
						}
					    }

					    // Check the flag to exit the outer loop
					    if (continueExecution) 
					    {
						break; // Exit the outer loop
					    }

					    // Clear the window
					    window.clear(sf::Color::Black);

					    // Draw the background
					    window.draw(BGsprite);

					    // Display the content on the window
					    window.display();
					}
					break;	
				
				}
				else if ( i == 3)
				{
					levelResult++;

					Texture BGtexture;
					if (!BGtexture.loadFromFile("Textures/Cowboy gaming_two.png")) 
					{
						return -1; // Exit on texture load failure
					}

					// Create a sprite for the background
					sf::Sprite BGsprite;
					BGsprite.setTexture(BGtexture);

					// Main game loop
					bool continueExecution = false; // Flag to exit the main loop

					while (window.isOpen()) 
					{
					    Event event;

					    // Process events
					    while (window.pollEvent(event)) 
					    {
						// Check if the Enter key is pressed
						if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) 
						{
						    continueExecution = true; // Set flag to exit the loop
						    break; // Exit the inner event loop
						} 
						else if (event.type == sf::Event::Closed) 
						{
						    window.close(); // Handle window close event
						}
					    }

					    // Check the flag to exit the outer loop
					    if (continueExecution) 
					    {
						break; // Exit the outer loop
					    }

					    // Clear the window
					    window.clear(sf::Color::Black);

					    // Draw the background
					    window.draw(BGsprite);

					    // Display the content on the window
					    window.display();
					}
					break;	
				
				}
				else if ( i == 4)
				
				{
					
					cout << "asdasd";
					break;
				}
			}
				
		}
	
	
	}
	
	
	bool flagg = appendScoreToFile(playerName, score);
	int check2 = drawHighScores(window, playerName);			// updated score
	
	Texture BGtexture;
	if (!BGtexture.loadFromFile("Textures/Cowboy gaming1.png")) 
	{
		return -1; // Exit on texture load failure
	}

	// Create a sprite for the background
	Sprite BGsprite;
	BGsprite.setTexture(BGtexture);

	// Main game loop
	while (window.isOpen()) {
	Event event;

	// Process events
	while (window.pollEvent(event)) 
	{
		// Check if the Escape key is pressed or the window is closed
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) 
		{
			window.close(); // Close the window
		} 
		else if (event.type == Event::Closed) 
		{
			window.close(); // Handle window close event
		}
	}

	// Clear the window
	window.clear(Color::Black);

	
	window.draw(BGsprite);
	window.display();
	}

   		
	
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) 
{
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) 
{
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet_y -= 20;
	if (bullet_y < -32)
		{bullet_exists = false;}
}
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) 
{
	bulletSprite.setPosition(bullet_x, bullet_y);
	window.draw(bulletSprite);
}



void handleMovement(const Event& e, float& player_x, float& player_y, int boxPixelsX, int boxPixelsY)
{
	if (e.type == Event::KeyPressed) 
	{
		if (e.key.code == Keyboard::Left) 
		{
			if (player_x - boxPixelsX >= 0) 
			{
				player_x -= boxPixelsX;
			}
		} 
		else if (e.key.code == Keyboard::Right) 
		{
			if (player_x + boxPixelsX <= resolutionX - 2 * boxPixelsX) 
			{
				player_x += boxPixelsX;
			}
		}
	}
}


void handleBullet(const Event& e, bool& bullet_exists, float& bullet_x, float& bullet_y, float player_x, float player_y, int& spraysLeft, int& sprayLevel, int& sprayCans,Music &fireMusic)
{
    if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space) 
    {
        if (!bullet_exists && spraysLeft > 0 && sprayCans > 0) 
        {
      	    fireMusic.play();	 	
            bullet_exists = true;
            bullet_x = player_x + (boxPixelsX / 2); // Center bullet with player
            bullet_y = player_y;                  // Set bullet's initial position
            useSpray(spraysLeft, sprayLevel, sprayCans); // Reduce sprays only when bullet is fired
        }
         
        
    }
}



void useSpray(int& spraysLeft, int& sprayLevel, int& sprayCans) 
{
	if (spraysLeft > 0) 
	{
		spraysLeft -= 1;
		sprayLevel = spraysLeft / 8; // Update level every 8 sprays
		
	} 
	if (spraysLeft <= 0 && sprayCans > 0)
        {
		
        	sprayCans--;     		 // Use a new can
		spraysLeft = 56;   		// Refill sprays
		sprayLevel = spraysLeft / 8;    // Reset spray level
    	}
	
    
}


int drawMenu(RenderWindow& window) 
{
	// Create Start Button
	Texture BGtexture;
	BGtexture.loadFromFile("Textures/Cowboy gaming.png"); // Correct loading
	Sprite BGsprite;
	BGsprite.setTexture(BGtexture);
	// Drawing the sprite
	
	RectangleShape startButton(Vector2f(128, 64));
	startButton.setPosition(416, 192);
	startButton.setFillColor(Color:: Black);

	// Create Exit Button
	RectangleShape exitButton(Vector2f(128, 64));
	exitButton.setPosition(416, 272);
	exitButton.setFillColor(Color :: Black);

	// Load font locally for text
	Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) 
	{
		cout << "Error loading font!" << endl;
		return -1; // Return -1 if font fails to load
	}

	// Text labels for buttons
	Text startText("Start", font, 24);
	startText.setFillColor(Color::White);
	startText.setPosition
	(
	startButton.getPosition().x + (startButton.getSize().x - startText.getLocalBounds().width) / 2,
	startButton.getPosition().y + (startButton.getSize().y - startText.getLocalBounds().height) / 2 - 5
	);

	Text exitText("Exit", font, 24);
	exitText.setFillColor(Color::White);
	exitText.setPosition(
	exitButton.getPosition().x + (exitButton.getSize().x - exitText.getLocalBounds().width) / 2,
	exitButton.getPosition().y + (exitButton.getSize().y - exitText.getLocalBounds().height) / 2 - 5
	);

	// Main Menu Loop
	while (window.isOpen()) 
	{
		Event e;
		while (window.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				window.close();
				return -1;
			}

			if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
			{

				if (startButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
				{
					return 1; // Start game
				}
				if (exitButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
				{
					return 0; // Exit game
					
			}
		}
	}

		// Draw Menu
		window.draw(BGsprite); 
		window.draw(startButton);
		window.draw(exitButton);
		window.draw(startText);
		window.draw(exitText);
		window.display();
	}

	return -1; // Default return if window closes unexpectedly
}

int drawLevelMenu(RenderWindow& window) {
	// Create Level Buttons
	RectangleShape level1Button(Vector2f(128, 64));
	level1Button.setPosition(416, 192);
	level1Button.setFillColor(Color(0, 128, 128));

	RectangleShape level2Button(Vector2f(128, 64));
	level2Button.setPosition(416, 272);
	level2Button.setFillColor(Color(255, 255, 0));

	RectangleShape level3Button(Vector2f(128, 64));
	level3Button.setPosition(416, 352);
	level3Button.setFillColor(Color(220, 20, 60));
	
	Texture BGtexture;
	BGtexture.loadFromFile("Textures/Cowboy gaming.png"); // Correct loading
	Sprite BGsprite;
	BGsprite.setTexture(BGtexture);

	// Load font locally for text
	Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) 
	{
		cout << "Error loading font!" << endl;
		return -1; // Return -1 if font fails to load
	}
	
	// Create Text for Buttons
	Text level1Text("Level 1", font, 24);
	level1Text.setFillColor(Color::Black);
	level1Text.setStyle(Text::Bold);
	level1Text.setPosition
	(level1Button.getPosition().x + (level1Button.getSize().x - level1Text.getLocalBounds().width) / 2,
	level1Button.getPosition().y + (level1Button.getSize().y - level1Text.getLocalBounds().height) / 2
	);

	Text level2Text("Level 2", font, 24);
	level2Text.setFillColor(Color::Black);
	level2Text.setStyle(Text::Bold);
	level2Text.setPosition
	(
		level2Button.getPosition().x + (level2Button.getSize().x - level2Text.getLocalBounds().width) / 2,
		level2Button.getPosition().y + (level2Button.getSize().y - level2Text.getLocalBounds().height) / 2
	);

	Text level3Text("Level 3", font, 24);
	level3Text.setFillColor(Color::Black);
	level3Text.setStyle(Text::Bold);
	level3Text.setPosition
	(
		level3Button.getPosition().x + (level3Button.getSize().x - level3Text.getLocalBounds().width) / 2,
		level3Button.getPosition().y + (level3Button.getSize().y - level3Text.getLocalBounds().height) / 2
	);
	
	
	
	
	// Main Loop for Level Menu
	while (window.isOpen()) 
	{
		Event e;
		while (window.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				window.close();
				return -1;
			}

			// Check for mouse clicks on the buttons
			if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
			{
				// Check if the mouse click is within the button bounds using event's position
				if (level1Button.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
				{
					return 1; // Level 1 selected
				}
				if (level2Button.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
				{
					return 2; // Level 2 selected
				}
				if (level3Button.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
				{
					return 3; // Level 3 selected
				}
		 	}
		}
		
		
		
		// Draw the buttons and text
		window.clear();
		window.draw(BGsprite);
		window.draw(level1Button);
		window.draw(level2Button);
		window.draw(level3Button);
		window.draw(level1Text);
		window.draw(level2Text);
		window.draw(level3Text);
		window.display();
	}

	return -1; // Default return if window closes unexpectedly
}

// Initialize bee settings
void initializeBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees) 
{
	for (int i = 0; i < totalBees; i++) 
	{
		
		beeX[i] = rand() % 960;		
		beeY[i] = 0;
		beeActive[i] = false;
		beeDirection[i] = 1; // Start moving right
	}
	beeActive[0] = true;
}

void initializeBees_Fast(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees) {
	for (int i = 0; i < totalBees; i++) 
	{
		
		beeX[i] = rand() % 960;		
		beeY[i] = 0;
		beeActive[i] = false;
		beeDirection[i] = 1; // Start moving right
	}
	beeActive[0] = true;
}


void spawnBees(float beeX[], float beeY[], bool beeActive[], int totalBees, int &currentBee, float interval, Clock &clock) 
{
	if (currentBee < totalBees && clock.getElapsedTime().asSeconds() >= interval) 
	{
		currentBee++; // Activate the next bee
		beeActive[currentBee] = true;
		clock.restart(); // Reset the timer
	}
}

void spawnBees_Fast(float beeX[], float beeY[], bool beeActive[], int totalBees, int &currentBee, float interval, Clock &clock) 
{
	if (currentBee < totalBees && clock.getElapsedTime().asSeconds() >= interval) 
	{
		currentBee++; // Activate the next bee
		beeActive[currentBee] = true;
		clock.restart(); // Reset the timer
	}
}



// Draw bees
void drawBees(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], bool beeActive[], int totalBees) 
{
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i]) 
		{
			beeSprite.setPosition(beeX[i], beeY[i]);
			window.draw(beeSprite);
		}
	}
}

void drawBees_Fast(RenderWindow& window, Sprite& beeSprite, float beeX[], float beeY[], bool beeActive[], int totalBees) 
{
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i]) 
		{
			beeSprite.setPosition(beeX[i], beeY[i]);
			window.draw(beeSprite);
		}
	}
}

 // Check bullet collision with bees
bool checkBulletCollision(float bulletX, float bulletY, bool& bulletExists, float beeX[], float beeY[], bool beeActive[], int totalBees, 
                          float honeycombX[], float honeycombY[], bool honeycombActive[], int &score, Music &collision) 
{
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i] &&
		bulletX + 10 >= beeX[i] && bulletX <= beeX[i] + boxPixelsX && // Check X overlap
		bulletY + 10 >= beeY[i] && bulletY <= beeY[i] + boxPixelsY)  // Check Y overlap
		{
			beeActive[i] = false;            // Deactivate bird
			bulletExists = false;           // Remove bullet
			score += 100;
			collision.play();
			// Activate a honeycomb at the bird's position
			honeycombX[i] = beeX[i];
			honeycombY[i] = beeY[i];
			honeycombActive[i] = true;
			return true;	
			break;
		}
	}
	return false;		
}
bool checkBulletCollision_F(float bulletX, float bulletY, bool& bulletExists, float beeX[], float beeY[], bool beeActive[], int totalBees, 
                          float honeycombX[], float honeycombY[], bool honeycombActive[], int &score, Music &collision)
{
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i] &&
		bulletX + 10 >= beeX[i] && bulletX <= beeX[i] + boxPixelsX && // Check X overlap
		bulletY + 10 >= beeY[i] && bulletY <= beeY[i] + boxPixelsY)  // Check Y overlap
		{
			beeActive[i] = false;            // Deactivate bird
			bulletExists = false;           // Remove bullet
			score += 1000;
			collision.play();
			// Activate a honeycomb at the bird's position
			honeycombX[i] = beeX[i];
			honeycombY[i] = beeY[i];
			honeycombActive[i] = true;
			return true;	
			break;
		}
	}
	return false; 	                         
}                          
void drawHoneycombs(RenderWindow &window, Sprite &honeySprite, float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs,
                    float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs) 
{
	// Draw dynamically generated honeycombs

	for (int i = 0; i < maxHoneycombs; i++) 
	{

		if (honeycombActive[i]) 
		{

			honeySprite.setPosition(honeycombX[i], honeycombY[i]);
			window.draw(honeySprite);
		}
	}

	// Draw pre-generated honeycombs
	for (int i = 0; i < preGeneratedHoneycombs; i++) 
	{
		if (preHoneycombActive[i]) 
		{
			honeySprite.setPosition(preHoneycombX[i], preHoneycombY[i]);
			window.draw(honeySprite);
		}
	}
}

void drawHoneycombs_F(RenderWindow &window, Sprite &honeySprite, float honeycombX[], float honeycombY[], bool honeycombActive[], int maxHoneycombs) 
{
	// Draw dynamically generated honeycombs

	for (int i = 0; i < maxHoneycombs; i++) 
	{

		if (honeycombActive[i]) 
		{

			honeySprite.setPosition(honeycombX[i], honeycombY[i]);
			window.draw(honeySprite);
		}
	}

}



void updateBees(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees, float screenWidth, 
                float beeSpeed, float rowHeight, float honeycombX[], float honeycombY[], bool honeycombActive[], 
                int maxHoneycombs, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs)
{
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i]) 
		{
			beeX[i] += beeDirection[i] * beeSpeed;

			// Check if the bee collides with any active dynamically generated honeycomb
			for (int j = 0; j < maxHoneycombs; j++) 
			{
				if (honeycombActive[j] && // Honeycomb is active
				beeX[i] + boxPixelsX >= honeycombX[j] && beeX[i] <= honeycombX[j] + boxPixelsX && // Check X overlap
				beeY[i] + boxPixelsY >= honeycombY[j] && beeY[i] <= honeycombY[j] + boxPixelsY) 
				{ // Check Y overlap
					// Reverse bee's direction and move down
					beeDirection[i] *= -1;
					beeY[i] += rowHeight; // Move down
					break; // Exit once the first collision is detected
				}
			}

			// Check if the bee collides with any pre-generated honeycomb
			for (int j = 0; j < preGeneratedHoneycombs; j++) 
			{
				if (preHoneycombActive[j] && // Pre-generated honeycomb is active
				beeX[i] + boxPixelsX >= preHoneycombX[j] && beeX[i] <= preHoneycombX[j] + boxPixelsX && // Check X overlap
				beeY[i] + boxPixelsY >= preHoneycombY[j] && beeY[i] <= preHoneycombY[j] + boxPixelsY) 
				{ // Check Y overlap
					// Reverse bee's direction and move down
					beeDirection[i] *= -1;
					beeY[i] += rowHeight; // Move down
					break; // Exit once the first collision is detected
				}
			}

			// Check for screen boundaries
			if (beeX[i] <= 0 || beeX[i] + boxPixelsX >= screenWidth) 
			{
				beeDirection[i] *= -1; // Reverse direction
				beeY[i] += rowHeight; // Move down
				beeX[i] = std::max(0.0f, std::min(beeX[i], screenWidth - boxPixelsX)); // Keep in bounds
			}
			 if (beeY[i] >= 640) 
			{
				beeActive[i] = false; // Deactivate the bee
			
			}
		}
	}
}

void updateBees_F(float beeX[], float beeY[], bool beeActive[], int beeDirection[], int totalBees, float screenWidth, 
                float beeSpeed, float rowHeight)
{


	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i]) 
		{
			beeX[i] += beeDirection[i] * beeSpeed;			

			// Check for screen boundaries
			if (beeX[i] <= 0 || beeX[i] + boxPixelsX >= screenWidth) 
			{
				beeDirection[i] *= -1; // Reverse direction
				beeY[i] += rowHeight; // Move down
				beeX[i] = std::max(0.0f, min(beeX[i], screenWidth - boxPixelsX)); // Keep in bounds
			}
			 if (beeY[i] >= 640) 
			{
				beeActive[i] = false; // Deactivate the bee
			
			}
		}
	}
}                
bool checkHoneycombCollision(float bulletX, float bulletY, bool& bulletExists,float honeycombX[], float honeycombY[], bool honeycombActive[], 
                              int maxHoneycombs, int &score, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs,Music &collision) 
{
	// Check collision for dynamically generated honeycombs
	for (int i = 0; i < maxHoneycombs; i++) 
	{
		if (honeycombActive[i] && 
		bulletX + 10 >= honeycombX[i] && bulletX <= honeycombX[i] + boxPixelsX && // Check X overlap
		bulletY + 10 >= honeycombY[i] && bulletY <= honeycombY[i] + boxPixelsY) 
		{ // Check Y overlap

			// Deactivate the honeycomb
			honeycombActive[i] = false;
			bulletExists = false;  // Remove bullet
			collision.play();
			// Calculate score based on honeycomb's position
			int tierScore = 0;

			// Determine the score based on Y position (tiers)
			if (honeycombY[i] <= 64) 
			{
				tierScore = 1000; // Top two screen tiers
			}
			else if (honeycombY[i] <= 160) 
			{
				tierScore = 800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier
			return true;
			break;  // Exit once the first collision is detected
			
		}
	}

	// Check collision for pre-generated honeycombs
	for (int i = 0; i < preGeneratedHoneycombs; i++) 
	{
		if (preHoneycombActive[i] && 
		bulletX + 10 >= preHoneycombX[i] && bulletX <= preHoneycombX[i] + boxPixelsX && // Check X overlap
		bulletY + 10 >= preHoneycombY[i] && bulletY <= preHoneycombY[i] + boxPixelsY) 
		{ // Check Y overlap

			// Deactivate the honeycomb
			preHoneycombActive[i] = false;
			bulletExists = false;  // Remove bullet
			collision.play();
			// Calculate score based on honeycomb's position
			int tierScore = 0;

			// Determine the score based on Y position (tiers)
			if (preHoneycombY[i] <= 64) 
			{
				tierScore = 1000; // Top two screen tiers
			}
			else if (preHoneycombY[i] <= 160) 
			{
				tierScore = 800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier
			return true;
			break;  // Exit once the first collision is detected
		}
	}
	return false;
}

bool checkHoneycombCollision_F(float bulletX, float bulletY, bool& bulletExists,float honeycombX[], float honeycombY[], bool honeycombActive[], 
                              int maxHoneycombs, int &score,Music &collision) 
{
	// Check collision for dynamically generated honeycombs
	for (int i = 0; i < maxHoneycombs; i++) 
	{
		if (honeycombActive[i] && 
		bulletX + 10 >= honeycombX[i] && bulletX <= honeycombX[i] + boxPixelsX && // Check X overlap
		bulletY + 10 >= honeycombY[i] && bulletY <= honeycombY[i] + boxPixelsY) 
		{ // Check Y overlap

			// Deactivate the honeycomb
			honeycombActive[i] = false;
			bulletExists = false;  // Remove bullet
			collision.play();
			// Calculate score based on honeycomb's position
			int tierScore = 0;

			// Determine the score based on Y position (tiers)
			if (honeycombY[i] <= 64) 
			{
				tierScore = 2000; // Top two screen tiers
			}
			else if (honeycombY[i] <= 160) 
			{
				tierScore = 1800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 1500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier
			break;  // Exit once the first collision is detected
			return true;
		}
	}
	return false;	
}


void spawnHummingbird(RenderWindow& window, Sprite& HummingSprite, Texture& HummingTexture, 
                      float& hummingbirdX, float& hummingbirdY, float screenWidth, float screenHeight, 
                      float& speedX, float& speedY, bool& hummingActive, int &toggle, bool color_condition) 
{
    // Ensure the hummingbird is spawned only if it's inactive
	if (!hummingActive) 
	{
		// Randomly spawn the hummingbird at a position within the screen bounds
		hummingbirdX = rand() % 900;// Assuming bird width is 32 pixels
		hummingbirdY = rand() % 400; // Assuming bird height is 32 pixels

		// Random speed and direction assignment
		speedX = (rand() % 3 + 1)/ 10.0f; // Random speed between 1 and 3
		speedY = (rand() % 3 + 1) / 10.0f;

		hummingActive = true;
	}
	if ( toggle == 1)       
	{
		HummingSprite.setTextureRect(IntRect(0, 0, 32, 32)); // First bird image (left side of texture)
		toggle = 0;
	}
	else 
	{
		HummingSprite.setTextureRect(IntRect(32, 0, 32, 32)); // First bird image (left side of texture)
		toggle = 1;
	} 
	// Set the texture and position of the sprite
	HummingSprite.setTexture(HummingTexture);
	HummingSprite.setPosition(hummingbirdX, hummingbirdY);
	if (color_condition)
		HummingSprite.setColor(Color::Blue);
	// Draw the sprite on the window
	else 
		HummingSprite.setColor(Color::Green);


	window.draw(HummingSprite);
   
  
}




void updateHummingbird(float& hummingbirdX, float& hummingbirdY, 
                       float& speedX, float& speedY, 
                       bool& hummingActive, int& hummingbirdX_direction, int& hummingbirdY_direction, 
                       float resolutionX, float resolutionY, float boxPixelsX, float boxPixelsY,
                       float honeycombX[], float honeycombY[], int totalHoneycombs, Clock& clock, bool honeycombActive[], int& score,
                       float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int preGeneratedHoneycombs, float honeycombX_F[], float honeycombY_F[], bool honeycombActive_F[], int maxHoneycombs_F, int choice)
{



	if (!hummingActive) 
	{
		return; 
	}


	static float stopTime = 0.0f;      // Timer to track when to resume movement
	static const float stopDuration = 3.0f;  // Duration the bird should stop (3 seconds)

    // Check if stopTime is set and the timer is still running
	if (stopTime > 0) 
	{
		stopTime -= clock.restart().asSeconds(); // Decrease the stopTime by the elapsed time
		if (stopTime <= 0) 
		{
			stopTime = 0;  // Reset stopTime after the pause ends
		}
		return; // Skip movement until stopTime is over
	}

  	  // Check for collision with regular honeycombs
	for (int i = 0; i < totalHoneycombs; ++i) 
	{
		// If the honeycomb is active and collision occurs
		if (honeycombActive[i] && 
		hummingbirdX < honeycombX[i] + boxPixelsX && 
		hummingbirdX + boxPixelsX > honeycombX[i] &&
		hummingbirdY < honeycombY[i] + boxPixelsY && 
		hummingbirdY + boxPixelsY > honeycombY[i]) 
		{

			// Set stopTime to 3 seconds (pause for 3 seconds)
			stopTime = stopDuration;

			// Reverse direction after collision
			hummingbirdX_direction *= -1; 
			hummingbirdY_direction *= -1;

			// Calculate score based on honeycomb's Y position (tier scoring)
			int tierScore = 0;
			if (honeycombY[i] <= 64) 
			{
				tierScore = 1000; // Top two screen tiers
			}
			else if (honeycombY[i] <= 160) 
			{
				tierScore = 800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier

			// Deactivate the honeycomb
			honeycombActive[i] = false;
			break;  // Stop checking further honeycombs if collision is detected
		}
	}

    // Check for collision with pre-generated honeycombs
	for (int i = 0; i < preGeneratedHoneycombs; ++i)
	{
		// If the pre-generated honeycomb is active and collision occurs
		if (preHoneycombActive[i] && 
		hummingbirdX < preHoneycombX[i] + boxPixelsX && 
		hummingbirdX + boxPixelsX > preHoneycombX[i] &&
		hummingbirdY < preHoneycombY[i] + boxPixelsY && 
		hummingbirdY + boxPixelsY > preHoneycombY[i]) 
		{

			// Set stopTime to 3 seconds (pause for 3 seconds)
			stopTime = stopDuration;

			// Reverse direction after collision
			hummingbirdX_direction *= -1;
			hummingbirdY_direction *= -1;

			// Calculate score based on pre-generated honeycomb's Y position (tier scoring)
			int tierScore = 0;
			if (preHoneycombY[i] <= 64)
			{
				tierScore = 1000; // Top two screen tiers
			}
			else if (preHoneycombY[i] <= 160) \
			{
				tierScore = 800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier

			// Deactivate the pre-generated honeycomb
			preHoneycombActive[i] = false;
			break;  // Stop checking further pre-generated honeycombs if collision is detected
		}
	}
	if (choice> 1)
	{
		for (int i = 0; i < maxHoneycombs_F; ++i) 
		{
			if (honeycombActive_F[i] && 
			hummingbirdX < honeycombX_F[i] + boxPixelsX && 
			hummingbirdX + boxPixelsX > honeycombX_F[i] &&
			hummingbirdY < honeycombY_F[i] + boxPixelsY && 
			hummingbirdY + boxPixelsY > honeycombY_F[i]) 
			{

			// Set stopTime to 3 seconds (pause for 3 seconds)
			stopTime = stopDuration;

			// Reverse direction after collision
			hummingbirdX_direction *= -1; 
			hummingbirdY_direction *= -1;

			// Calculate score based on honeycomb's Y position (tier scoring)
			int tierScore = 0;
			if (honeycombY_F[i] <= 64) 
			{
				tierScore = 2000; // Top two screen tiers
			}
			else if (honeycombY[i] <= 160) 
			{
				tierScore = 1800;  // Tiers 3, 4, and 5
			}
			else 
			{
				tierScore = 1500;  // Rest of the honeycombs
			}

			score += tierScore;  // Add score based on tier
			// Deactivate the honeycomb
			honeycombActive_F[i] = false;
			break;  // Stop checking further honeycombs if collision is detected
			}
		}	
	}
	
	// Ensure valid speed and direction
	if (speedX == 0) speedX = 1;
	if (hummingbirdX_direction == 0) hummingbirdX_direction = 1;

	// Update position based on speed and direction
	hummingbirdX += speedX * hummingbirdX_direction;
	hummingbirdY += speedY * hummingbirdY_direction;

	// Check for X boundaries and reverse direction if needed
	if (hummingbirdX <= 0 || hummingbirdX + boxPixelsX >= resolutionX) 
	{
		hummingbirdX_direction *= -1; // Reverse direction
		hummingbirdX = std::max(0.0f, std::min(hummingbirdX, resolutionX - boxPixelsX)); // Clamp position
	}

	// Check for Y boundaries and reverse direction if needed
	if (hummingbirdY <= 0 || hummingbirdY + boxPixelsY >= (resolutionY - 64)) 
	{
		hummingbirdY_direction *= -1; // Reverse direction
		hummingbirdY = std::max(0.0f, std::min(hummingbirdY, resolutionY - boxPixelsY)); // Clamp position
	}
}


void pollination(RenderWindow& window, float beeX[], float beeY[], bool beeActive[], int totalBees, bool obstaclePlaced[])
{
	// Load the obstacle texture
	Texture obstaclestexture;
	if (!obstaclestexture.loadFromFile("Textures/obstacles.png")) 
	{
		cerr << "Error: Could not load obstacle texture!" << std::endl;
		return;
	}

	// Create a sprite for the obstacle
	Sprite obstaclesSprite;
	obstaclesSprite.setTexture(obstaclestexture);
	obstaclesSprite.setScale(2, 2); // Set scale once, outside the loop

	// Process each bee
	for (int i = 0; i < totalBees; i++)
	{
		if (beeY[i] > 520 && !obstaclePlaced[i]) // If the bee crosses the Y threshold and no obstacle yet
		{

			obstaclesSprite.setPosition(i * 32 +10, 520); // Fixed Y-coordinate (last row)
			window.draw(obstaclesSprite);
			obstaclePlaced[i] = true;
			beeActive[i] = false;
		}
	}
}


void  gameOver(RenderWindow& window)
{
	Texture obstaclestexture;
	obstaclestexture.loadFromFile("Textures/Cowboy gaming1.png"); // Correct loading
	Sprite obstaclesSprite;
	obstaclesSprite.setTexture(obstaclestexture);
	window.clear();
	window.draw(obstaclesSprite);

}

int drawNameInputMenu(RenderWindow& window, string& playerName) 
{
	// Load background texture
	Texture BGtexture;
	if (!BGtexture.loadFromFile("Textures/Cowboy gaming.png")) 
	{
		cout << "Error loading background texture!" << std::endl;
		return -1;
	}
	Sprite BGsprite;
	BGsprite.setTexture(BGtexture);

	// Create text input box
	RectangleShape inputBox(Vector2f(300, 50));
	inputBox.setPosition(312, 200);
	inputBox.setFillColor(Color::White);
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(Color::Black);

	// Create Submit Button
	RectangleShape submitButton(Vector2f(128, 64));
	submitButton.setPosition(336, 300);
	submitButton.setFillColor(Color::Black);

	// Load font
	Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) 
	{
		cout << "Error loading font!" << std::endl;
		return -1;
	}

	// Create Submit button text
	Text submitText("Submit", font, 24);
	submitText.setFillColor(Color::White);
	submitText.setPosition(
	submitButton.getPosition().x + (submitButton.getSize().x - submitText.getLocalBounds().width) / 2,
	submitButton.getPosition().y + (submitButton.getSize().y - submitText.getLocalBounds().height) / 2 - 5
	);

	// Display entered text
	Text inputText("", font, 24);
	inputText.setFillColor(Color::Black);
	inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 10);

	string inputString = ""; // Store user input

	// Main input loop
	while (window.isOpen()) 
	{
	Event e;
	while (window.pollEvent(e)) 
	{
		if (e.type == Event::Closed) 
		{
			window.close();
			return -1;
		}

		// Handle text 
		if (e.type == Event::TextEntered) 
		{
			if (e.text.unicode == 8 && !inputString.empty()) // Backspace
			{
				inputString.pop_back();
			} 
			else if (e.text.unicode >= 32 && e.text.unicode <= 126) // Printable characters
			{
				inputString += static_cast<char>(e.text.unicode);
			}
			inputText.setString(inputString);
		}

		// Handle button click
		if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) 
		{
			if (submitButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) 
			{
				playerName = inputString; // Save entered name
				return 1; // Proceed to next screen
			}
		}
	}

	// Draw elements
	window.clear();
	window.draw(BGsprite);
	window.draw(inputBox);
	window.draw(submitButton);
	window.draw(submitText);
	window.draw(inputText);
	window.display();
	}
	return -1; // Default return if window closes unexpectedly
}


int drawHighScores(RenderWindow& window, string& playerName) 
{
    // Load background texture
	Texture BGtexture;
	if (!BGtexture.loadFromFile("Textures/Cowboy gaming.png")) 
	{
		return -1; // Exit
	}
	Sprite BGsprite;
	BGsprite.setTexture(BGtexture);

	// Load font
	Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) 
	{
		return -1; // Exit 
	}

	// Title and Player Name
	Text titleText("High Scores", font, 32);
	titleText.setFillColor(Color::Black);
	titleText.setStyle(Text::Bold);
	titleText.setPosition(364, 200);

	Text playerText("Your Name: " + playerName, font, 24);
	playerText.setFillColor(Color::Black);
	playerText.setStyle(Text::Bold);
	playerText.setPosition(370, 264);

	// Read scores from file
	ifstream file("scores.txt");
	if (!file.is_open()) 
	{
		return -1; // Exit if file cannot be opened
	}

	const int MAX_SCORES = 10;
	string names[MAX_SCORES];
	int scores[MAX_SCORES] = {0};
	int count = 0;

	string name;
	int score;

	while (file >> name >> score) 
	{
		if (count < MAX_SCORES || score > scores[MAX_SCORES - 1]) 
		{
			int pos = count < MAX_SCORES ? count : MAX_SCORES - 1;
			while (pos > 0 && scores[pos - 1] < score) 
			{
				if (pos < MAX_SCORES) 
					{
					names[pos] = names[pos - 1];
					scores[pos] = scores[pos - 1];
				}
				pos--;
			}
			names[pos] = name;
			scores[pos] = score;
			if (count < MAX_SCORES) count++;
		}
	}
	file.close();

	// Prepare score texts
	Text scoreTexts[MAX_SCORES];
	for (int i = 0; i < count; ++i) 
	{
		scoreTexts[i].setFont(font);
		scoreTexts[i].setString(names[i] + ": " + std::to_string(scores[i]));
		scoreTexts[i].setCharacterSize(20);
		scoreTexts[i].setFillColor(Color::Black);
		scoreTexts[i].setPosition(370, 296 + i * 30);
	}

	while (window.isOpen()) 
	{
		Event e;
		while (window.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				window.close();
				return -1;
			}
			if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter) 
			{
				window.clear();
				return 1; // Exit 
			}
		}

		// Draw elements
		window.clear();
		window.draw(BGsprite);
		window.draw(titleText);
		window.draw(playerText);
		for (int i = 0; i < count; ++i) 
		{
			window.draw(scoreTexts[i]);
		}
		window.display();
	}

	return 0;
}

bool appendScoreToFile(const std::string& playerName, int score) 
{
	//  append mode
	ofstream file("scores.txt", std::ios::app);
	if (!file.is_open()) 
	{
		return false;  // Return false if the file cannot be opened
	}

	// Write the player's name and score to the file
	file << playerName << " " << score << std::endl;

	file.close();

	return true;  
}



bool level_check(bool beeActive[], int totalBees, bool beeActive_Fast[], int totalBees_F, int levelResult) {
	// Check if all normal bees are inactive
	for (int i = 0; i < totalBees; i++) 
	{
		if (beeActive[i]) 
		{
			return false; // Level is not complete
		}
	}
	if (levelResult != 1)
	{
		// Check if all fast bees are inactive
		for (int i = 0; i < totalBees_F; i++)
		{
			if (beeActive_Fast[i]) 
			{
				return false; // Level is not complete
			}
		}
	}
	// If all bees are inactive, the level is complete
	return true;
}




