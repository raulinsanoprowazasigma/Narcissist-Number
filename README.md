# Narcissist Number - Road to 153

An animated visualization, built in C++ with SFML of a curious property of the number **153**:
take any **positive multiple of 3**, repeatedly replace it with the **sum of the cubes of its digits**,
and the result will *always* collapse into 153 (where it stays forever).

## What's behind it

153 is a **narcissistic number** (also called an Armstrong number): it equals the sum of the cubes of its own digits.


1³ + 5³ + 3³ = 1 + 125 + 27 = 153


Because of this, 153 is a **fixed point** of the operation "sum of the cubes of the digits": once you reach it,
applying the operation again gives 153 back. The beautiful thing is that this fixed point acts like a magnet
for every positive multiple of 3. For example, starting from 3:


3  →  3³ = 27
27 →  2³ + 7³ = 351
351 → 3³ + 5³ + 1³ = 153   ✔


Note that the property is only guaranteed for **positive** multiples of 3. The value 0 is a multiple
of 3 but it maps to itself (0³ = 0), so it never reaches 153 and is rejected by the program.


This program **demonstrates** the property for a given number, step by step. It is not a formal *proof* of the
general theorem. It verifies and visualizes one case at a time.

## What the program does

You type a number directly inside the window and press **Enter**. The program then animates the chain of
iterations until it lands on 153, with an iteration counter in the top left corner.

Each iteration plays out in four phases:

1. The current number is shown, held together for a moment.
2. Its digits separate, while the '+' signs and the cube exponents ('³') fade in, forming an expression like '3³ + 5³ + 1³'.
3. The full expression is held still so it can be read.
4. Everything converges toward the center and merges into the resulting number.

This repeats, feeding each result back as the next input, until 153 is reached.

## Controls

| Key       | Action                                              |
|-----------|-----------------------------------------------------|
| Digits    | Type a number (positive multiples of 3 only)        |
| Backspace | Delete the last digit                               |
| Enter     | Start the animation / restart with a new number     |
| Escape    | Return to the input screen at any time              |

## Requirements

- **SFML 3.0.x** (the event API differs from SFML 2; code written for SFML 2 will not compile)
- A **C++17** compiler
- A .ttf font available at runtime — the program tries arial.ttf next to the executable first,
  then falls back to C:/Windows/Fonts/arial.ttf

## Building (Windows · Visual Studio · vcpkg)

1. Install SFML through vcpkg: https://github.com/microsoft/vcpkg:

   ```bash
   vcpkg install sfml
   vcpkg integrate install
   ```

2. Open the project in Visual Studio and set, in **Project → Properties → Configuration Properties → C/C++ → Language**,
   the **C++ Language Standard** to **ISO C++17 (/std:c++17)**.
3. Set the build platform to **x64** (vcpkg installs the x64 build by default).
4. Build with **Ctrl + Shift + B** and run with **F5**.

Any other setup works too, as long as it provides a C++17 compiler linked against SFML 3.


## Author
Raúl Alemán Rubiales

## License
Released under the MIT License.
