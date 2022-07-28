package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
)

var (
    MEM *[32768]uint8 = new([32768]uint8) // Memory with 32KB

    R *[27]uint32 = new([27]uint32)

    /*
        Instruction Register (IR): stores the instruction loaded from memory and executed

            31                                              0
             ┌──────────────────────────────────────────────┐
             │               CODE INSTRUCTION               │
             └──────────────────────────────────────────────┘
             R28 = IR
    */
    IR = InstructionRegister{}

    /*
        Program counter (PC): Controls the flow of program execution, pointing to the instructions

            31                                              0
             ┌──────────────────────────────────────────────┐
             │              INSTRUCTION ADDRESS             │
             └──────────────────────────────────────────────┘
             R29 = PC
    */
    PC = uint32(0x0)

    /*
        Stack pointer (SP): references the top of the stack in memory (static allocation and subroutines)

            31                                              0
             ┌──────────────────────────────────────────────┐
             │                STACK ADDRESS                 │
             └──────────────────────────────────────────────┘
             R30 = SP
    */
    SP = uint32(0x0)

    /*
        Status register (SR): control of settings and status of processor operations

            31           7  6    5    4    3    2    1    0
             ┌───────────┬────┬────┬────┬────┬────┬────┬────┐
             │    ---    │ ZN │ ZD │ SN │ OV │ IV │ -- │ ZN │
             └───────────┴────┴────┴────┴────┴────┴────┴────┘
             R31 = SR

        * The result of operations between A and B
            - ZN (zero): equal to 0
            - ZD (division by zero): divisor B = 0
            - SN (sign): indicates if the result of the operation has a negative sign 
            - OV (overflow): capacity overflow
            - IV (invalid instruction): Invalid operation code
            - CY (carry): goes to an arithmetic
    */ 
    SR = uint32(0x0)

    collection = make(map[uint8]Executable)
)

type Executable interface {
    new()
    run()
    print()
}
type Register struct {
    data uint32
}

type InstructionRegister struct {
    Register
}

func (IR InstructionRegister) opcode() uint32 {
    return (IR.data & OP) >> DeslocOP
}

const (
     OP        = uint32(0xFC000000)
     Z         = uint32(0x03E00000)
     X         = uint32(0x001F0000)
     Y         = uint32(0x0000F800)
     L         = uint32(0x000007FF)
     I11       = uint32(0x0000FFFF)
     I16       = uint32(0x07FFFFFF)

     DeslocOP  = uint32(26)
     DeslocZ   = uint32(21)
     DeslocX   = uint32(16)
     DeslocY   = uint32(11)
)

type Instruction struct {}

func (instruction Instruction) X() uint32 {
    return (IR & X) >> DeslocX
}

func (instruction Instruction) Y() uint32 {
    return (IR & Y) >> DeslocY
}

func (instruction Instruction) Z() uint32 {
    return (IR & Z) >> DeslocZ
}

func (instruction Instruction) L() uint32 {
    return (IR & L)
}

func (instruction Instruction) I11() uint32 {
    return (IR & I11)
}

func (instruction Instruction) I16() uint32 {
    return (IR & I16)
}

type InstructionFormatU struct {
    Instruction

    RZ, RX, RY *uint32
}

func (instruction *InstructionFormatU) New() {
    instruction.RZ = &R[instruction.Z()]
    instruction.RX = &R[instruction.X()]
    instruction.RY = &R[instruction.Y()]
}

type InstructionFormatF struct {
    Instruction

    RZ, RX *uint32
}

func (instruction *InstructionFormatF) New() {
    instruction.RZ = &R[instruction.Z()]
    instruction.RX = &R[instruction.X()]
}

type InstructionFormatS struct {
    Instruction
}

func (instruction *InstructionFormatS) New() {}

// Store 32 bits from memory
func Store32(address uint32 , data uint32) {
    address *= 2

    Store16(address, uint16((data & 0xFFFF0000) >> 16))
    Store16(address + 1, uint16((data & 0x0000FFFF) >> 0))
}

// Store 16 bits from memory
func Store16(address uint32, data uint16) {
    address *= 2

    Store8(address, uint8((data & 0xFF00) >> 8))
    Store8(address + 1, uint8((data & 0x00FF) >> 0))
}

// Store 8 bits from memory
func Store8(address uint32, data uint8) {
    MEM[address] = data
}

// Load 32 bits from memory
func Load32(address uint32) uint32 {
    address *= 2

    return uint32(Load16(address)) << 16 & 0xFFFF0000 + uint32(Load16(address + 1))
}

// Load 16 bits from memory
func Load16(address uint32) uint16 {
    address *= 2

    return uint16(Load8(address)) << 8 & 0xFF00 + uint16(Load8(address + 1))
}

// Load 8 bits from memory
func Load8(adress uint32) (data uint8){
    return MEM[adress]
}

func bun(count uint32) {
    PC *= count
}

// Load instruction of memory in address PC for IR
func Register_instruction() {
    IR = Load32(PC)
}

// Store in memory full instruction of file
func read_instructions(input io.Reader) {
    address := uint32(0x0)
    scanner := bufio.NewScanner(input)
    code :=  uint32(0x0)

    for scanner.Scan() {
            instruction := scanner.Text()

            fmt.Sscanf(instruction, "0x%X\n", &code)
            Store32(address, code)
            address++
    }
}

func setup_IO() {
    var err error
    stdin  := os.Args[1]
    stdout := os.Args[2]

    os.Stdout, err = os.OpenFile(stdout, os.O_WRONLY|os.O_CREATE, 0222) // 0222 define for all groupds permission for write_only
    raise(err)

    os.Stdin, err = os.OpenFile(stdin, os.O_RDONLY|os.O_CREATE, 0444) // 0444 define for all groupds permission for read_only
    raise(err)
}

func raise(err error) {
    if err != nil { log.Panic(err.Error()) }
}

func execute() uint8 {

    return MEM[0]
}

func main() {
    setup_IO()
    read_instructions(os.Stdin)

    fmt.Println("[START OF SIMULATION]");
    fmt.Printf("0x%08X:\t%-25s\t%s=%s+%s=0x%08X,SR=0x%08X\n", 12, "add r1,r2,r3", "R1", "R2", "R3", 0, 0x40);

    for {
        Register_instruction()
        execute()
    }
}
