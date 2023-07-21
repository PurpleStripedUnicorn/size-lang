
#include <fstream>
#include <iostream>
#include <vector>

// Show parsed input and running instructions
constexpr bool DEBUG_MODE = false;



/**
 * Object to keep track of variable values, all values are initialized at zero
 */
struct Vars {
    std::vector<char> vals;
    char get(size_t i) {
        return i >= vals.size() ? 0 : vals[i];
    }
    void expand(size_t i) {
        while (vals.size() <= i)
            vals.push_back(0);
    }
    void set(size_t i, size_t val) {
        expand(i);
        vals[i] = val;
    }
    void increment(size_t i) {
        expand(i);
        vals[i]++;
    }
    void decrement(size_t i) {
        expand(i);
        vals[i]--;
    }
};



/**
 * Instruction types
 */
enum InstructionType {
    INSTR_PRINT, INSTR_INPUT, INSTR_INC, INSTR_DEC, INSTR_JUMP,
};



/**
 * Parsed instruction
 */
struct Instruction {
    InstructionType type;
    size_t paramA = 0, paramB = 0;

    /**
     * For debug output
     */
    friend std::ostream &operator<<(std::ostream &os, const Instruction &instr)
    {
        switch (instr.type) {
            case INSTR_PRINT:
                os << "print  " << int(instr.paramA) << std::endl;
                break;
            case INSTR_INPUT:
                os << "input  " << int(instr.paramA) << std::endl;
                break;
            case INSTR_INC:
                os << "inc    " << int(instr.paramA) << std::endl;
                break;
            case INSTR_DEC:
                os << "dec    " << int(instr.paramA) << std::endl;
                break;
            case INSTR_JUMP:
                os << "jump   " << int(instr.paramA) << ", " <<
                int(instr.paramB) << std::endl;
                break;
        }
        return os;
    }
};



/**
 * Object that actually parses the input
 */
class Parser {

public:

    /**
     * Constructor
     * @param inp The input to be parsed
     */
    Parser(size_t inp) : inp(inp), mask(0) { }

    /**
     * Run the parser
     */
    void run() {
        moveToFirst();
        while (!atEnd()) {
            bool first = cur();
            next();
            bool second = cur();
            next();
            if (first) {
                if (second) {
                    // Jump
                    size_t vA = readVar(), vB = readVar();
                    instructions.push_back({INSTR_JUMP, vA, vB});
                } else {
                    bool third = cur();
                    next();
                    if (third) {
                        // Decrement
                        size_t v = readVar();
                        instructions.push_back({INSTR_DEC, v});
                    } else {
                        // Increment
                        size_t v = readVar();
                        instructions.push_back({INSTR_INC, v});
                    }
                }
            } else {
                if (second) {
                    // Input
                    size_t v = readVar();
                    instructions.push_back({INSTR_INPUT, v});
                } else {
                    // Print
                    size_t v = readVar();
                    instructions.push_back({INSTR_PRINT, v});
                }
            }
        }
        if (DEBUG_MODE)
            debugOutput();
    }

    /**
     * Get the output list of instructions
     * @return A constant reference to the list of instructions
     */
    const std::vector<Instruction> &getInstructions() const {
        return instructions;
    }

private:

    /**
     * Move to the first position in the input by adjusting mask, ignores the
     * first `1` in the input
     */
    void moveToFirst() {
        mask = ~((~size_t(0)) >> 1);
        while (!(mask & inp))
            mask >>= 1;
        mask >>= 1;
    }

    /**
     * Check if the current bit is set
     * @return A boolean indicating if the bit is set
     */
    bool cur() const {
        if (atEnd())
            throw std::runtime_error("Reading past end of input");
        return mask & inp;
    }

    /**
     * Move to next bit in input
     */
    void next() {
        mask >>= 1;
    }

    /**
     * Check if we are at the end of the input
     * @return A boolean indicating if we are reading past the end
     */
    bool atEnd() const {
        return !mask;
    }

    /**
     * Read a variable bitstring
     * @return The variable index
     */
    size_t readVar() {
        size_t i = 0;
        while (cur()) {
            i++;
            next();
        }
        next();
        return i;
    }

    /**
     * Debug output the parsed instructions
     */
    void debugOutput() const {
        std::cerr << "Parsed instructions:" << std::endl;
        for (const Instruction &instr : instructions)
            std::cerr << instr;
    }

    // Input
    size_t inp;
    // Current input mask
    size_t mask;
    // Output list of instructions
    std::vector<Instruction> instructions;

};



/**
 * Used to execute the parsed list of instructions
 */
class Runner {

public:

    /**
     * Constructor
     * @param instructions A constant reference to the parsed list of
     * instructions
     */
    Runner(const std::vector<Instruction> &instructions) :
    instructions(instructions) { }

    /**
     * Run the instructions
     */
    void run() {
        long long ptr = 0;
        if (DEBUG_MODE)
            std::cerr << "Executing instructions:" << std::endl;
        while (0 <= ptr && ptr < (long long)(instructions.size())) {
            const Instruction &instr = instructions.at(ptr);
            if (DEBUG_MODE)
                std::cerr << instr;
            char tmp;
            switch (instr.type) {
                case INSTR_PRINT:
                    std::cout << vars.get(instr.paramA) << std::flush;
                    break;
                case INSTR_INPUT:
                    std::cin >> tmp;
                    vars.set(instr.paramA, tmp);
                    break;
                case INSTR_INC:
                    vars.increment(instr.paramA);
                    break;
                case INSTR_DEC:
                    vars.decrement(instr.paramA);
                    break;
                case INSTR_JUMP:
                    if (!vars.get(instr.paramA)) {
                        ptr += vars.get(instr.paramB);
                        ptr--;
                    }
                    break;
            }
            ptr++;
        }
    }

private:

    // List of instructions
    const std::vector<Instruction> &instructions;
    // Variable values
    Vars vars;

};



int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Could not open file \"" << argv[1] << "\"" << std::endl;
        return 2;
    }
    size_t sz = 0;
    char tmp;
    while (file >> tmp)
        sz++;
    file.close();
    if (sz == 0) {
        std::cerr << "Given file \"" << argv[1] << "\" has no content" <<
        std::endl;
    }
    if (DEBUG_MODE)
        std::cerr << "Input: " << sz << std::endl;
    Parser parser(sz);
    parser.run();
    Runner runner(parser.getInstructions());
    runner.run();
    return 0;
}