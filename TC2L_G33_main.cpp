// *********************************************************
// Program: TC2L_G33_main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC2L
// Tutorial Class: TT7L
// Trimester: 2510
// Member 1: 242UC244GR | Lee Xiu Wei    | LEE.XIU.WEI@student.mmu.edu.my    | 017-9218088
// Member 2: 243UC24672 | Lai Seng Kung  | lai.seng.kung1@student.mmu.edu.my | 012-9169228
// Member 3: 242UC244S3 | Teng Ming Hein | TENG.MING.HEIN@student.mmu.edu.my | 016-7831558
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

// Seng Kung's part: Generic Robot...

int main()
{


    return 0;
}
