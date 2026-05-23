# 🤖 Robot War Simulator

A C++ console application that simulates the warfare of robots in a given battlefield. Built with standard C++ and object-oriented design principles, this simulator demonstrates combat mechanics, AI decision-making, and real-time battlefield updates.

---

## ✨ Features

- 🎮 **Robot Combat Simulation** — Multiple robots engage in strategic warfare on a dynamic battlefield
- 🧠 **AI-Driven Movement** — Robots make intelligent decisions based on opponent positions and resources
- 💥 **Battle Mechanics** — Attack, defense, and damage calculation systems
- 📊 **Real-Time Updates** — Live battlefield state visualization and status tracking
- ⚙️ **Configurable Scenarios** — Customize robot count, battlefield size, and initial conditions
- 🏆 **Victory Conditions** — Automatic winner detection and battle statistics

---

## 📋 Prerequisites

- **C++ Compiler** — GCC, Clang, or MSVC with C++11 standard or higher
- **Standard Library** — STL support (vector, map, iostream, etc.)
- **Build Tool** (optional) — Make, CMake, or your preferred build system

---

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone <repository-url>
cd robot_war_oppds
```

### 2. Compile the project

Using g++:

```bash
g++ -std=c++11 -o robot_war src/*.cpp
```

Or with Make (if Makefile exists):

```bash
make
```

### 3. Run the simulator

```bash
./robot_war
```

### 4. View the battle simulation

The program will display:
- Initial robot positions and stats
- Turn-by-turn battle actions
- Battlefield state after each turn
- Final results and statistics

---

## 📁 Project Structure

```
robot_war_oppds/
├── src/
│   ├── main.cpp              — Entry point for the simulator
│   ├── robot.cpp             — Robot class implementation
│   ├── robot.h               — Robot class definition
│   ├── battlefield.cpp       — Battlefield class implementation
│   ├── battlefield.h         — Battlefield class definition
│   └── util.cpp              — Utility functions
├── include/
│   └── config.h              — Configuration constants and enums
├── Makefile                  — Build configuration (optional)
└── README.md
```

---

## 🏗️ Design Patterns

| Pattern | Usage |
|---|---|
| **Object-Oriented Design** | Robot and Battlefield as core classes encapsulating state and behavior |
| **Encapsulation** | Private attributes with public getter/setter methods for controlled access |
| **Inheritance** (if applicable) | Specialized robot types (e.g., Tank, Scout) inheriting from base Robot class |
| **Simulation Loop** | Main game loop handling turn-based combat and state updates |
| **Vector/Container Usage** | STL vectors for managing dynamic robot collections |

---

## 🎮 How to Play / Simulate

1. **Launch the program** — Robots are initialized on the battlefield
2. **Observe each turn** — Robots take actions based on AI logic
3. **Monitor battle progress** — Watch health, ammo, and position changes
4. **Wait for results** — Battle concludes when a victory condition is met

---

## 🤝 Contributions

- Lee Xiu Wei
- Teng Ming Hein
- Lai Seng Kung

The development of this project is distributed even between these 3 people mentioned.

