package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"math/bits"
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

    SR = StatusRegister{}

    collection = make(map[uint8]Executable)
)

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

type Executable interface {
    new()
    run()
    print()
}

type Register struct {
    Data uint32
}

/*
    Status register (SR): control of settings and status of processor operations

        31           7  6    5    4    3    2    1    0
         ┌───────────┬────┬────┬────┬────┬────┬────┬────┐
         │    ---    │ ZN │ ZD │ SN │ OV │ IV │ -- │ CY │
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
type StatusRegister struct {
    Register
}

// ZN (zero): equal to 0
func (SR *StatusRegister) ZN() {
    SR.Data |= 0x40 
}

// ZD (division by zero): divisor B = 0
func (SR *StatusRegister) ZD() {
    SR.Data |= 0x20 
}

// SN (sign): indicates if the result of the operation has a negative sign 
func (SR *StatusRegister) SN() {
    SR.Data |= 0x10 
}

// OV (overflow): capacity overflow
func (SR *StatusRegister) OV() {
    SR.Data |= 0x8 
}

// IV (invalid instruction): Invalid operation code
func (SR *StatusRegister) IV() {
    SR.Data |= 0x4
}

// CY (carry): goes to an arithmetic
func (SR *StatusRegister) CY() {
    SR.Data |= 0x1
}

type InstructionRegister struct {
    Register
}

func (IR InstructionRegister) Opcode() uint8 {
    return uint8((IR.Data & OP) >> DeslocOP)
}

// Load instruction of memory in address PC for IR
func (IR *InstructionRegister) Load() {
    IR.Data = Load32(PC)
}

type Instruction struct {}

func (instruction Instruction) X() uint32 {
    return (IR.Data & X) >> DeslocX
}

func (instruction Instruction) Y() uint32 {
    return (IR.Data & Y) >> DeslocY
}

func (instruction Instruction) Z() uint32 {
    return (IR.Data & Z) >> DeslocZ
}

func (instruction Instruction) L() uint32 {
    return (IR.Data & L)
}

func (instruction Instruction) I11() uint32 {
    return (IR.Data & I11)
}

func (instruction Instruction) I16() uint32 {
    return (IR.Data & I16)
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

type Mov struct { InstructionFormatU }


// R[z] = 0 : x : y : l
func (mov *Mov) Execute() {
    *mov.RZ = uint32(0x0) + (mov.X() << DeslocX) & X + (mov.Y() << DeslocY) & Y + (mov.L() & L)
}

func(mov * Mov) Print() {
    execution := fmt.Sprintf("R%d=0x%08X", mov.Z(), *mov.RZ)
    code :=fmt.Sprintf("mov r%d, %d", mov.Z(), *mov.RZ)

    write(code, execution)
}

type Movs struct { InstructionFormatU }

// R[z] = (11)X4 : X : Y : L
func (movs *Movs) Execute() {
    *movs.RZ = uint32(0x0) + (movs.X() << DeslocX) & X + (movs.Y() << DeslocY) & Y + (movs.L() & L)

    *movs.RZ |= movs.signal()
}

func (movs Movs) signal() uint32 {   
    if (movs.X() >> 4) == uint32(0x1) {
       return uint32(0xFFE00000)
    } else {
       return uint32(0)
    }
}

func(movs * Movs) Print() {
    execution := fmt.Sprintf("R%d=0x%08X", movs.Z(), *movs.RZ)
    code := fmt.Sprintf("movs r%d, %d", movs.Z(), int32(*movs.RZ))

    write(code, execution)
}

type Add struct { InstructionFormatU }

// R[z] = R[x] + R[y]
func (add *Add) Execute() {
    var carry uint32

    *add.RZ, carry = bits.Add32(*add.RX, *add.RY, 0)

    if *add.RZ == 0 { SR.ZN() }

    if (*add.RZ >> 31) == 1 { SR.SN() }

    if carry == 1 { SR.CY() }

    if (*add.RX >> 31) == (*add.RY >> 31) &&
    (*add.RZ >> 31) != (*add.RX) {
	SR.OV()
    }

    // TODO: Implement overflow
}

func(add * Add) Print() {
    execution := fmt.Sprintf("R%d=R%d+R%d=0x%08X,SR=0x%08X", add.Z(), add.X(), add.Y(), *add.RZ, SR.Data)
    code := fmt.Sprintf("add r%d,r%d,r%d", add.Z(), add.X(), add.Y())

    write(code, execution)
}


type Sub struct { InstructionFormatU }

// R[z] = R[x] + R[y]
func (sub *Sub) Execute() {
    var carry uint32

    *sub.RZ, carry = bits.Sub32(*sub.RX, *sub.RY, 0)

    if *sub.RZ == 0 { SR.ZN() }

    if (*sub.RZ >> 31) == 1 { SR.SN() }

    if carry == 1 { SR.CY() }

    // TODO: Implement overflow
}

func(sub * Sub) Print() {
    execution := fmt.Sprintf("R%d=R%d+R%d=0x%08X,SR=0x%08X", sub.Z(), sub.X(), sub.Y(), *sub.RZ, SR.Data)
    code := fmt.Sprintf("sub r%d,r%d,r%d", sub.Z(), sub.X(), sub.Y())

    write(code, execution)
}
// Store 32 bits from memory
func Store32(address uint32 , Data uint32) {
    address *= 2

    Store16(address, uint16((Data & 0xFFFF0000) >> 16))
    Store16(address + 1, uint16((Data & 0x0000FFFF) >> 0))
}

// Store 16 bits from memory
func Store16(address uint32, Data uint16) {
    address *= 2

    Store8(address, uint8((Data & 0xFF00) >> 8))
    Store8(address + 1, uint8((Data & 0x00FF) >> 0))
}

// Store 8 bits from memory
func Store8(address uint32, Data uint8) {
    MEM[address] = Data
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
func Load8(adress uint32) (Data uint8){
    return MEM[adress]
}

func bun(count uint32) {
    PC *= count
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

func print_sum() {
    execution := fmt.Sprintf("%s=%s+%s=0x%08X,SR=0x%08X", "R1", "R2", "R3", 0, 0x40)
    code :=fmt.Sprintf("add r,r2,r3")

    write(code, execution)
}

func write(code string, execution string) {
    fmt.Printf("0x%08X:\t%-25s\t%s\n", PC, code, execution);
}

func main() {
    setup_IO()
    read_instructions(os.Stdin)

    fmt.Println("[START OF SIMULATION]");
    // print_sum()

    add := Add{}
    R[1] = 0x00123456
    R[2] = 0xFFF00000
    IR.Data = uint32(0x08611000)
    add.New()
    add.Execute()
    add.Print()

    // for {
    //     IR.Load()

    //     exececutable := collection[IR.Opcode()]

    //     exececutable.new()

    //     exececutable.run()

    //     exececutable.print()
    // }
}
