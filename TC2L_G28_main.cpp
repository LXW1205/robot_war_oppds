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
    //data member
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
            actionThink (battlefield);
            actionLook (battlefield);
            actionFire (battlefield);
            actionMove(battlefield);
        }
        else if(randomInt % 2 == 1)
        {
            actionThink (battlefield);
            actionLook (battlefield);
            actionMove(battlefield);
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

    vector<Robot*> robots;
    queue<Robot*> destroyedRobots_;
    queue<Robot*> waitingRobots__;

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
    int getNumOfRobots ()
    {
        return numOfRobots;
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

                robotInfoStream >> type >> idName >> posY >> posX;

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
            if (robots[i]->getPosX() < battlefield.size() && robots[i]->getPosY() < battlefield[0].size())
            {
                battlefield[robots[i]->getPosX()][robots[i]->getPosY()] = robots[i]->getId();
            }
            else
            {
                cerr << "Error message: Invalid location for the robot" << robots[i]->getId() << endl;
                exit(1);
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
};

void GenericRobot::actionThink (Battlefield* battlefield)
{
    cout << "GenericRobot actionThink" << endl;
}
void GenericRobot::actionLook (Battlefield* battlefield)
{
    cout<<"GenericRobot actionLook" << endl;
}

void GenericRobot::actionFire (Battlefield* battlefield)
{
    cout << "GenericRobot actionFire" << endl;
}
void GenericRobot::actionMove (Battlefield* battlefield)
{
    cout<<"GenericRobot actionMove" << endl;
}

int main()
{
    srand(242213244718 / 100); //Leader ID = 242UC244GR, U=21,C=3,G=7,R=18

    Battlefield b;
    b.readInputFile("inputFile1.txt");
    b.placeRobots();
    b.displayBattleField();

    return 0;
}

