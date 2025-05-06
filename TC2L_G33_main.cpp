// *********************************************************
// Program: TC2L_G33_main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC2L
// Tutorial Class: TT7L
// Trimester: 2510
// Member 1: 242UC244GR | Lee Xiu Wei    | LEE.XIU.WEI@student.mmu.edu.my    | 017-9218088
// Member 2: 243UC24672 | Lai Seng Kung  | lai.seng.kung1@student.mmu.edu.my | 012-9169228
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

// Ming Hein's part: Robot base class
// Base Abstract Class
class Robot
{
private:
    string robotId;
    string robotName;
    string robotType;

    int robotPositionX;
    int robotPositionY;

    int numberOfLives = 3; // Initial value
    int numberOfKills = 0; // Initial value

public:
    // Parameterized Constructor(PC)
    Robot(string id, int posX, int posY) { id = ""; posX = -1; posY = -1; } // Set the initial default value

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
};

// Xiu Wei's part: Robot's actions...

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
    GenericRobot (string id = "", int x = -1, int y = -1)
    {
        id = id;
        robotPositionX = x;
        robotPositionY = y;

        robotAutoIncrementInt_++;
    }

    static int robotAuto IncrementInt()
    {
        return robotAuto IncrementInt_;
    }

    virtual GenericRobot() {}

    virtual void setLocation(int x, int y)
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
int GenericRobot::robotAutoIncrementInt = 0;


class Battlefield
{
private:
    int BATTLEFIELD_NUM_OF_COLS_ = -1;
    int BATTLEFIELD_NUM_OF_ROWS_ = -1;

    int turns_ = -1;         // Total number of turns
    int turn = 0;           // Current turn number

    int numOfRobots_ = -1; // Number of robots

    vector<Robot*> robots_;
    queue<Robot*> destroyedRobots_;
    queue<Robot*> waitingRobots__;

    vector<vector<string>> battlefield_; // 2D vector representing the battlefield
public:
    // Getter functions
    int BATTLEFIELD_NUM_OF_COLS()
    {
        return BATTLEFIELD_NUM_OF_COLS_;
    }
    int BATTLEFIELD_NUM_OF_ROWS ()
    {
        return BATTLEFIELD_NUM_OF_ROWS_;
    }
    int turns()
    {
        return turns_;
    }
    int numOfRobots ()
    {
        return numOfRobots_;
    }
    // Read input file to initialize battlefield and robots
    void readFile(string filename)
    {
    }

    // Place robots on the battlefield
    void placeRobots()
    {
        for (int i=0; i<battlefield_.size(); ++i)
        {
            for (int j=0; j<battlefield_[i].size(); ++j)
            {
                battlefield_[i][j] = "";
            }
        }

        for (int i=0; i<robots_.size(); ++i)
        {
            if (robots_[i]->y() < battlefield_.size() && robots_[i]->x() < battlefield_[0].size())
            {
                battlefield_[robots_[i]->y()] [robots_[i]->x()] = robots_[i]->id();
            }
            else
            {

                cout<< "Error message: Invalid location for the robot" << robot_[i]->id() << endl;
                exit(1);
            }
        }
    }

        // Display the battlefield in the screen
        void displayBattlefield() const
        {
            cout<<"Display Battlefield";
            cout <<endl<<" ";
            for (int j = 0; j < battlefield_[0].size(); ++j)
            {
                cout << "  "<< right << setfill('0') << setw(2) <<j<< " ";
            }

            cout << endl;

            for (int i = 0; i < battlefield_.size(); ++i)
            {
                cout << "    ";
                for (int j = 0; j < battlefield_[i].size(); ++j)
                    cout<<"+----";
                cout<< "+" << endl;

                cout << " "<< right << setfill('0') << setw(2) << i;
                for (int j=0; j < battlefield_[0].size(); ++j)
                {
                    if (battlefield_[i][j] == "")
                    {
                        cout << "|" << "    ";
                    }
                    else
                    {
                        cout << "|" << left <<< setfill() <<< setw(4) << battlefield_[i][j];
                    }
                }
                cout << "|" << endl;
            }

            cout << "   ";
            for (int j =0; j< battlefield_[0].size(); ++j)
                cout<<"+----";
            cout << "+" << endl;
        }
    };
    void GenericRobot::actionThink (Battlefield battlefield)
    {
        cout << "GenericRobot actionThink" << endl;
    }
    void GenericRobot::actionLook (Battlefield battlefield)
    {
        cout<<"GenericRobot actionLook" << endl;
    }

    void GenericRobot::actionFire (Battlefield* battlefield)
    {
        cout << "GenericRobot actionFire" << endl;
    }
    void GenericRobot::actionMove (Battlefield battlefield)
    {
        cout<<"GenericRobot actionMove" << endl;
    }

    int main()
    {
        srand(242213244718); //Leader ID = 242UC244GR, U=21,C=3,G=7,R=18

        Battlefield battlefield;
        Robot* robotGenericRobot = new GenericRobot("GR01", 4, 4);
        cout << * robotGenericRobot << endl;
        robotGenericRobot->actions(&battlefield);
        delete robotGenericRobot;
        robotGenericRobot = nullptr;
        return 0;
    }

