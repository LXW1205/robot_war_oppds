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
// Member_1: Upgrade function, Robot upgrades for shooting
// Member_2: Robot actions, Robot upgrades for Moving, Output file (log)
// Member_3: Read file input function, Robot upgrades for seeing, Turns and queue
// *********************************************************

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>
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

    int numberOfLives = 3;
    int numberOfKills = 0;
    int queueEntryTurn = -1; // All the robots not in the queue at the beginning

    bool isDestroyed = false;

    bool isAbleUpgrade = false;
    bool upgradedMoving = 0;
    bool upgradedShooting = 0;
    bool upgradedSeeing = 0;

    //Detects directions the robot can move
    bool canMove[9] = {false};

    //Detects the presence of other robots
    bool hasEnemy[8] = {false};

    // direction arrays like Up, Up-Right, Right, Down-Right, Down, Down-Left, Left, Up-Left, Stand in place
    const int dx[9] = { 0, 1, 1, 1, 0,-1,-1,-1, 0};
    const int dy[9] = {-1,-1, 0, 1, 1, 1, 0,-1, 0};

    int numUpgrade = 0;
    int shellsRemaining = 10;

public:
    // Parameterized Constructor(PC)
    Robot(string id, int posX, int posY) : robotId(id), robotPositionX(posX), robotPositionY(posY) {}

    // Destructor (virtual for polymorphism)
    virtual ~Robot() {}

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
    void setIsDestroyed(bool destroyed) {isDestroyed = destroyed;}

    //Getter and Setter for isAbleUpgrade
    bool getIsAbleUpgrade() const {return isAbleUpgrade;}
    void setIsAbleUpgrade(bool available) {isAbleUpgrade = available;}

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

    //Getter for shellsRemaining
    int getShells() const { return this->shellsRemaining; }

    // Reduce life when getting shoot or selfDestruct
    void reduceLife() {setLives(getLives() - 1);}

    // selfDestruct when ran out of shells(ammo)
    void selfDestruct() { reduceLife(); }

    // Increase kills
    void incrementKills() {numberOfKills++;}

    // Check if the robot still have lives
    bool isAlive() const {return numberOfLives > 0;}

    //Getter for hidden, which is always false
    virtual bool getIsHidden() const { return false; }

    // To reset the bullet shells
    virtual void resetShells() { shellsRemaining = 10; }

    // To update the track targeted info
    virtual void updateRobotInfo(vector<Robot*>& r, Robot* newRobot) {}

    // Overloading the << operator for Robot class
    friend ostream& operator<<(ostream &COUT, const Robot& r)
    {
        COUT << r.robotType << " " << r.robotId << "_" << r.robotName << " turns" << endl
        << r.robotId << "'s lives remaining: " << r.numberOfLives << endl
        << r.robotId << "'s shells left: " << r.shellsRemaining << endl
        << r.robotId << "'s kills: " << r.numberOfKills << endl
        << r.robotId << "'s no.of Upgrades: " << r.numUpgrade << endl << endl
        << r.robotId << " at (" << r.robotPositionX << ", " << r.robotPositionY << ") actions:" << endl;
        return COUT;
    }

    // Overloading the == operator for Robot class
    bool operator==(const Robot& other) const
    {
        return this->robotId == other.robotId;
    }

    // Set Robot Location x y
    virtual void setRobotLocation(int x, int y)
    {
        robotPositionX = x;
        robotPositionY = y;
    }

    // Pure Virtual Functions
    virtual void actions(Battlefield* battlefield) = 0;
};

class ThinkingRobot: virtual public Robot
{
public:
    virtual ~ThinkingRobot(){}

    // Virtual function for thinking
    virtual void actionThink (Battlefield* battlefield);
};

class SeeingRobot: virtual public Robot
{
public:
    virtual ~SeeingRobot(){}

    // Virtual function for looking
    virtual void actionLook(Battlefield* battlefield);
};

class ShootingRobot: virtual public Robot
{
public:
    virtual ~ShootingRobot(){}

    // Virtual function for shooting
    virtual void actionFire(Battlefield* battlefield);
};

class MovingRobot: virtual public Robot
{
public:
    virtual ~MovingRobot(){}

    // Virtual function for moving
    virtual void actionMove(Battlefield* battlefield);
};

class GenericRobot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:
    static int robotAutoIncrementInt_; // Static member for auto-incrementing ID

public:
    GenericRobot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;

        robotAutoIncrementInt_++;
    }

    static int robotAutoIncrementInt() { return robotAutoIncrementInt_; }

    virtual ~GenericRobot() {}

    // Function override
    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};
int GenericRobot::robotAutoIncrementInt_ = 0;

class ScoutBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:
    int scoutLimit = 3;

public:
    ScoutBot(string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~ScoutBot() {}

    virtual void actionLook(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);
        }
    }
};

class TrackBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:
    int trackerNumber = 3;

    vector<Robot*> trackTargets; // To store the track targeted enemy

public:
    TrackBot(string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~TrackBot()
    {
            trackTargets.clear();
    }

    void updateRobotInfo(vector<Robot*>& allRobots, Robot* newRobot) override
    {
        for (size_t i = 0; i < trackTargets.size(); ++i)
        {   // The robot is already upgraded
            if (trackTargets[i] == newRobot)
                return;

            // Replace the old pointer with the upgraded one if match
            for (Robot* oldRobot : allRobots)
            {
                if (trackTargets[i] == oldRobot)
                {
                    if (oldRobot->getName() == newRobot->getName())
                    {
                        trackTargets[i] = newRobot;
                        break;
                    }
                }
            }
        }
    }

    bool checkTrackTargeted(Robot* robot)
    {
        for (Robot* targeted : trackTargets)
        {
            if (targeted == robot)
                return true;
        }
        return false;
    }

    virtual void actionLook(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);
        }
    }
};

class DroneBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:
    int droneNumber = 3;

    vector<pair<int, int>> placedDronePositions; // To store the drones position

public:
    DroneBot(string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~DroneBot() {}

    bool checkDronePosition(int x, int y, const vector<pair<int, int>>& placedDronePositions)
    {
        for (const auto& position : placedDronePositions)
        {
            if (position.first == x && position.second == y)
                return true;
        }
        return false;
    }

    virtual void actionLook(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink(battlefield);

            if (!isAlive()) return;
            actionLook(battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire(battlefield);
        }
    }
};

class LongShotBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
public:
    LongShotBot(string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~LongShotBot() {}

    virtual void actionFire(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }

};

class SemiAutoBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
public:
    SemiAutoBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~SemiAutoBot() {}

    virtual void actionFire(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class ThirthyShotBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
public:
    ThirthyShotBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
        shellsRemaining = 30;
    }

    virtual ~ThirthyShotBot() {}

    // Function override
    void resetShells() override { shellsRemaining = 30; }

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class BomberBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
public:
    BomberBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
        shellsRemaining = 5;
    }

    virtual ~BomberBot() {}

    // Function override
    void resetShells() override { shellsRemaining = 5; }

    virtual void actionFire(Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = rand();

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class HideBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:

    int hideSkill = 3;

    bool isHidden = false;

public:
    HideBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~HideBot() {}

    bool getIsHidden() const override { return isHidden; }

    virtual void actionMove (Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = 0;

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }

        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class JumpBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:

    int jumpSkill = 3;

public:
    JumpBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~JumpBot() {}

    virtual void actionMove (Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = 0;

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class PortalBot: public ThinkingRobot, public SeeingRobot, public ShootingRobot, public MovingRobot
{
private:

    int portalSkill = 3;

public:
    PortalBot (string id = "", int x = -1, int y = -1): Robot(id, x, y)
    {
        robotId = id;
        robotPositionX = x;
        robotPositionY = y;
    }

    virtual ~PortalBot() {}

    virtual void actionMove (Battlefield* battlefield) override;

    virtual void actions (Battlefield* battlefield) override
    {
        int randomInt = 0;

        if( randomInt % 2 == 0)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {

            if (!isAlive()) return;
            actionThink (battlefield);

            if (!isAlive()) return;
            actionLook (battlefield);

            if (!isAlive()) return;
            actionMove(battlefield);

            if (!isAlive()) return;
            actionFire (battlefield);
        }
    }
};

class Battlefield
{
private:
    int BATTLEFIELD_NUM_OF_COLS = -1;
    int BATTLEFIELD_NUM_OF_ROWS = -1;

    int totalTurns = -1; // Total number of turns
    int currentTurn = 0; // Current turn number

    int numOfRobots = -1; // Number of robots
    int currentRobotIndex = 0;

    vector<Robot*> robots;
    queue<Robot*> destroyedRobots;
    queue<Robot*> waitingRobots;

    vector<vector<string>> battlefield; // 2D vector representing the battlefield

public:
    ~Battlefield()
    {

        while (!destroyedRobots.empty())
        {
            delete destroyedRobots.front();
            destroyedRobots.pop();
        }
        while (!waitingRobots.empty())
        {
            delete waitingRobots.front();
            waitingRobots.pop();
        }
        while (!robots.empty())
        {
            delete robots[0];
            robots.erase(robots.begin());
        }
    }

    // Getter functions
    int getBATTLEFIELD_NUM_OF_COLS() const
    {
        return BATTLEFIELD_NUM_OF_COLS;
    }
    int getBATTLEFIELD_NUM_OF_ROWS () const
    {
        return BATTLEFIELD_NUM_OF_ROWS;
    }
    int getTotalTurns() const
    {
        return totalTurns;
    }
    int getCurrentTurn() const
    {
        return currentTurn;
    }
    int getNumOfRobots () const
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

        cout << "Read " << fileInputName << ":" << endl;
        fileOutput << "Read " << fileInputName << ":" << endl;

        string line;

        while (getline(fileInput, line))
        {

            cout << line << endl;
            fileOutput << line << endl;

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
            }
            else if (line.find("robots:") != string::npos)
            {
                // Read the robots line
                istringstream robotStream(line);
                string ignore;
                robotStream >> ignore >> numOfRobots;
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

        cout << endl;
        fileOutput << endl;

        // check to avoid same robot exist
        for (int i=0;i<robots.size()-1;i++)
        {
            if (robots[i]==robots[i+1])
            {
                cout << robots[i]->getId() << " and " << robots[i+1]->getId() << " are the same!" << endl;
            }
        }

        fileInput.close();
    }

    //Initialize Output file
    ofstream fileOutput;

    //Get the name of the OutputFile and open the file
    void writeOutputFile(const string& fileOutputName)
    {
        fileOutput.open(fileOutputName);

        // Check the file is able to open
        if (!fileOutput.is_open())
        {
            cerr << "Unable to open file" << endl;
            exit(-1);
        }
    }

    //For outside Battlefield class use
    template<typename T>
    Battlefield& operator<<(const T& data)
    {
        cout << data;
        if (fileOutput.is_open()) fileOutput << data;
        return *this;
    }

    // Overload for manipulators like endl
    Battlefield& operator<<(ostream& (*manip)(ostream&))
    {
        manip(cout);
        if (fileOutput.is_open()) fileOutput << manip;
        return *this;
    }

    //Close the OutputFile after finish writing
    void closeOutputFile()
    {
        if (fileOutput.is_open())
        {
            fileOutput.close();
        }
    }

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
    void displayBattleField()
    {
        cout << "Display Battlefield:" << endl;
        fileOutput << "Display Battlefield:" << endl;

        cout << "    ";
        fileOutput << "    ";

        // Print the column header
        for (int j = 0; j < battlefield[0].size(); ++j)
        {
            cout << "  " << right << setfill('0') << setw(2) << j << " ";
            fileOutput << "  " << right << setfill('0') << setw(2) << j << " ";
        }
        cout << endl;
        fileOutput << endl;

        // Print the battle field's row
        for (int i = 0; i < battlefield.size(); ++i)
        {
            cout << "    ";
            fileOutput << "    ";
            for (int j = 0; j < battlefield[i].size(); ++j)
            {
                cout << "+----";
                fileOutput << "+----";
            }
            cout << "+" << endl; // Close the last gap
            fileOutput << "+" << endl; // Close the last gap

            // Print the row header
            cout << "  " << right << setfill('0') << setw(2) << i;
            fileOutput << "  " << right << setfill('0') << setw(2) << i;
            for (int j = 0; j < battlefield[i].size(); ++j)
            {
                if (battlefield[i][j] == "")
                {
                    cout << "|    ";
                    fileOutput << "|    ";
                }
                else
                {
                    cout << "|" << left << setfill('0') << setw(4) << battlefield[i][j];
                    fileOutput << "|" << left << setfill('0') << setw(4) << battlefield[i][j];
                }
            }
            cout << "|" << endl; // Close the last gap
            fileOutput << "|" << endl; // Close the last gap
        }

        // Close the last row
        cout << "    ";
        fileOutput << "    ";
        for (int j = 0; j < battlefield[0].size(); ++j)
        {
            cout << "+----";
            fileOutput << "+----";
        }
        cout << "+" << endl;
        fileOutput << "+" << endl;
    }

    // Check if position is valid
    bool isPositionValid(int x, int y) const
    {
        return x >= 0 && x < BATTLEFIELD_NUM_OF_COLS && y >= 0 && y < BATTLEFIELD_NUM_OF_ROWS;
    }

    // Check if position is empty
    bool isPositionEmpty(int x, int y) const
    {
        return isPositionValid(x, y) &&
               battlefield[y][x].empty();
    }

    // Get robot position
    Robot* getRobotAt(int x, int y) const
    {
        if (!isPositionValid(x, y)) return nullptr;

        const string& id = battlefield[y][x];
        for (Robot* robot : robots) {
            if (robot->getId() == id) return robot;
        }
        return nullptr;
    }

    // queue the robots that died in the previous round
    void queueForRespawn(Robot* robot)
    {
        robot->setEntryTurn(currentTurn);
        waitingRobots.push(robot);
        cout << robot->getId() << " is enter the queue waiting robot..." << endl;
        fileOutput << robot->getId() << " is enter the queue waiting robot..." << endl;
    }

    // Remove Robot from it's position
    void removeRobot(Robot* robot)
    {
        robot->setIsDestroyed(true);
        cout << robot->getId() << " has " << robot->getLives() << " lives remaining" << endl;
        fileOutput << robot->getId() << " has " << robot->getLives() << " lives remaining" << endl;
    }

    // Permanently destroy robot because of no more lives
    void destroyRobot(Robot* robot)
    {
        for (auto it = robots.begin(); it != robots.end(); ++it) {
            if (*it == robot) {
                destroyedRobots.push(*it);
                robot->setIsDestroyed(true);
                cout << robot->getId() << " is enter the queue destroyed robot..." << endl;
                fileOutput << robot->getId() << " is enter the queue destroyed robot..." << endl;
                break;  // important: exit loop after erasing
            }
        }
        numOfRobots--;
    }

    // respawn or let robots re-enter the battlefield
    void respawnRobots()
    {
        while (!waitingRobots.empty()) {
            Robot* robot = waitingRobots.front();

            if (currentTurn - robot->getEntryTurn() >= 2)
            {
                waitingRobots.pop();

                // Find a random empty position
                int newX, newY;
                do {
                    newX = rand() % BATTLEFIELD_NUM_OF_COLS;
                    newY = rand() % BATTLEFIELD_NUM_OF_ROWS;
                } while (!isPositionEmpty(newX, newY));

                robot->setPosX(newX);
                robot->setPosY(newY);
                battlefield[newY][newX] = robot->getId();
                robot->resetShells();
                cout << robot->getId() << " is re-enter the battlefield at the position ("
                     << robot->getPosX() << ", " << robot->getPosY() << ")" << endl << endl;

                fileOutput << robot->getId() << " is re-enter the battlefield at the position ("
                     << robot->getPosX() << ", " << robot->getPosY() << ")" << endl << endl;

                robot->setEntryTurn(-1); // Mark as no longer queued
                robot->setIsDestroyed(false);
            }
            else
            {
                break;
            }
        }
    }

    //Check if there is only one robot remaining
    bool isLastStand ()
    {
        int aliveCount = 0;

        for (Robot* robot : robots)
        {
            if (robot->isAlive())
            {
                aliveCount++;

                if (aliveCount > 1)
                {
                    return false;
                }
            }
        }

        return true; // 0 or 1 robot alive
    }

    // Return the last surviving robot (if any), or nullptr if none
    Robot* lastStandRobot()
    {
        for (Robot* livingRobots : robots) {
            if (livingRobots->isAlive()) {
                return livingRobots;
            }
        }
        return nullptr; // No robot alive
    }

    // Control the turn of the Simulation
    void turnBased()
    {
        // Loop through robots in cycles until totalTurns is reached or Last robot standing
        while (currentTurn < totalTurns && !isLastStand()) {
            currentTurn++;
            cout << "\nTurn " << currentTurn << ":" << endl;
            fileOutput << "\nTurn " << currentTurn << ":" << endl;

            respawnRobots(); // Respawn the robot from waiting queue
            placeRobots(); // Update the respawn robot's in battlefield

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
                fileOutput << *currentRobot;

                // Check the robot is able to get an upgrade
                currentRobot->actions(this);
                if (currentRobot->getIsAbleUpgrade())
                {
                    decideUpgradeType(currentRobot); // Upgrade to a new robot after get kills
                    currentRobot->setIsAbleUpgrade(false);
                }
            }
            cout << endl;
            fileOutput << endl;

            // Re-display battlefield after robot acts
            placeRobots();             // Re-update positions on the grid
            displayBattleField();      // Show updated battlefield

            cout << endl;
            fileOutput << endl;
        }

        // Game over simulation check
        if (isLastStand()) // If there is one robot left inside the battlefield
        {
            cout << "--SIMULATION ENDS--- in turn " << currentTurn << endl
            << lastStandRobot()->getId() << "_" << lastStandRobot()->getName() << " WINS the match!!" << endl;
            fileOutput << "--SIMULATION ENDS--- in turn " << currentTurn << endl
            << lastStandRobot()->getId() << "_" << lastStandRobot()->getName() << " WINS the match!!" << endl;
        }
        else
        {
            cout << "--GAME OVER-- Maximum turns(" << totalTurns << ") reached!!" << endl
            << "Robots remaining: " << numOfRobots << endl;

            fileOutput << "--GAME OVER-- Maximum turns(" << totalTurns << ") reached!!" << endl
            << "Robots remaining: " << numOfRobots << endl;
        }

        cout << endl;
        fileOutput << endl;

        cout << "Static members:" << endl <<
        "Number of GenericRobot robots: " << GenericRobot::robotAutoIncrementInt() << endl << endl;
        fileOutput << "Static members:" << endl <<
        "Number of GenericRobot robots: " << GenericRobot::robotAutoIncrementInt() << endl << endl;

        cout << "queue data structure:" << endl;

        while (!destroyedRobots.empty())
        {
            Robot* printDestroyedRobot = destroyedRobots.front();
            destroyedRobots.pop();

            if (printDestroyedRobot)
            {
                cout << "destroyedRobots_: Robot " << printDestroyedRobot->getId() <<
                " at (" << printDestroyedRobot->getPosX() <<
                "," << printDestroyedRobot->getPosY() << ")" << endl;

                fileOutput << "destroyedRobots_: Robot " << printDestroyedRobot->getId() <<
                " at (" << printDestroyedRobot->getPosX() <<
                "," << printDestroyedRobot->getPosY() << ")" << endl;

                //delete printDestroyedRobot; // Avoid memory leak
            }
        }

        while (!waitingRobots.empty())
        {
            Robot* printWaitingRobot = waitingRobots.front();
            waitingRobots.pop();

            if (printWaitingRobot)
            {

                cout << "waitingRobots_: Robot " << printWaitingRobot->getId() <<
                " at (" << printWaitingRobot->getPosX() <<
                "," << printWaitingRobot->getPosY() << ")" << endl;

                fileOutput << "waitingRobots_: Robot " << printWaitingRobot->getId() <<
                " at (" << printWaitingRobot->getPosX() <<
                "," << printWaitingRobot->getPosY() << ")" << endl;

                //delete printWaitingRobot; // Avoid memory leak
            }
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
            newRobot = new HideBot(id, x ,y);
        }
        else if (upgradeType == "JumpBot")
        {
            id = "JB" + id;
            newRobot = new JumpBot(id, x ,y);
        }
        else if (upgradeType == "PortalBot")
        {
            id = "PB" + id; // x
            newRobot = new PortalBot(id, x ,y);
        }

        // Shooting robot
        else if (upgradeType == "LongShotBot")
        {
             id = "LB" + id;
            newRobot = new LongShotBot(id, x ,y);
        }
        else if (upgradeType == "SemiAutoBot")
        {
            id = "SA" + id;
            newRobot = new SemiAutoBot(id, x ,y);
        }
        else if (upgradeType == "ThirthyShotBot")
        {
            id = "TS" + id;
            newRobot = new ThirthyShotBot(id, x ,y);
        }
        else if (upgradeType == "BomberBot")
        {
            id = "BB" + id; // x
            newRobot = new BomberBot(id, x ,y);
        }

        // Seeing robot
        else if (upgradeType == "ScoutBot")
        {
            id = "SB" + id;
            newRobot = new ScoutBot(id, x ,y);
        }
        else if (upgradeType == "TrackBot")
        {
            id = "TB" + id;
            newRobot = new TrackBot(id, x ,y);
        }
        else if (upgradeType == "DroneBot")
        {
            id = "DB" + id; // x
            newRobot = new DroneBot(id, x ,y);
        }

        newRobot->setName(robot->getName());
        newRobot->setType(upgradeType);

        cout << endl;
        fileOutput << endl;

        cout << robot->getName() << " upgrade from " << robot->getType() << " " << robot->getId() << " to " << upgradeType << " " << id <<endl;
        fileOutput << robot->getName() << " upgrade from " << robot->getType() << " " << robot->getId() << " to " << upgradeType << " " << id <<endl;

        // To update the trackTargeted vector in TrackBot class
        for (Robot* r : robots)
        {   // r is not itself
            if (r != robot && r != nullptr)
                r->updateRobotInfo(robots, newRobot); // Check whether this upgraded robot is a track targeted or not
        }

        for (int i = 0; i < robots.size(); ++i)
        {
            if (robots[i]==robot)
            {
                int tempNumUpgrades = robots[i]->getNumUpgrade();
                int tempKills = robots[i]->getKills();
                int tempLives = robots[i]->getLives();

                bool tempSeeing = robots[i]->getUpgradedSeeing();
                bool tempShooting = robots[i]->getUpgradedShooting();
                bool tempMoving = robots[i]->getUpgradedMoving();

                delete robots[i];
                robots[i] = newRobot;

                //Copy info from old robot
                robots[i]->setNumUpgrade(tempNumUpgrades);
                robots[i]->setKills(tempKills);
                robots[i]->setLives(tempLives);
                robots[i]->setUpgradedSeeing(tempSeeing);
                robots[i]->setUpgradedShooting(tempShooting);
                robots[i]->setUpgradedMoving(tempMoving);
                break;
            }
        }
    }

    void upgradeMovingRobot (Robot* robot)
    {
        robot->setUpgradedMoving(1);
        string upgradeType;
        int randomNumber = rand() % 3;

        if (randomNumber == 0)
            upgradeType = "HideBot";
        else if (randomNumber == 1)
            upgradeType = "JumpBot";
        else if (randomNumber == 2)
            upgradeType = "PortalBot";

        upgrade(upgradeType, robot);
    }

    void upgradeShootingRobot (Robot* robot)
    {
        robot->setUpgradedShooting(1);
        string upgradeType;
        int randomNumber = rand() % 4;

        if (randomNumber == 0)
            upgradeType = "LongShotBot";
        else if (randomNumber == 1)
            upgradeType = "SemiAutoBot";
        else if (randomNumber == 2)
            upgradeType = "ThirthyShotBot";
        else if (randomNumber == 3)
            upgradeType = "BomberBot";

        upgrade(upgradeType, robot);
    }

    void upgradeSeeingRobot (Robot* robot)
    {
        robot->setUpgradedSeeing(1);
        string upgradeType;
        int randomNumber = rand() % 3;

        if (randomNumber == 0)
            upgradeType = "ScoutBot";
        else if (randomNumber == 1)
            upgradeType = "TrackBot";
        else if (randomNumber == 2)
            upgradeType = "DroneBot";

        upgrade(upgradeType, robot);
    }


    void decideUpgradeType (Robot* robot)
    {
        int randomNumber = 0;

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
            cout << "Robot " << robot->getId() << " cannot upgrade anymore!" << endl;
            fileOutput << "Robot " << robot->getId() << " cannot upgrade anymore!" << endl;
        }
    }
};

void ThinkingRobot::actionThink (Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionThink--" << endl;

    *battlefield << getId() << " is thinking..." << endl;

    *battlefield << endl;
}
void SeeingRobot::actionLook (Battlefield* battlefield)
{
    *battlefield<< "--" << getType() << " actionLook--" << endl;

    *battlefield << getId() << " is looking around..." << endl;

    // Check all 8 direction for enemies
    for (int directionCheckEnemy = 0; directionCheckEnemy < 8; directionCheckEnemy++) {
        int lookX = getPosX() + dx[directionCheckEnemy];
        int lookY = getPosY() + dy[directionCheckEnemy];
        hasEnemy[directionCheckEnemy] = battlefield->isPositionValid(lookX, lookY) && battlefield->getRobotAt(lookX, lookY) != nullptr;

        Robot* enemy = battlefield->getRobotAt(lookX, lookY);

        hasEnemy[directionCheckEnemy] = battlefield->isPositionValid(lookX, lookY) && enemy != nullptr &&!enemy->getIsHidden();  // <- skip hidden robots
        if (hasEnemy[directionCheckEnemy] == true)
        {
            *battlefield << getId() << " found " << enemy->getId() << " at the position (" << lookX << ", " << lookY << ")" << endl;
        }
    }

    // Check all 9 movement options
    for (int directionCheckMoves = 0; directionCheckMoves < 9; directionCheckMoves++) {
        int moveX = getPosX() + dx[directionCheckMoves];
        int moveY = getPosY() + dy[directionCheckMoves];
        canMove[directionCheckMoves] = (directionCheckMoves == 8) ? true : (battlefield->isPositionValid(moveX, moveY)) && (battlefield->isPositionEmpty(moveX, moveY));
    }

    *battlefield << endl;
}
void ShootingRobot::actionFire(Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionFire--" << endl;

    // Generate random direction to shot at (excluding current position)
    int targetX, targetY, shotAtX, shotAtY;

    do {
        shotAtX = (rand() % 3) - 1; // -1, 0, or 1
        shotAtY = (rand() % 3) - 1;

        targetX = getPosX() + shotAtX;
        targetY = getPosY() + shotAtY;
    // To ensure that the robot will not fire at its own position and outside the battlefield
    } while ((shotAtX == 0 && shotAtY == 0) || !battlefield->isPositionValid(targetX, targetY));

    Robot* target = battlefield->getRobotAt(targetX, targetY);

    if (target != nullptr && target != this && !target->getIsHidden()) {

        // 70% chance to hit
        if (rand() % 100 < 70) {
            // If hit the robot target
            *battlefield << getId() << " hit " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
            incrementKills(); // Number of kills + 1

            // Reduce target's lives
            target->reduceLife();
            battlefield->removeRobot(target);

            // Check if target was destroyed
            if (target->getLives() >= 1) {
                battlefield->queueForRespawn(target); // The target enter waiting robot queue
            }
            else {
                *battlefield << target->getId() << " was destroyed!" << endl;
                battlefield->destroyRobot(target); // Battlefield handles destruction
            }

            // Check the robot whether is able to get an upgrade or not
            if (getNumUpgrade() < 3)
            {
                setNumUpgrade(getNumUpgrade()+1);
                setIsAbleUpgrade(true); // Set to true if the robot has killed the target
            }
        }
        else {
            *battlefield << getId() << " missed " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;

            }
        }

        else {
            *battlefield << getId() << " fired at empty space (" << targetX << "," << targetY << ")" << endl;
        }

    // Handle ammo and self-destruction
    shellsRemaining--;

    if (shellsRemaining <= 0) {
        *battlefield << getId() << " is out of ammo and self-destructs!" << endl;
        selfDestruct();
        battlefield->removeRobot(this);

        if (this->getLives() >= 1) {

            battlefield->queueForRespawn(this);
        }
        else {
            *battlefield << getId() << " was destroyed!" << endl;
            battlefield->destroyRobot(this); // Use the robot's own selfDestruct method
        }
    }

    *battlefield << endl;
}
void MovingRobot::actionMove(Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionMove--" << endl;

    vector<int> validMoves;

    // Collect all valid movement directions (including standing still)
    for (int dir = 0; dir < 9; ++dir) {
        if (canMove[dir]) validMoves.push_back(dir);
    }

    if (!validMoves.empty()) {
        // Randomly select one of the valid directions
        int dir, newX, newY;
        do {
            dir = validMoves[rand() % validMoves.size()];

            newX = getPosX() + dx[dir];
            newY = getPosY() + dy[dir];
        // To ensure that the robot is not going to move to other robot's position
        } while (!battlefield->isPositionEmpty(newX, newY));

        if (dir == 8) {
            // Standing still
            *battlefield << getId() << " decides to stay in place." << endl;

        }
        else{
            // Move to new position
            setPosX(newX);
            setPosY(newY);
            battlefield->placeRobots();
            *battlefield << getId() << " moves to (" << newX << "," << newY << ")" << endl;


        }
    }
    else{
        *battlefield << getId() << " decides to stay in place." << endl;
    }

    *battlefield << endl;
}

/*
Instead of neighbor look(x,y) The robot can look the entire battlefield for one turn.
The ability can be used three times in a match.*/
void ScoutBot::actionLook (Battlefield* battlefield)
{
    // Check the number of scout
    if (scoutLimit > 0)
    {
        int randomNumber = rand();

        // Ready to scout the entire battlefield
        if (randomNumber % 2 == 0)
        {
            *battlefield << "--" << getType() << " actionLook--" << endl;

            *battlefield << getId() << " is scouting the entire battlefield..." << endl;

            for (int i = 0; i < battlefield->getBATTLEFIELD_NUM_OF_ROWS(); i++)
            {
                for (int j = 0; j < battlefield->getBATTLEFIELD_NUM_OF_COLS(); j++)
                {
                    if (battlefield->isPositionEmpty(j, i) || (j == getPosX() && i == getPosY())) // Skip the empty position and self position
                    {
                        continue;
                    }
                    else
                    {
                        Robot* enemy = battlefield->getRobotAt(j, i); // Get the enemy position
                        *battlefield << "=> " << getId() << " found " << enemy->getId() << " at (" << j << ", "  << i << ")" << endl;

                    }
                }
            }
            scoutLimit--; // scoutLimit - 1
            *battlefield << getId() << " has " << scoutLimit << " scouts remaining" << endl;
            *battlefield << endl;
        }
        else
        {
            SeeingRobot::actionLook(battlefield); // Perform the normal look action
            *battlefield << endl;
        }
    }
    else
    {
        *battlefield << getId() << " has no scout left!!" << endl;
        SeeingRobot::actionLook(battlefield); // Perform the normal look action
        *battlefield << endl;
    }
}

/*
The robot can plant a tracker on another enemy robot so that it can look.
The location of the targeted enemy robot will be known to the robot until the end of a match.
The robot has maximum of three trackers to look on maximum of three enemy robots.*/
void TrackBot::actionLook (Battlefield* battlefield)
{
    // Perform the normal look action
    SeeingRobot::actionLook(battlefield);

    // Check the number of trackers
    if (trackerNumber > 0)
    {
        int randomNumber = rand();

        // To plant a tracker on another enemy robot
        if (randomNumber % 2 == 0)
        {
            *battlefield << getId() << " is finding the target to plant a tracker..." << endl;

            vector<Robot*> validTargets;
            vector <Robot*>& robots = battlefield->getRobots();
            for (Robot* robot : robots)
            {
                // To ensure the trackBot will not track on its own or the robot has been destroyed or at the same targeted robot
                if (robot != this && !robot->getIsDestroyed() && !checkTrackTargeted(robot))
                    validTargets.push_back(robot);
            }

            Robot* target = nullptr;
            if (!validTargets.empty())
            {
                int index = rand() % validTargets.size(); // Target the random valid robot
                target = validTargets[index];

                trackTargets.push_back(target); // Update the track targeted enemy
                *battlefield << getId() << " has planted a tracker on " << target->getId() << endl;

                trackerNumber--; // trackerNumber - 1
                *battlefield << getId() << " has " << trackerNumber << " trackers remaining" << endl;
            }
        }
    }
    else
        *battlefield << getId() << " has no tracker left!!" << endl;

    // Check each tracker status
    for (Robot* target : trackTargets)
    {
        if (target->isAlive() == true && target != nullptr) // Check the targeted is alive and not been destrroyed
        {
            *battlefield << getId() << " tracked " << target->getId() << " at (" << target->getPosX() << ", " << target->getPosY() << ")" << endl;
        }
    }

    *battlefield << endl;
}

/*This robot contains 3 drones initially.
The drone can be place at any position inside the battlefield. (The drone cannot be place at the same position)
Each drone can view from its own position and 8 neighboring positions.
The drone is placed until the match ends, or the DroneBot has upgraded to another robot type, or DroneBot has been destroyed.*/
void DroneBot::actionLook (Battlefield* battlefield)
{
    // Perform the normal looking action
    SeeingRobot::actionLook(battlefield);

    int droneLookX, droneLookY;
    // Check the number of drone
    if (droneNumber > 0)
    {
        int randomNumber = rand();
        // To place a drone inside the battlefield
        if (randomNumber % 2 == 0)
        {
            *battlefield << getId() << " is finding a position to place a drone..." << endl;

            int droneX, droneY;
            do {
                droneX = rand() % battlefield->getBATTLEFIELD_NUM_OF_COLS(); // Get the column position of the drone
                droneY = rand() % battlefield->getBATTLEFIELD_NUM_OF_ROWS(); // Get the row position of the drone
            // To ensure the drone will not be place at the same position
            } while (checkDronePosition(droneX, droneY, placedDronePositions));
            placedDronePositions.push_back({droneX, droneY});
            *battlefield << getId() << " placed the drone at (" << droneX << ", " << droneY << ")" << endl;

            droneNumber--; // droneNumber - 1
            *battlefield << getId() << " has " << droneNumber << " drones remaining" << endl;
        }
    }
    else
    {
        *battlefield << getId() << " has no drone left!!" << endl;
    }

    // Check each drone status
    for (const auto& position : placedDronePositions)
    {   // Get the each drone position
        *battlefield << getId() << "'s drone is scanning at (" << position.first << ", " << position.second << ")..." << endl;
        for (int directionCheckEnemy = 0; directionCheckEnemy < 9; directionCheckEnemy++)
        {
            droneLookX = position.first + dx[directionCheckEnemy];
            droneLookY = position.second + dy[directionCheckEnemy];

            // If the look position is valid and found the robot
            if (battlefield->isPositionValid(droneLookX, droneLookY) && battlefield->getRobotAt(droneLookX, droneLookY) != nullptr)
            {
                Robot* droneTarget = battlefield->getRobotAt(droneLookX, droneLookY);
                if (droneTarget != this) // To ensure the target is not the DroneBot itself
                    *battlefield << "=> " << getId() << " spotted " << droneTarget->getId() << " at (" << droneLookX << ", " << droneLookY << ") by drone" << endl;
            }
        }
    }
    *battlefield << endl;
}

/*
The robot can fire up to three unit distance away from its location.
It means the robot can fire(x, y) where x + y <= 3
*/
void LongShotBot::actionFire(Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionFire--" << endl;

    // Generate random direction to shot at (excluding current position)
    int targetX, targetY, shotAtX, shotAtY;

    do {
        shotAtX = (rand() % 7) - 3; // -3, -2, -1, 0, 1, 2, 3
        shotAtY = (rand() % 7) - 3;

        targetX = getPosX() + shotAtX;
        targetY = getPosY() + shotAtY;
    // To ensure that the robot will not fire at its own position and outside the battlefield
    } while ((shotAtX == 0 && shotAtY == 0) || !battlefield->isPositionValid(targetX, targetY)||(abs(shotAtX)+abs(shotAtY)>3));

    Robot* target = battlefield->getRobotAt(targetX, targetY);

    if (target != nullptr && target != this)
    {

        // 70% chance to hit
        if (rand() % 100 < 70)
        {
            // If hit the robot target
            *battlefield << getId() << " hit " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
            incrementKills(); // Number of kills + 1

            // Reduce target's lives
            target->reduceLife();
            battlefield->removeRobot(target);

            // Check if target was destroyed
            if (target->getLives() >= 1)
            {
                battlefield->queueForRespawn(target); // The target enter waiting robot queue
            }
            else
            {
                *battlefield << target->getId() << " was destroyed!" << endl;
                battlefield->destroyRobot(target); // Battlefield handles destruction
            }

            // Check the robot whether is able to get an upgrade or not
            if (getNumUpgrade() < 3)
            {
                setNumUpgrade(getNumUpgrade()+1);
                setIsAbleUpgrade(true); // Set to true if the robot has killed the target
            }
        }
        else
        {
            *battlefield << getId() << " missed " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
        }
    }

    else
    {
        *battlefield << getId() << " fired at empty space (" << targetX << "," << targetY << ")" << endl;
    }

    // Handle ammo and self-destruction
    shellsRemaining--;

    if (shellsRemaining <= 0) {
        *battlefield << getId() << " is out of ammo and self-destructs!" << endl;
        selfDestruct();
        battlefield->removeRobot(this);

        if (this->getLives() >= 1) {

            battlefield->queueForRespawn(this);
        }
        else {
            *battlefield << getId() << " was destroyed!" << endl;
            battlefield->destroyRobot(this); // Use the robot's own selfDestruct method
        }
    }

    *battlefield << endl;
}

/*
Each shell the robot fires is now considered as three consecutive
shots into one location and each shot has a 70% probability to hit and destroy
another robot.
*/
void SemiAutoBot::actionFire(Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionFire--" << endl;

    // Generate random direction to shot at (excluding current position)
    int targetX, targetY, shotAtX, shotAtY;

    do {
        shotAtX = (rand() % 3) - 1; // -1, 0, 1
        shotAtY = (rand() % 3) - 1;

        targetX = getPosX() + shotAtX;
        targetY = getPosY() + shotAtY;
    // To ensure that the robot will not fire at its own position and outside the battlefield
    } while ((shotAtX == 0 && shotAtY == 0) || !battlefield->isPositionValid(targetX, targetY));

    Robot* target = battlefield->getRobotAt(targetX, targetY);

    if (target != nullptr && target != this)
    {

        int randomNum=0;
        bool missedShot = true;

        //Each shell considered as three consecutive shots into one location
        for (int i=0;i<3;i++)
        {
            randomNum = rand()%100;
            if(randomNum<70)
            {
                missedShot == false;
                break;
            }
        }

        // 70% chance to hit
        if (!missedShot)
        {
            // If hit the robot target
            *battlefield << getId() << " hit " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
            incrementKills(); // Number of kills + 1

            // Reduce target's lives
            target->reduceLife();
            battlefield->removeRobot(target);

            // Check if target was destroyed
            if (target->getLives() >= 1)
            {
                battlefield->queueForRespawn(target); // The target enter waiting robot queue
            }
            else
            {
                *battlefield << target->getId() << " was destroyed!" << endl;
                battlefield->destroyRobot(target); // Battlefield handles destruction
            }

            // Check the robot whether is able to get an upgrade or not
            if (getNumUpgrade() < 3)
            {
                setNumUpgrade(getNumUpgrade()+1);
                setIsAbleUpgrade(true); // Set to true if the robot has killed the target
            }
        }
        else
        {
            *battlefield << getId() << " missed " << target->getId() << " at (" << targetX << "," << targetY << ")" << endl;
        }
    }

    else
    {
        *battlefield << getId() << " fired at empty space (" << targetX << "," << targetY << ")" << endl;
    }

    // Handle ammo and self-destruction
    shellsRemaining--;

    if (shellsRemaining <= 0) {
        *battlefield << getId() << " is out of ammo and self-destructs!" << endl;
        selfDestruct();
        battlefield->removeRobot(this);

        if (this->getLives() >= 1) {

            battlefield->queueForRespawn(this);
        }
        else {
            *battlefield << getId() << " was destroyed!" << endl;
            battlefield->destroyRobot(this); // Use the robot's own selfDestruct method
        }
    }
    *battlefield << endl;
}

/*
Instead of shooting, the robot will throw a bomb and hit all targets in 3x3 area.
It has 5 bombs only and it can fire up to 2 distance, it means that the bomb will centered on (x,y)
where x+y<=2.
But it can't throw the bomb around itself, since it get will damaged from it.
*/
void BomberBot::actionFire(Battlefield* battlefield)
{
    *battlefield << "--" << getType() << " actionFire--" << endl;

    int centerX, centerY, shotAtX, shotAtY;
    vector<int> bombLocationX, bombLocationY;

    bool posNotValid;

    do {
        posNotValid = false;

        shotAtX = (rand() % 5) - 2; // Range: -2 to 2
        shotAtY = (rand() % 5) - 2;

        centerX = getPosX() + shotAtX;
        centerY = getPosY() + shotAtY;

        // Avoid firing at or near self
        if ((abs(shotAtX) <= 1 && abs(shotAtY) <= 1)||(!battlefield->isPositionValid(centerX, centerY))||(abs(shotAtX) + abs(shotAtY) >2))
        {
            posNotValid = true;
        }

        // If valid, mark 3x3 bomb impact zone
        if (!posNotValid)
        {
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (battlefield->isPositionValid(centerX + x, centerY + y)) {
                        bombLocationX.push_back(centerX + x);
                        bombLocationY.push_back(centerY + y);
                    }
                }
            }
        }

    } while (posNotValid);

    vector<Robot*> targets;

    for (int i = 0; i < bombLocationX.size(); i++)
    {
        Robot* target = battlefield->getRobotAt(bombLocationX[i], bombLocationY[i]);
        if (target != nullptr) {
            targets.push_back(target);
        }
    }

    string fireSpace;
    for (int i = 0; i < bombLocationX.size(); i++)
    {
        fireSpace += "(" + to_string(bombLocationX[i])+ "," + to_string(bombLocationY[i]) + ")";
        if (i != bombLocationX.size()-1)
            fireSpace += ",";
    }

    if (!targets.empty()) {
        *battlefield << getId() << " throw a bomb centered at ("<< centerX <<"," << centerY << ")" << endl;
        *battlefield << getId() << " fired at " << fireSpace << endl;

        if (rand() % 100 < 70) // 70% hit chance
        {

            for (int i = 0; i < targets.size(); i++) {
                Robot* target = targets[i];
                *battlefield << getId() << " hit " << target->getId()
                     << " at (" << target->getPosX() << "," << target->getPosY() << ")" << endl;

                incrementKills();
                target->reduceLife();
                battlefield->removeRobot(target);

                if (target->getLives() >= 1) {
                    battlefield->queueForRespawn(target);
                } else {
                    *battlefield << target->getId() << " was destroyed!" << endl;
                    battlefield->destroyRobot(target);
                }
            }

            // Check the robot whether is able to get an upgrade or not
            if (getNumUpgrade() < 3)
            {
                setNumUpgrade(getNumUpgrade()+1);
                setIsAbleUpgrade(true); // Set to true if the robot has killed the target
            }
        }
        else
        {
            for (Robot* target : targets) {
                *battlefield << getId() << " missed " << target->getId()
                     << " at (" << target->getPosX()<< "," << target->getPosY() << ")" << endl;
            }
        }
    } else {

        *battlefield << getId() << " throw a bomb at empty space centered at ("<< centerX <<"," << centerY << ")" << endl;
        *battlefield << getId() << " fired at" << fireSpace << endl;

    }

    // Ammo handling
    shellsRemaining--;
    if (shellsRemaining <= 0) {
        *battlefield << getId() << " is out of ammo and self-destructs!" << endl;
        selfDestruct();
        battlefield->removeRobot(this);

        if (getLives() >= 1) {
            battlefield->queueForRespawn(this);
        } else {
            *battlefield << getId() << " was destroyed!" << endl;
            battlefield->destroyRobot(this);
        }
    }

    *battlefield << endl;
}

/*
The robot has a skill to hide when actionMove.
The hiding skill is based on chances and nearby enemies.
If there are enemies nearby, the chances will be 70% to hide, otherwise it will be 30% to hide.
If it fails to hide because it ran out of skills or chances fail, the robot will move normally.
The robot will hide until it is it's turn again, and it can hide continuously.
Once in hiding, the robot cannot be shot at or seen by enemies, also note that enemies cannot move to that space.
*/
void HideBot::actionMove(Battlefield* battlefield)
{
    //If HideBot is in hiding, unhide it
    if (isHidden)
    {
        isHidden = false;
        *battlefield << getId() << " is now visible again." << endl;
    }

    if (hideSkill > 0)
    {
        //Check if any enemies nearby
        bool enemyNearby = false;

        for (int i = 0; i < 8; ++i)
        {
            if (hasEnemy[i])
            {
                enemyNearby = true;
                break;
            }
        }

        //If enemy nearby, 70% to activate skill, else 30% to activate skill
        if ((enemyNearby ? rand() % 100 < 70 : rand() % 100 < 30)) {

            *battlefield << "--" << getType() << " actionHide--" << endl;

            isHidden = true;

            hideSkill--;

            *battlefield << getId() << " went into hiding." << endl;
            *battlefield << endl;
            return;

        }
        else
        {
            *battlefield << getType() << " actionHide" << endl;

            *battlefield << getId() << " failed to hide and moves normally." << endl;
        }
    }
    else
    {
        *battlefield << getId() << " has no hides left!!" << endl;
    }

    //If no more jumpSkill or chances fail or failed to hide, move normally
    MovingRobot::actionMove(battlefield);
    *battlefield << endl;
}

/*
The robot has a skill to jump when actionMove.
The jump skill is based on chances and nearby enemies.
If there are enemies nearby, the chances will be 70% to jump, otherwise it will be 30% to jump.
The robot can jump continuously and to a random location on the battlefield, and cannot jump on occupied spaces.
*/
void JumpBot::actionMove(Battlefield* battlefield)
{

    if (jumpSkill > 0)
    {
        //Check if any enemies nearby
        bool enemyNearby = false;

        for (int i = 0; i < 8; ++i)
        {
            if (hasEnemy[i])
            {
                enemyNearby = true;
                break;
            }
        }

        //If enemy nearby, 70% to activate skill, else 30% to activate skill
        if ((enemyNearby ? rand() % 100 < 70 : rand() % 100 < 30)) {
            *battlefield << "--" << getType() << " actionJump--" << endl;

            int newX, newY;
            do {
                newX = rand() % battlefield->getBATTLEFIELD_NUM_OF_COLS();
                newY = rand() % battlefield->getBATTLEFIELD_NUM_OF_ROWS();
            } while (!battlefield->isPositionEmpty(newX, newY));

            setPosX(newX);
            setPosY(newY);
            battlefield->placeRobots();

            *battlefield << getId() << " jumps to (" << newX << "," << newY << ")" << endl;
            *battlefield << endl;
            return;
        }

        else
        {
            *battlefield << getId() << " failed to jump and moves normally" << endl;
        }
    }
    else
    {
        *battlefield << getId() << " has no jumps left!!" << endl;
    }

    //If no more jumpSkill, chances fail, failed to jump move normally
    MovingRobot::actionMove(battlefield);
    *battlefield << endl;
}

/*
The robot has a skill to create a portal when actionMove.
The portal skill is based on chances and nearby enemies.
If there are enemies nearby, the chances will be 70% to portal, otherwise it will be 30% to portal.
The portal the robot creates allows that robot to swap places with a random robot on the battlefield.
If there are enemies nearby, the robot will prioritise other robots that are not nearby to portal to,
if the nearby enemies are the only robots in the battlefield, then it will choose to portal to them.
*/
void PortalBot::actionMove(Battlefield* battlefield)
{

    if (portalSkill > 0)
    {
        //Check if any enemies nearby
        bool enemyNearby = false;
        int numOfenemyNearby = 0;

        for (int i = 0; i < 8; ++i)
        {
            if (hasEnemy[i])
            {
                enemyNearby = true;
                numOfenemyNearby++;
            }
        }

        //If enemy nearby, 70% to activate skill, else 30% to activate skill
        if ((enemyNearby ? rand() % 100 < 70 : rand() % 100 < 30))
        {
            *battlefield << "--" << getType() << " actionPortal--" << endl;

            //Target to portal and swap places with
            vector<Robot*> portalTarget;

            for (Robot* otherRobots : battlefield->getRobots())
            {
                //Make sure the target is not itself and also alive
                if (otherRobots != this && otherRobots->isAlive())
                {
                    portalTarget.push_back(otherRobots);
                }
            }

            //Choose a random target and swap places
            Robot* target = nullptr;

            //Avoid swapping places with nearby enemy, if possible
            if (enemyNearby && portalTarget.size() == 1 && numOfenemyNearby == 1)
            {
                Robot* target = portalTarget.front();
            }
            else if (enemyNearby && portalTarget.size() > numOfenemyNearby)
            {
                // Multiple teleport targets available, avoid nearby enemies
                vector<Robot*> safeTargets;

                for (Robot* candidate : portalTarget)
                {
                    bool nearEnemy = false;

                    // Check 8 directions around for enemy's position
                    for (int dir = 0; dir < 8; ++dir)
                    {
                        int checkX = candidate->getPosX() + dx[dir];
                        int checkY = candidate->getPosY() + dy[dir];

                        Robot* nearby = battlefield->getRobotAt(checkX, checkY);

                        if (nearby != nullptr && nearby != this)
                        {
                            nearEnemy = true;
                            break;
                        }
                    }

                    if (!nearEnemy) safeTargets.push_back(candidate);
                }

                if (!safeTargets.empty())
                {
                    target = safeTargets[rand() % safeTargets.size()];
                }
                else {
                    //Fallback to any available
                    target = portalTarget[rand() % portalTarget.size()];
                }
            }

            else if (!portalTarget.empty())
            {
                // No enemy nearby or no filtering needed
                target = portalTarget[rand() % portalTarget.size()];
            }

            //Swap places with target
            int myX = getPosX();
            int myY = getPosY();

            int targetX = target->getPosX();
            int targetY = target->getPosY();

            setPosX(targetX);
            setPosY(targetY);
            target->setPosX(myX);
            target->setPosY(myY);

            *battlefield << getId() << " portal and swaps with " << target->getId() << " to position (" << targetX << ", " << targetY << ")" << endl;
            *battlefield << endl;
            return;

        }
        else
        {
            *battlefield << getId() << " failed to portal and moves normally" << endl;
        }

    }
    else
    {
        *battlefield << getId() << " has no portals left!!" << endl;
    }

    //If no more portalSkill or chances fail or failed to portal, move normally
    MovingRobot::actionMove(battlefield);
    *battlefield << endl;
}

int main()
{
    // Fixed Seed Random Integer
    srand(242213244718 / 100); //Leader ID = 242UC244GR, U=21,C=3,G=7,R=18

    Battlefield b;
    b.writeOutputFile("fileOutput1.txt");
    b.readInputFile("fileInput1.txt");
    b.placeRobots();
    b.displayBattleField();
    b.turnBased();
    b.closeOutputFile();

    return 0;
}
