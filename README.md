
# Size-lang

An esoteric programming language that only uses the number of characters in the input file :)

## How it works

The interpreter counts the number of characters in the input file. This number of characters is then converted to a sequence of bits based on little-endian notation. For example, if the input file contains the string `jfueor` (excluding any EOF chararacter) which is 6 characters, the input is `110`, which is 6 in binary.

### Program structure

The binary string is then read from start to end as a sequence of commands, each having a unique starting bit sequence. For example, to print something to the console the bits `00` can be used. After these starting bits more information can be passed to the command, for example about variables. The first bit in the input should be `1` and is ignored by the interpreter.

### Variables

Size-lang can keep track of any amount of variables. All variables are initialized at zero at the start of the program. When a variable is referenced it should be represented as a certain number of `1` bits, followed by a single `0`. The first few variables are `0`, `10`, `110`, `1110`, etc.

### Instructions

A full list of instructions can be found below:

| Starting bits | Instruction | Description | Usage |
| --- | --- | --- | --- |
| `00` | Print | Print a character to the console. | `00[var]` |
| `01` | Input | Set a variable value given by the user (given as an ascii character). | `01[var]` |
| `100` | Increment | Increment a variable by 1. | `100[var]` |
| `101` | Decrement | Decrement a variable by 1. | `101[var]` |
| `11` | Conditional jump | If the given variable is zero, move to the left (negative) or right (positive) a given amount of instructions, stored in a variable. When jumping to a position outside the program, the program is terminated. | `11[compvar][jumpvar]` |

### Example program: Next character

We can ask a user for a character as input and then print the next character in ascii to the console. This is representated with the following binary string:
```
1       # Program start
01 0    # Ask for user input and store this is variable 0
100 0   # Increment variable 0
00 0    # Print variable 0
```
The input binary string is `10101000000`, which is 1344 in decimal. Hence an input file with 1344 characters should be created to run this program.

### Example program: Infinite letter

The following example prints all ascii characters, except null.
```
1           # Program start
01 0        # Put input letter in variable 0
101 10      # Decrement variable 1 to -1
00 0        # Print value of variable 0
11 110 10  # When variable 2 is zero (always) go back 1 instruction (value of variable 1)
```
This binary value is 350330 in decimal. Hence an input file with 350330 characters should be created to run this program. This can be automated with the `filefiller` script.

### How to write more complex programs

To write more complex programs, buy more disk space.
