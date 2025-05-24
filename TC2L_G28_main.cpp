// *********************************************************
// Program: TC2L_G28_main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC2L
// Tutorial Class: TT7L
// Trimester: 2510
// Member 1: 243UC24672 | Lai Seng Kung  | lai.seng.kung1@student.mmu.edu.my | 012-9169228
// Member 2: 242UC244GR | Lee Xiu Wei    | LEE.XIU.WEI@student.mmu.edu.my    | 017-9218088
// Member 3: 242UC244S3 | Teng Ming Hein | TENG.MING.HEIN@student.mmu.edu.my | 016-7831558
// *********************************************************
// Task Distribution
// Member_1:
// Member_2:
// Member_3:
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;

class Battlefield;

// Base Abstract Class
class Robot
{
protected:
    string robotId;
    string robotName;
    string robotType;

    int robotPositionX;
    int robotPositionY;

    int numberOfLives = 3; // Initial value
    int numberOfKills = 0; // Initial value
    int queueEntryTurn = -1; // All the robots not in the queue at the beginning

    bool isDestroyed = false;

    bool upgradedMoving = 0;
    bool upgradedShooting = 0;
    bool upgradedSeeing = 0;

    int numUpgrade = 0;
public:
    // Parameterized Constructor(PC)
    Robot(string id, int posX, int posY) : robotId(id), robotPositionX(posX), robotPositionY(posY) {}

    // Destructor (virtual for polymorphism)
    virtual ~Robot();

    // Getter and Setter for RobotId
    string getId() const { return robotId; }
    void setId(string id) { robotId = id; }

    // Getter and Setter for RobotName
    string getName() const { return robotName; }
    void setName(string name) { robotName = name; }

    // Getter and Setter for RobotType
    string getType() const { return robotType; }
    void setType(string type) { robotType = type; }

    // Getter and Setter for RobotPositionX
    int getPosX() const { return robotPositionX; }
    void setPosX(int posX) { robotPositionX = posX; }

    // Getter and Setter for RobotPositionY
    int getPosY() const { return robotPositionY; }
    void setPosY(int posY) { robotPositionY = posY; }

    // Getter and Setter for numberOfLives
    int getLives() const { return numberOfLives; }
    void setLives(int lives) { numberOfLives = lives; }

    // Getter and Setter for numberOfKills
    int getKills() const { return numberOfKills; }
    void setKills(int kills) { numberOfKills = kills; }

    // Getter and Setter for queueEntryTurn
    int getEntryTurn() const { return queueEntryTurn; }
    void setEntryTurn(int entry) { queueEntryTurn = entry; }

    //Getter and Setter for isDestroyed
    bool getIsDestroyed() const {return isDestroyed;}
    void setIsDestroyed( bool destroyed) {isDestroyed = destroyed;}

    //Getter and Setter for upgradedMoving
    bool getUpgradedMoving () const {return upgradedMoving;}
    void setUpgradedMoving (bool moving) {upgradedMoving=moving;}

    //Getter and Setter for upgradedShooting
    bool getUpgradedShooting () const {return upgradedShooting;}
    void setUpgradedShooting (bool shooting) {upgradedShooting=shooting;}

    //Getter and Setter for upgradedSeeing
    bool getUpgradedSeeing () const {return upgradedSeeing;}
    void setUpgradedSeeing (bool seeing) {upgradedSeeing=seeing;}

    //Getter and Setter for numUpgrade
    int getNumUpgrade() const {return numUpgrade;}
    void setNumUpgrade (int numUp) {numUpgrade=numUp;}

    // Reduce life when getting shoot or selfDestruct
    void reduceLife() {setLives(getLives() - 1);}


    // selfDestruct when ran out of shells(ammo)
    void selfDestruct() {
        reduceLife();
    }

    // Increase kills
    void incrementKills() {numberOfKills++;}

    // Check if the robot still have lives
    bool isAlive() const {return numberOfLives > 0;}

    // Overloading the << operator for Robot class
    friend ostream& operator<<(ostream &COUT, const Robot& r)
    {
        COUT << r.robotId << " at (" << r.robotPositionX << ", " << r.robotPositionY << ") actions:" << endl;
        return COUT;
    }

    // Pure Virtual Functions
    virtual void setRobotLocation(int posX, int posY) = 0;
    virtual void actions(Battlefield* battlefield) = 0;
};
inline Robot::~Robot() {}

class ThinkingRobot: virtual public Robot
{
protected :
    // data member
public:
    virtual ~ThinkingRobot(){}

    // Pure virtual function for thinking
    virtual void actionThink (Battlefield* battlefield) = 0;
};

class SeeingRobot: virtual public Robot
{
protected:
    //data member
public:
    ~SeeingRobot(){}

    // Pure virtual function for looking
    virtual void actionLook(Battlefield* battlefield) = 0;
};

class ShootingRobot: virtual public Robot
{
protected:
    //data member
public:
    ~ShootingRobot(){}

    // Pure virtual function for shooting
    virtual void actionFire(Battlefield* battlefield) = 0;
};

class MovingRobot: virtual public Robot
{
protected:
    //data member
public:
    ~MovingRobot(){}

    // Pure virtual function for moving
    virtual void actionMove(Battlefield* battlefield) = 0;
};

// Pure virtual

class GenericRobot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:
    static int robotAutoIncrementInt_; // Static member for auto-incrementing ID

    int shellsRemaining = 10;

    //Detects directions the robot can move
    bool canMove[9] = {false};

    //Detects the presence of other robots
    bool hasEnemy[8] = {false};

    // direction arrays like Up, Up-Right, Right, Down-Right, Down, Down-Left, Left, Up-Left, Stand in place
    const int dx[9] = { 0, 1, 1, 1, 0,-1,-1,-1, 0};
    const int dy[9] = {-1,-1, 0, 1, 1, 1, 0,-1, 0};

public:
    GenericRobot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;

        robotAutoIncrementInt_++;
    }

    static int robotAutoIncrementInt()
    {
        return robotAutoIncrementInt_;
    }

    virtual ~GenericRobot() {}

    virtual void setRobotLocation(int x, int y)
    {
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual void actionThink (Battlefield* battlefield);
    virtual void actionLook (Battlefield* battlefield);
    virtual void actionFire (Battlefield* battlefield);
    virtual void actionMove (Battlefield* battlefield);

    virtual void actions (Battlefield* battlefield)
    {
        int randomInt = 0;

        if( randomInt % 2 == 0)
        {
            cout << endl;
            actionThink (battlefield);
            cout << endl;
            actionLook (battlefield);
            cout << endl;
            actionFire (battlefield);
            cout << endl;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {
            cout << endl;
            actionThink (battlefield);
            cout << endl;
            actionLook (battlefield);
            cout << endl;
            actionMove(battlefield);
            cout << endl;
            actionFire (battlefield);
        }
    }
};
int GenericRobot::robotAutoIncrementInt_ = 0;

class Battlefield
{
private:
    int BATTLEFIELD_NUM_OF_COLS = -1;
    int BATTLEFIELD_NUM_OF_ROWS = -1;

    int totalTurns = -1;         // Total number of turns
    int currentTurn = 0;           // Current turn number

    int numOfRobots = -1; // Number of robots
    int currentRobotIndex = 0;

    vector<Robot*> robots;
    queue<Robot*> destroyedRobots;
    queue<Robot*> waitingRobots;

    vector<vector<string>> battlefield; // 2D vector representing the battlefield
public:
    // Getter functions
    int getBATTLEFIELD_NUM_OF_COLS()
    {
        return BATTLEFIELD_NUM_OF_COLS;
    }
    int getBATTLEFIELD_NUM_OF_ROWS ()
    {
        return BATTLEFIELD_NUM_OF_ROWS;
    }
    int getTotalTurns()
    {
        return totalTurns;
    }
    int getCurrentTurn()
    {
        return currentTurn;
    }
    int getNumOfRobots ()
    {
        return numOfRobots;
    }
    vector<Robot*>& getRobots() {
        return robots;
    }

    // Setter Function
    void setCurrentTurn(int turn)
    {
        currentTurn = turn;
    }

    // Read input file to initialize battlefield and robots
    void readInputFile(string fileInputName)
    {
        ifstream fileInput;
        fileInput.open(fileInputName);

        // Check the file is able to open
        if (!fileInput.is_open())
        {
            cerr << "Unable to open file" << endl;
            exit(-1);
        }

        string line;
        while (getline(fileInput, line))
        {
            if (line.find("M by N:") != string::npos)
            {
                // Read the dimension line (M by N)
                istringstream dimensionStream(line);
                string ignore;
                dimensionStream >> ignore >> ignore >> ignore >> BATTLEFIELD_NUM_OF_COLS >> BATTLEFIELD_NUM_OF_ROWS;
                battlefield.resize(BATTLEFIELD_NUM_OF_ROWS, vector<string>(BATTLEFIELD_NUM_OF_COLS));
            }
            else if (line.find("turns:") != string::npos)
            {
                // Read the turns line
                istringstream turnStream(line);
                string ignore;
                turnStream >> ignore >> totalTurns;
                //cout << totalTurns << endl;
            }
            else if (line.find("robots:") != string::npos)
            {
                // Read the robots line
                istringstream robotStream(line);
                string ignore;
                robotStream >> ignore >> numOfRobots;
                //cout << numOfRobots << endl;
            }
            else if (line.empty())
                // If line is empty, skip the line
                continue;
            else
            {
                // Read the robot details line
                istringstream robotInfoStream(line);
                string type, idName, posX, posY;
                int x, y;

                robotInfoStream >> type >> idName >> posX >> posY;

                // Get the id of the robot
                string id = idName.substr(0, idName.find("_"));
                // Get the name of the robot
                string name = idName.substr(idName.find("_") + 1);
                //cout << id << " and " << name << endl;

                if (posX == "random")
                    x = rand() % BATTLEFIELD_NUM_OF_COLS; // Randomize the column position
                else
                    x = stoi(posX); // Convert string to int

                if (posY == "random")
                    y = rand() % BATTLEFIELD_NUM_OF_ROWS; // Randomize the row position
                else
                    y = stoi(posY); // Convert string to int

                // Create a new robot in advance after get the details of the robot
                Robot* robotGenericRobot = new GenericRobot(id, x ,y);
                robotGenericRobot->setName(name);
                robotGenericRobot->setType(type);
                robots.push_back(robotGenericRobot); // Update the robots's vector
            }
        }
        //cout << robots.size() << endl;
        fileInput.close();
    }

    // Write to output file as log file
    /*void writeOutputFile(const std::string& fileOutputName)
    {
        ofstream fileOutput;
        fileOutput.open(fileOutputName);

        // Check the file is able to open
        if (!fileOutput.is_open())
        {
            cerr << "Unable to open file" << endl;
            exit(-1);
        }

        oldCoutBuf_ = std::cout.rdbuf();         // Save original cout buffer
        std::cout.rdbuf(outFile_.rdbuf());       // Redirect cout to file
    }*/

    // Place robots on the battlefield
    void placeRobots()
    {
        // Set all the position with empty
        for (int i = 0; i < battlefield.size(); ++i)
        {
            for (int j = 0; j < battlefield[i].size(); ++j)
                battlefield[i][j] = "";
        }

        // To place the robot position
        for (int i = 0; i < robots.size(); ++i)
        {
            //check if the robot is still alive
            if (robots[i]->getIsDestroyed() == false )
            {
                // check position
                if (robots[i]->getPosY() < battlefield.size() && robots[i]->getPosX() < battlefield[0].size())
                {
                    battlefield[robots[i]->getPosY()][robots[i]->getPosX()] = robots[i]->getId();
                }
                else
                {
                    cerr << "Error message: Invalid location for the robot" << robots[i]->getId() << endl;
                    exit(1);
                }
            }
        }
    }

    // Display the battlefield in the screen
    void displayBattleField() const
    {
        cout << "Display Battlefield" << endl;
        cout << "    ";

        // Print the column header
        for (int j = 0; j < battlefield[0].size(); ++j)
        {
            cout << "  " << right << setfill('0') << setw(2) << j << " ";
        }
        cout << endl;

        // Print the battle field's row
        for (int i = 0; i < battlefield.size(); ++i)
        {
            cout << "    ";
            for (int j = 0; j < battlefield[i].size(); ++j)
            {
                cout << "+----";
            }
            cout << "+" << endl; // Close the last gap

            // Print the row header
            cout << "  " << right << setfill('0') << setw(2) << i;
            for (int j = 0; j < battlefield[i].size(); ++j)
            {
                if (battlefield[i][j] == "")
                    cout << "|    ";
                else
                    cout << "|" << left << setfill('0') << setw(4) << battlefield[i][j];
            }
            cout << "|" << endl; // Close the last gap
        }

        // Close the last row
        cout << "    ";
        for (int j = 0; j < battlefield[0].size(); ++j)
        {
            cout << "+----";
        }
        cout << "+" << endl;
    }

    // Check if position is valid
    bool isPositionValid(int x, int y) const {
        return x >= 0 && x < BATTLEFIELD_NUM_OF_COLS && y >= 0 && y < BATTLEFIELD_NUM_OF_ROWS && battlefield[y][x].empty();
    }

    // Check if position is empty
    bool isPositionEmpty(int x, int y) const {
        return isPositionValid(x, y) &&
               battlefield[y][x].empty();
    }

    // Get robot position
    Robot* getRobotAt(int x, int y) const {
        if (!isPositionValid(x, y)) return nullptr;

        const string& id = battlefield[y][x];
        for (Robot* robot : robots) {
            if (robot->getId() == id) return robot;
        }
        return nullptr;
    }

    // queue the robots that died in the previous round
    void queueForRespawn(Robot* robot) {
        robot->setEntryTurn(currentTurn);
        waitingRobots.push(robot);
        cout << robot->getId() << " is enter the queue waiting robot..." << endl;
    }

    // Remove Robot from it's position
    void removeRobot(Robot* robot) {robot->setIsDestroyed(true);}

    // Permanently destroy robot because of no more lives
    void destroyRobot(Robot* robot) {
        removeRobot(robot);

        for (auto it = robots.begin(); it != robots.end(); ++it) {
            if (*it == robot) {
                destroyedRobots.push(*it);
                robot->getIsDestroyed() == true;
                cout << robot->getId() << " is enter the queue destroyed robot..." << endl;
                break;  // important: exit loop after erasing
            }
        }
    }
    // respawn / let robots re-enter the bf
    void respawnRobots() {
        while (!waitingRobots.empty()) {
            Robot* robot = waitingRobots.front();

            if (currentTurn - robot->getEntryTurn() >= 2)
            {
                waitingRobots.pop();

                // Find empty position
                int newX, newY;
                do {
                    newX = rand() % BATTLEFIELD_NUM_OF_COLS;
                    newY = rand() % BATTLEFIELD_NUM_OF_ROWS;
                } while (!isPositionEmpty(newX, newY));

                robot->setPosX(newX);
                robot->setPosY(newY);
                battlefield[newY][newX] = robot->getId();
                cout << robot->getId() << " is re-enter the battlefield at the position ("
                     << robot->getPosX() << ", " << robot->getPosY() << ")" << endl;

                robot->setEntryTurn(-1); // Mark as no longer queued
                robot->setIsDestroyed(false);
            }
            else
            {
                break;
            }
        }
    }

    // Control the turn of the Simulation
    void turnBased()
    {
        // Loop through robots in cycles until totalTurns is reached or Last robot standing
        while (currentTurn < totalTurns && robots.size() > 1) {
            currentTurn++;
            cout << "\nTurn " << currentTurn << ":" << endl;

            respawnRobots();
            placeRobots();

            // To select next active robot
            Robot* currentRobot = nullptr;
            for (int i = 0; i < robots.size(); ++i) {
                Robot* player = robots[(currentRobotIndex + i) % robots.size()]; // [%robots.size()] => If the robot's index reached the robots's size, it wraps it back to 0
                // To ensure the robot is still inside the battlefield and no longer inside the waiting queue
                if (player->isAlive() && player->getEntryTurn() == -1) {
                    currentRobot = player;
                    currentRobotIndex = (currentRobotIndex + i + 1) % robots.size(); // increment of currentRobotIndex
                break;
                }
            }

            if (currentRobot) {
                cout << *currentRobot;
                currentRobot->actions(this);
            }
            cout << endl;

            // Re-display battlefield after robot acts
            placeRobots();             // Re-update positions on the grid
            displayBattleField();      // Show updated battlefield

            cout << endl;
        }
    }

    // Upgrade robot after the robot get kills
    void upgrade(string upgradeType, Robot* robot)
    {
        Robot* temp = nullptr;
        Robot* newRobot = nullptr;

        // Get the id Number of the robot
        string id = robot->getId();
        id = id.substr(2,2);

        // Get the current position of the robot
        int x = robot->getPosX();
        int y = robot->getPosY();

        // Moving Robot
        if (upgradeType == "HideBot")
        {
            id = "HB" + id;
            //newRobot = new HideBot(id, x ,y);
        }
        else if (upgradeType == "JumpBot")
        {
            id = "JB" + id;
            //newRobot = new JumpBot(id, x ,y);
        }
        else if (upgradeType == "NewMoveBot")
        {
            id = "MB" + id; // x
            //newRobot = new NewMoveBot(id, x ,y);
        }

        // Shooting robot
        else if (upgradeType == "LongShotBot")
        {
             id = "LB" + id;
            //newRobot = new LongShotBot(id, x ,y);
        }
        else if (upgradeType == "SemiAutoBot")
        {
            id = "SA" + id;
            //newRobot = new SemiAutoBot(id, x ,y);
        }
        else if (upgradeType == "ThirthyShotBot")
        {
            id = "TS" + id;
            //newRobot = new ThirthyShotBot(id, x ,y);
        }
        else if (upgradeType == "NewShootBot")
        {
            id = "SB" + id; // x
            //newRobot = new NewShootBot(id, x ,y);
        }

        // Seeing robot
        else if (upgradeType == "ScoutBot")
        {
            id = "SB" + id;
            //newRobot = new ScoutBot(id, x ,y);
        }
        else if (upgradeType == "TrackBot")
        {
            id = "TB" + id;
            //newRobot = new TrackBot(id, x ,y);
        }
        else if (upgradeType == "NewSeeBot")
        {
            id = "NSB" + id; // x
            //newRobot = new NewSeeBot(id, x ,y);
        }

        newRobot->setName(robot->getName());
        newRobot->setType(upgradeType);

        cout << robot->getName() << "upgrade from " << robot->getType() << " " <<robot->getId() << " to " << upgradeType << " " << id <<endl;

        for (int i = 0; i < robots.size(); ++i)
        {
            if (robots[i]->getId()==robot->getId())
            {
                temp = robots[i];
                robots[i] = newRobot;
                newRobot = temp;
                break;
            }
        }

        // Remove the old robot
        delete newRobot;
        delete temp;
    }


    void upgradeMovingRobot (Robot* robot)
    {
        robot->setUpgradedMoving(1);
        string upgradeType;
        int randomNumber = rand() % 3;

        if (rand() == 0)
            upgradeType = "HideBot";
        else if (rand() == 1)
            upgradeType = "JumpBot";
        else if (rand() == 2)
            upgradeType = "NewMoveBot";

        upgrade(upgradeType, robot);
    }

    void upgradeShootingRobot (Robot* robot)
    {
        robot->setUpgradedShooting(1);
        string upgradeType;
        int randomNumber = rand() % 4;

        if (rand() == 0)
            upgradeType = "LongShotBot";
        else if (rand() == 1)
            upgradeType = "SemiAutoBot";
        else if (rand() == 2)
            upgradeType = "ThirthyShotBot";
        else if (rand() == 3)
            upgradeType = "NewShootBot";

        upgrade(upgradeType, robot);
    }

    void upgradeSeeingRobot (Robot* robot)
    {
        robot->setUpgradedSeeing(1);
        string upgradeType;
        int randomNumber = rand() % 3;

        if (rand() == 0)
            upgradeType = "ScoutBot";
        else if (rand() == 1)
            upgradeType = "TrackBot";
        else if (rand() == 2)
            upgradeType = "NewSeeBot";

        upgrade(upgradeType, robot);
    }


    void decideUpgradeType (Robot* robot)
    {
        int randomNumber = 0;
        robot->setNumUpgrade(robot->getNumUpgrade()+1);

        // first upgrade
        if (robot->getNumUpgrade() == 1)
        {
            randomNumber = rand() % 3;

            if (randomNumber == 0)
                upgradeMovingRobot(robot);
            else if (randomNumber == 1)
                upgradeShootingRobot(robot);
            else if (randomNumber == 2)
                upgradeSeeingRobot(robot);
        }

        // second upgrade
        else if (robot->getNumUpgrade() == 2)
        {
            randomNumber = rand() % 2;

            if (!robot->getUpgradedMoving()&& !robot->getUpgradedShooting())
            {
                if (randomNumber == 0)
                    upgradeMovingRobot(robot);
                else if (randomNumber == 1)
                    upgradeShootingRobot(robot);
            }
            else if (!robot->getUpgradedMoving() && !robot->getUpgradedSeeing())
            {
                if (randomNumber == 0)
                    upgradeMovingRobot(robot);
                else if (randomNumber == 1)
                    upgradeSeeingRobot(robot);
            }
            else if (!robot->getUpgradedShooting()&& !robot->getUpgradedSeeing())
            {
                if (randomNumber == 0)
                    upgradeShootingRobot(robot);
                else if (randomNumber == 1)
                    upgradeSeeingRobot(robot);
            }
        }

        // third upgrade
        else if (robot->getNumUpgrade() == 3)
        {
            if (!robot->getUpgradedMoving())
                upgradeMovingRobot(robot);
            else if (!robot->getUpgradedSeeing())
                upgradeSeeingRobot(robot);
            else if (!robot->getUpgradedShooting())
                upgradeShootingRobot(robot);
        }
        else
        {
            cout << "Robot " << robot->getId() << " cannot ugrade anymore!" << endl;
        }
    }

};

void GenericRobot::actionThink (Battlefield* battlefield) {
    cout << "GenericRobot actionThink" << endl;

    cout << getId() << " is thinking..." << endl;
}
void GenericRobot::actionLook (Battlefield* battlefield) {

    // Check all 8 direction for enemies
    for (int directionCheckEnemy = 0; directionCheckEnemy < 8; directionCheckEnemy++) {
        int lookX = getPosX() + dx[directionCheckEnemy];
        int lookY = getPosY() + dy[directionCheckEnemy];
        hasEnemy[directionCheckEnemy] = battlefield->isPositionValid(lookX, lookY) && battlefield->getRobotAt(lookX, lookY) != nullptr;
    }

    // Check all 9 movement options
    for (int directionCheckMoves = 0; directionCheckMoves < 9; directionCheckMoves++) {
        int moveX = getPosX() + dx[directionCheckMoves];
        int moveY = getPosY() + dy[directionCheckMoves];
        canMove[directionCheckMoves] = (directionCheckMoves == 8) ? true : (battlefield->isPositionValid(moveX, moveY)) && (battlefield->isPositionEmpty(moveX, moveY));
    }

    cout<<"GenericRobot actionLook" << endl;

    cout << getId() << " is looking around..." << endl;
}
void GenericRobot::actionFire(Battlefield* battlefield) {
    cout << "GenericRobot actionFire" << endl;

    // Generate random direction to shot at (excluding current position)
    int shotAtX, shotAtY;

    do {
        shotAtX = (rand() % 3) - 1; // -1, 0, or 1
        shotAtY = (rand() % 3) - 1;
    } while ((shotAtX == 0 && shotAtY == 0) || !battlefield->isPositionValid(shotAtX, shotAtY));


    int targetX = getPosX() + shotAtX;
    int targetY = getPosY() + shotAtY;

    Robot* target = battlefield->getRobotAt(targetX, targetY);

    if (target != nullptr && target != this) {

        // 70% chance to hit
        if (rand() % 100 < 70) {
            // Reduce target's lives
            target->reduceLife();
            battlefield->removeRobot(target);

            // Check if target was destroyed
            if (target->getLives() >= 1) {
                battlefield->queueForRespawn(target); // The target enter waiting robot queue
            }
            else {
                cout << target->getId() << " is out of lives! " << target->getId() << " was destroyed!" << endl;
                battlefield->destroyRobot(target); // Battlefield handles destruction
            }

            incrementKills();
            battlefield->decideUpgradeType(this); // Upgrade to a new robot after get kills
            cout << getId() << " hit " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
        }
        else {
            cout << getId() << " missed " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
            }
        }

        else {
            cout << getId() << " fired at empty space (" << targetX << "," << targetY << ")" << endl;
        }

    // Handle ammo and self-destruction
    shellsRemaining--;

    if (shellsRemaining <= 0) {
        cout << getId() << " is out of ammo and self-destructs!" << endl;
        selfDestruct();
        battlefield->removeRobot(this);

        if (this->getLives() >= 1) {

            battlefield->queueForRespawn(this);
        }
        else {
            cout << getId() << " is out of lives! " << getId() << " was destroyed!" << endl;
            battlefield->destroyRobot(this); // Use the robot's own selfDestruct method
        }
    }
}
void GenericRobot::actionMove(Battlefield* battlefield) {
    cout << "GenericRobot actionMove" << endl;

    vector<int> validMoves;

    // Collect all valid movement directions (including standing still)
    for (int dir = 0; dir < 9; ++dir) {
        if (canMove[dir]) validMoves.push_back(dir);
    }

    if (!validMoves.empty()) {
        // Randomly select one of the valid directions
        int dir = validMoves[rand() % validMoves.size()];

        int newX = getPosX() + dx[dir];
        int newY = getPosY() + dy[dir];

        if (dir == 8) {
            // Standing still
            cout << getId() << " decides to stay in place." << endl;
        }
        else{
            // Move to new position
            setPosX(newX);
            setPosY(newY);
            cout << getId() << " moves to (" << newX << "," << newY << ")" << endl;
        }
    }
    else{
        cout << getId() << " decides to stay in place." << endl;
    }
}

int main()
{
    srand(242213244718 / 100); //Leader ID = 242UC244GR, U=21,C=3,G=7,R=18

    Battlefield b;
    b.readInputFile("inputFile1.txt");
    b.placeRobots();
    b.displayBattleField();
    b.turnBased();

    b.getTotalTurns();
    return 0;
}
