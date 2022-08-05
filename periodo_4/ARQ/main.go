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

    R *[32]Registers = new([32]Registers)

    INSTRUCTION = InstructionCollection{}

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
    SR *StatusRegister 

    /*
	Stack pointer (SP): references the top of the stack in memory (static allocation and subroutines)

	    31                                              0
	     ┌──────────────────────────────────────────────┐
	     │                STACK ADDRESS                 │
	     └──────────────────────────────────────────────┘
	     R30 = SP
    */
    SP *StackPointer

    /*
	Program counter (PC): Controls the flow of program execution, pointing to the instructions

	    31                                              0
	     ┌──────────────────────────────────────────────┐
	     │              INSTRUCTION ADDRESS             │
	     └──────────────────────────────────────────────┘
	     R29 = PC
    */
    PC *ProgramCounter

    /*
	Instruction Register (IR): stores the instruction loaded from memory and executed

	    31                                              0
	     ┌──────────────────────────────────────────────┐
	     │               CODE INSTRUCTION               │
	     └──────────────────────────────────────────────┘
	     R28 = IR
    */
    IR *InstructionRegister
)

const (
     I_SR = 31
     I_SP = 30
     I_PC = 29
     I_IR = 28

     OP            = uint32(0xFC000000)
     SubCode       = uint32(0x00000700)
     Z             = uint32(0x03E00000)
     X             = uint32(0x001F0000)
     Y             = uint32(0x0000F800)
     L             = uint32(0x000007FF)
     I5            = uint32(0x0000001F)
     I11           = uint32(0x000007FF)
     I16           = uint32(0x07FFFFFF)

     DeslocOP      = uint32(26)
     DeslocSubCode = uint32(8)
     DeslocZ       = uint32(21)
     DeslocX       = uint32(16)
     DeslocY       = uint32(11)
)

type Executable interface {
    New()
    Execute()
    Status()
    Store()
    Print()
}

// {{{ Struct For Registers

    // This interface define the format basic of register
    type Registers interface {
	Get() uint32
	Set(uint32)
    }

    // Struct Basic Regist
    type Register struct {
	data uint32
    }
    // Default read access
    func (register *Register) Get() uint32 {
	return register.data
    }

    // Default write access
    func (register *Register) Set(val uint32) {
	register.data = val
    }
    
    // Default register without registrition for read and write
    type GeneralRegister struct {
	Register
    }

    // Default register with registrition for write
    type ReadOnlyRegister struct {
	Register
    }

    // Especific Set for blocked write in register
    func (register *ReadOnlyRegister) Set(val uint32) {}

    // {{{ Program counter (PC): Controls the flow of program execution, pointing to the instructions
	type ProgramCounter struct {
	    Register
	}
    // }}} 

    // {{{ Stack pointer (SP): references the top of the stack in memory (static allocation and subroutines) 
	type StackPointer struct {
	    Register
	}
    // }}}

    // {{{ Status register (SR): control of settings and status of processor operations
	type StatusRegister struct {
	    Register
	}
	// ZN (zero): equal to 0
	    func (SR *StatusRegister) ZN() {
		SR.Set(SR.Get() | 0x40)
	    }

	// ZD (division by zero): divisor B = 0
	    func (SR *StatusRegister) ZD() {
		SR.Set(SR.Get() | 0x20)
	    }

	// SN (sign): indicates if the result of the operation has a negative sign
	    func (SR *StatusRegister) SN() {
		SR.Set(SR.Get() | 0x10)
	    }

	// OV (overflow): capacity overflow
	    func (SR *StatusRegister) OV() {
		SR.Set(SR.Get() | 0x8)
	    }

	// IV (invalid instruction): Invalid operation code
	    func (SR *StatusRegister) IV() {
		SR.Set(SR.Get() | 0x4)
	    }

	// CY (carry): goes to an arithmetic
	    func (SR *StatusRegister) CY() {
		SR.Set(SR.Get() | 0x1)
	    }
    // }}} 

    // {{{ Struct for Instruction Register (IR): stores the instruction loaded from memory and executed
	type InstructionRegister struct {
	    Register
	}
	// Operation Code (OP): code indentify for instructions
	    func (IR InstructionRegister) Opcode() uint8 {
		return uint8((IR.Get() & OP) >> DeslocOP)
	    }

	// Sub Operation Code (SubCode): sub code indentify for instructions with ID nested
	    func (IR InstructionRegister) SubCode() uint8 {
		return uint8((IR.Get() & SubCode) >> DeslocSubCode)
	    }

	// Load instruction of memory in address PC for IR
	    func (IR *InstructionRegister) Load() {
		IR.data = Load32(PC.data)
	    }
    // }}} 

// }}}

type InstructionCollection struct {
    data map[uint16]Executable
}

func (collection *InstructionCollection) New() {
    collection.data = make(map[uint16]Executable)
}

func (collection *InstructionCollection) Setup() {
    // collection.data[uint16(0)] = &Mov{}
    // collection.data[uint16(1)] = &Movs{}
    // collection.data[uint16(2)] = &Add{}
    // collection.data[uint16(3)] = &Sub{}
    // collection.data[uint16(4) * uint16(300) + uint16(0)] = &Mul{}
    // collection.data[uint16(4) * uint16(300) + uint16(1)] = &Sll{}
    // collection.data[uint16(4) * uint16(300) + uint16(2)] = &Muls{}
    // collection.data[uint16(4) * uint16(300) + uint16(3)] = &Sla{}
    // collection.data[uint16(4) * uint16(300) + uint16(4)] = &Div{}
}

func (collection *InstructionCollection) Get() Executable {
    opcode := uint16(IR.Opcode())
    subcode := uint16(IR.SubCode())

    if opcode != uint16(4) {
	return  collection.data[opcode]
    }

    return collection.data[opcode * 300 + subcode]
}

// {{{ Intructions

    // {{{ Default struct for Instruction, each instruction defining yours fields
	type Instruction struct {}

	// Return bits of 25 <-> 21 with 6 bits
	func (instruction Instruction) Z() uint32 {
	    return (IR.data & Z) >> DeslocZ
	}

	// Return bits of 20 <-> 16 with 6 bits
	func (instruction Instruction) X() uint32 {
	    return (IR.Get() & X) >> DeslocX
	}

	// Return bits of 15 <-> 11 with size 6 bits
	func (instruction Instruction) Y() uint32 {
	    return (IR.data & Y) >> DeslocY
	}

	// Return bits of 10 <-> 0 with size 11 bits
	func (instruction Instruction) L() uint32 {
	    return (IR.data & L)
	}

	// Return bits of 4 <-> 0 with size 5 bits
	func (instruction Instruction) I5() uint32 {
	    return (IR.data & I5)
	}

	// Return bits of 10 <-> 0 with size 11 bits
	func (instruction Instruction) I11() uint32 {
	    return (IR.data & I11)
	}

	// Return bits of 15 <-> 0 with size 16 bits
	func (instruction Instruction) I16() uint32 {
	    return (IR.data & I16)
	}
    // }}} 

    /// {{{ Especific formats
	// {{{ Format U 
	    /**
		Fields (OP, Z, X, Y, L)
		    ▶ 6 bits para operação (OP)
		    ▶ 5 bits para operandos (Z, X, Y)
		    ▶ 11 bits para uso livre (L)

		31      26 25   21 20   16 15   11 10           0
		  ┌───────┬───────┬───────┬───────┬─────────────┐
		  │  OP   │   Z   │   X   │   Y   │      L      │
		  └───────┴───────┴───────┴───────┴─────────────┘
	    **/

	    type InstructionFormatU struct {
		Instruction

		RZ, RX, RY Registers

		MS uint32 // MS - Most Partial Signifitive Operation Result
		LS uint32 // LS - Less Partial Signifitive Operation Result
	    }

	    func (instruction *InstructionFormatU) New() {
		instruction.RZ = R[instruction.Z()]
		instruction.RX = R[instruction.X()]
		instruction.RY = R[instruction.Y()]
	    }

	    type InstructionFormatUforSubCode struct {
		InstructionFormatU

		RI Registers
	    }

	    func (instruction *InstructionFormatUforSubCode) New() {
		instruction.RZ = R[instruction.Z()]
		instruction.RX = R[instruction.X()]
		instruction.RY = R[instruction.Y()]
		instruction.RI = R[instruction.I5()]
		instruction.MS = 0
		instruction.LS = 0
	    }

	    func (instruction InstructionFormatUforSubCode) Desloc() uint32 {
	        return instruction.I5() + 1
	    }
	// }}} 

	// {{{ Formato F
	    /**
		Fields (OP, Z, X, I16)
		    ▶ 6 bits para operação (OP)
		    ▶ 5 bits para operandos (Z, X)
		    ▶ 16 bits para imediato (I16)

		31      26 25   21 20   16 15                   0
		  ┌───────┬───────┬───────┬─────────────────────┐
		  │   OP  │   Z   │   X   │        I16          │
		  └───────┴───────┴───────┴─────────────────────┘
	    **/
	    type InstructionFormatF struct {
		Instruction

		RZ, RX Registers
	    }

	    func (instruction *InstructionFormatF) New() {
		instruction.RZ = R[instruction.Z()]
		instruction.RX = R[instruction.X()]
	    }
	// }}}

	// {{{ Formato S
	    /**
		Fields (OP, I26)
		    ▶ 6 bits para operação (OP)
		    ▶ 26 bits para imediato (I26)

		31      26 25                                   0
		  ┌───────┬─────────────────────────────────────┐
		  │   OP  │                I26                  │
		  └───────┴─────────────────────────────────────┘
	    **/
	    type InstructionFormatS struct {
		Instruction
	    }

	    func (instruction *InstructionFormatS) New() {}
	// }}}
    /// }}}
// }}}

// {{ Instructions
    // {{{ Mov - R[z] = 0 : x : y : l
	type Mov struct { InstructionFormatU }

	func (mov *Mov) Execute() {
	    mov.LS = uint32(0x0) + (mov.X() << DeslocX) & X + (mov.Y() << DeslocY) & Y + (mov.L() & L)
	}

	func (mov *Mov) Status() {

	}

	func (mov *Mov) Store() {
	    mov.RZ.Set(mov.LS)
	}

	func(mov *Mov) Print() {
	    execution := fmt.Sprintf("R%d=0x%08X", mov.Z(), mov.RZ.Get())
	    code :=fmt.Sprintf("mov r%d, %d", mov.Z(), mov.RZ.Get())

	    write(code, execution)
	}
    // }}}

    // {{{ Movs - R[z] = (11)X4 : X : Y : L
	type Movs struct { InstructionFormatU }

	func (movs *Movs) Execute() {
	    movs.LS = (0x0) | (movs.X() << DeslocX) & X | (movs.Y() << DeslocY) & Y | (movs.L() & L)

	    movs.LS |= movs.signal()
	}

	func (movs *Movs) signal() uint32 {
	    if (movs.X() >> 4) == uint32(0x1) {
	       return uint32(0xFFE00000)
	    } else {
	       return uint32(0x0)
	    }
	}

	 //  Without conditions for status
	func (mov *Movs) Status() {

	}

	func (movs *Movs) Store() {
	    movs.RZ.Set(movs.LS)
	}

	func(movs * Movs) Print() {
	    execution := fmt.Sprintf("R%d=0x%08X", movs.Z(), movs.RZ.Get())
	    code := fmt.Sprintf("movs r%d, %d", movs.Z(), int32(movs.RZ.Get()))

	    write(code, execution)
	}
    // }}}

    // {{{ Add -  R[z] = R[x] + R[y]
	type Add struct { InstructionFormatU }

	//
	func (add *Add) Execute() {
	    add.LS, add.MS = bits.Add32(add.RX.Get(), add.RY.Get(), 0)
	}

	func (add *Add) Status() {
	    if add.LS == 0 { SR.ZN() }

	    if (add.LS >> 31) == 1 { SR.SN() }

	    if add.MS == 1 { SR.CY() }

	    if (add.RX.Get() >> 31) == (add.RY.Get() >> 31) &&
	    (add.LS >> 31) != (add.RX.Get()) {
		SR.OV()
	    }
	}

	func (add *Add) Store() {
	    add.RZ.Set(add.LS)
	}

	func(add * Add) Print() {
	    execution := fmt.Sprintf("R%d=R%d+R%d=0x%08X,SR=0x%08X", add.Z(), add.X(), add.Y(), add.RZ.Get(), SR.Get())
	    code := fmt.Sprintf("add r%d,r%d,r%d", add.Z(), add.X(), add.Y())

	    write(code, execution)
	}
    // }}} 

    // {{{ Sub - R[z] = R[x] - R[y]
	type Sub struct { InstructionFormatU }

	func (sub *Sub) Execute() {
	    sub.LS, sub.MS = bits.Sub32(sub.RX.Get(), sub.RY.Get(), 0)
	}

	func (sub *Sub) Status() {
	    if sub.LS == 0 { SR.ZN() }

	    if (sub.LS >> 31) == 1 { SR.SN() }

	    if sub.MS == 1 { SR.CY() }

	    if (sub.RX.Get() >> 31) != (sub.RY.Get() >> 31) &&
	       (sub.LS >> 31) != (sub.RX.Get()) {
		SR.OV()
	    }
	}

	func (sub *Sub) Store() {
	    sub.RZ.Set(sub.LS)
	}

	func(sub * Sub) Print() {
	    execution := fmt.Sprintf("R%d=R%d-R%d=0x%08X,SR=0x%08X", sub.Z(), sub.X(), sub.Y(), sub.RZ.Get(), SR.Get())
	    code := fmt.Sprintf("sub r%d,r%d,r%d", sub.Z(), sub.X(), sub.Y())

	    write(code, execution)
	}
    // }}}
// }}



type Mul struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (mul *Mul) Execute() {
    mul.MS, mul.LS = bits.Mul32(mul.RX.Get(), mul.RY.Get())
}

func (mul *Mul) Status() {
    if mul.MS == 0 && mul.LS == 0 { SR.ZN() }

    if (mul.MS) != 0 { SR.CY() }
}

func (mul * Mul) Store() {
    mul.RI.Set(mul.MS)
    mul.RZ.Set(mul.LS)
}

func(mul *Mul) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d*R%d=0x%08X%08X,SR=0x%08X",mul.I5(), mul.Z(), mul.X(), mul.Y(), mul.RI.Get(), mul.RZ.Get(), SR.Get())
    code := fmt.Sprintf("mul r%d,r%d,r%d,r%d", mul.I5(),  mul.Z(), mul.X(), mul.Y())

    write(code, execution)
}


type Sll struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (sll *Sll) Execute() {
    sll.MS ,sll.LS = sll.sll32()
}

func (sll *Sll) sll32() (uint32, uint32) {
    tmp := uint64(sll.RZ.Get()) << 32 | uint64(sll.RY.Get())

    tmp <<= uint64(sll.I5())

    return uint32(tmp >> 32), uint32(tmp)
}

func (sll *Sll) Status() {
    if sll.MS == 0 && sll.LS == 0{ SR.ZN() }

    if (sll.MS) != 0 { SR.CY() }
}

func (sll *Sll) Store() {
    sll.RZ.Set(sll.MS)
    sll.RX.Set(sll.LS)
}

func(sll * Sll) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d<<%d=0x%08X%08X,SR=0x%08X",sll.Z(), sll.X(), sll.Z(), sll.Y(), sll.I5(), sll.RZ.Get(), sll.RX.Get(), SR.Get())
    code := fmt.Sprintf("sll r%d,r%d,r%d,%d", sll.Z(), sll.X(), sll.Y(), sll.I5())

    write(code, execution)
}

type Muls struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (muls *Muls) Execute() {
    muls.MS, muls.LS = bits.Mul32(muls.RX.Get(), muls.RY.Get())
}

func Mul32(x, y uint32) (uint32, uint32) {
    tmp := int64(x) * int64(y)

    return uint32(tmp>>32), uint32(tmp)
}

func (muls *Muls) Status() {
    if muls.LS == 0 && muls.MS == 0{ SR.ZN() }

    if (muls.MS) != 0 { SR.CY() }
}

func (muls *Muls) Store() {
    muls.RI.Set(muls.MS)
    muls.RZ.Set(muls.LS)
}

func(mul * Muls) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d*R%d=0x%08X%08X,SR=0x%08X",mul.I5(), mul.Z(), mul.X(), mul.Y(), mul.RI.Get(), mul.RZ.Get(), SR.Get())
    code := fmt.Sprintf("muls r%d,r%d,r%d,r%d", mul.I5(),  mul.Z(), mul.X(), mul.Y())

    write(code, execution)
}

type Sla struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (sla *Sla) Execute() {
    sla.MS ,sla.LS = sla.sla32()
}

func (sla *Sla) sla32() (uint32, uint32) {
    tmp := uint64(sla.RZ.Get()) << 32 | uint64(sla.RY.Get())

    tmp <<= uint64(sla.Desloc())

    return uint32(tmp >> 32), uint32(tmp)
}

func (sla *Sla) Status() {
    if sla.MS == 0 && sla.LS == 0{ SR.ZN() }

    if sla.MS != 0 { SR.CY() }
}

func (sla *Sla) Store() {
    sla.RZ.Set(sla.MS)
    sla.RX.Set(sla.LS)
}

func(sla * Sla) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d<<%d=0x%08X%08X,SR=0x%08X",sla.Z(), sla.X(), sla.Z(), sla.Y(), sla.Desloc(), sla.RZ.Get(), sla.RX.Get(), SR.Get())
    code := fmt.Sprintf("sla r%d,r%d,r%d,%d", sla.Z(), sla.X(), sla.Y(), sla.I5())

    write(code, execution)
}

type Div struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (div *Div) Execute() {
    if div.RY.Get() == uint32(0x00000000) {
	div.MS, div.LS = 0, 0
	return
    }

    div.MS = div.RX.Get() % div.RY.Get()
    div.LS = div.RX.Get() / div.RY.Get()
}

func (div *Div) Status() {
    if div.LS == 0 { SR.ZN() }

    if div.RY.Get() == 0 { SR.ZD() }

    if div.MS != 0 { SR.CY() }
}

func (div *Div) Store() {
    if SR.Get() != uint32(0x00000060){
	div.RI.Set(div.MS)
	div.RZ.Set(div.LS)
    }
}

func(div * Div) Print() {
    var execution string
    var mod, quo uint32 

    if SR.Get() != uint32(0x00000060){
	mod = div.RI.Get()
	quo = div.RZ.Get()
    } else {
	mod = div.MS
	quo = div.LS
    }

    execution = fmt.Sprintf("R%d=R%d%%R%d=0x%08X,R%d=R%d/R%d=0x%08X,SR=0x%08X",
			      div.I5(), div.X(), div.Y(), mod, div.Z(), div.X(), div.Y(), quo, SR.Get())

    code := fmt.Sprintf("div r%d,r%d,r%d,r%d", div.I5(),  div.Z(), div.X(), div.Y())

    write(code, execution)
}

type Srl struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (srl *Srl) Execute() {
    hi, lo := srl.slr32()

    srl.RZ.Set(hi)
    srl.RX.Set(lo)

    if srl.RZ.Get() == 0 && srl.RX.Get() == 0{ SR.ZN() }

    if srl.RZ.Get() != 0 { SR.CY() }
}

func (srl *Srl) slr32() (uint32, uint32) {
    tmp := uint64(srl.RZ.Get()) << 32 | uint64(srl.RY.Get())

    tmp >>= uint64(srl.I5())

    return uint32(tmp >> 32), uint32(tmp)
}

func(srl * Srl) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d>>%d=0x%08X%08X,SR=0x%08X",srl.Z(), srl.X(), srl.Z(), srl.Y(), srl.I5(), srl.RZ.Get(), srl.RX.Get(), SR.Get())
    code := fmt.Sprintf("srl r%d,r%d,r%d,%d", srl.Z(), srl.X(), srl.Y(), srl.I5())

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
    PC.data *= count
}

func NewRegister(register_type string) Registers {
    var register Registers
    switch register_type {
	case "ReadOnlyRegister": register = &ReadOnlyRegister{}
	case "GeneralRegister":  register = &GeneralRegister{}
	case "SR":		 register = &StatusRegister{}
	case "IR":		 register = &InstructionRegister{}
	case "SP":		 register = &StackPointer{}
	case "PC":		 register = &ProgramCounter{}
    }

    return register
}

func Setup_registers() {
    R[0] = NewRegister("ReadOnlyRegister")

    for i := 1; i <= 27; i++ {
        R[i] = NewRegister("GeneralRegister")
    }

    R[I_SR] = NewRegister("SR")
    R[I_PC] = NewRegister("PC")
    R[I_IR] = NewRegister("IR")
    R[I_SP] = NewRegister("SP")

    SR = R[I_SR].(*StatusRegister)

    SP = R[I_SP].(*StackPointer)

    PC = R[I_PC].(*ProgramCounter)

    IR = R[I_IR].(*InstructionRegister)
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

// Think if this code must going in Instruction
func write(code string, execution string) {
    fmt.Printf("0x%08X:\t%-25s\t%s\n", PC.Get(), code, execution);
}

func main() {
    setup_IO()
    Setup_registers()
    read_instructions(os.Stdin)

    INSTRUCTION.New()
    INSTRUCTION.Setup()
    fmt.Println("[START OF SIMULATION]");

    R[0].Set(0)

    // for i := 0; i < 6; i++ {
	// IR.Load()
	// PC.data += 1

	// exececutable := INSTRUCTION.Get()

	// exececutable.New()

	// exececutable.Execute()

	// exececutable.Print()
    // }

    fmt.Println("[END OF SIMULATION]");
}
