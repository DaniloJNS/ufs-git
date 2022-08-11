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

    STATUS uint32
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
     I16           = uint32(0x0000FFFF)
     I26           = uint32(0x00FFFFFF)

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

type ExecutableFormatS interface {
    Executable
    PC()
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

	    func convertBool(set bool ) uint32 {
	        if set {
	            return uint32(1)
		} else {
		    return uint32(0)
		}
	    }

	// ZN (zero): equal to 0
	    func (SR *StatusRegister) ZN(set bool ) {
		bit :=  convertBool(set) << 6
		resetBit:= uint32(0x000000001) << 6
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_ZN() bool {
		ZN :=  (SR.Get() >> 6) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
	    }

	// ZD (division by zero): divisor B = 0
	    func (SR *StatusRegister) ZD(set bool) {
		bit :=  convertBool(set) << 5
		resetBit:= uint32(0x000000001) << 5
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_ZD() bool {
		ZN :=  (SR.Get() >> 5) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
	    }

	// SN (sign): indicates if the result of the operation has a negative sign
	    func (SR *StatusRegister) SN(set bool) {
		bit :=  convertBool(set) << 4
		resetBit:= uint32(0x000000001) << 4
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_SN() bool {
		ZN :=  (SR.Get() >> 4) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
	    }

	// OV (overflow): capacity overflow
	    func (SR *StatusRegister) OV(set bool) {
		bit :=  convertBool(set) << 3
		resetBit:= uint32(0x000000001) << 3
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_OV() bool {
		ZN :=  (SR.Get() >> 3) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
	    }

	// IV (invalid instruction): Invalid operation code
	    func (SR *StatusRegister) IV(set bool) {
		bit :=  convertBool(set) << 2
		resetBit:= uint32(0x000000001) << 2
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_IV() bool {
		ZN :=  (SR.Get() >> 2) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
	    }

	// CY (carry): goes to an arithmetic
	    func (SR *StatusRegister) CY(set bool) {
		bit :=  convertBool(set) << 0
		resetBit:= uint32(0x000000001) << 0
		SR.Set((SR.Get() & ^resetBit) | bit)
	    }

	    func (SR *StatusRegister) Get_CY() bool {
		ZN :=  (SR.Get() >> 0) & uint32(0x00000001)
		return ZN == uint32(0x00000001)
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
		IR.data = Load32(PC.data / 4)
	    }
	//  Return when current instruction is an NOP
	    func (IR *InstructionRegister) NOP() bool {
		return IR.data == 0
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
    collection.data[uint16(0)] = &Mov{}
    collection.data[uint16(1)] = &Movs{}
    collection.data[uint16(2)] = &Add{}
    collection.data[uint16(3)] = &Sub{}
    collection.data[uint16(4) * uint16(300) + uint16(0)] = &Mul{}
    collection.data[uint16(4) * uint16(300) + uint16(1)] = &Sll{}
    collection.data[uint16(4) * uint16(300) + uint16(2)] = &Muls{}
    collection.data[uint16(4) * uint16(300) + uint16(3)] = &Sla{}
    collection.data[uint16(4) * uint16(300) + uint16(4)] = &Div{}
    collection.data[uint16(4) * uint16(300) + uint16(5)] = &Srl{}
    collection.data[uint16(4) * uint16(300) + uint16(6)] = &Divs{}
    collection.data[uint16(4) * uint16(300) + uint16(7)] = &Sra{}
    collection.data[uint16(5)] = &Cmp{}
    collection.data[uint16(6)] = &And{}
    collection.data[uint16(7)] = &Or{}
    collection.data[uint16(8)] = &Not{}
    collection.data[uint16(9)] = &Xor{}
    collection.data[uint16(18)] = &Addi{}
    collection.data[uint16(19)] = &Subi{}
    collection.data[uint16(20)] = &Muli{}
    collection.data[uint16(21)] = &Divi{}
    collection.data[uint16(22)] = &Modi{}
    collection.data[uint16(23)] = &Cmpi{}
    collection.data[uint16(24)] = &L8{}
    collection.data[uint16(25)] = &L16{}
    collection.data[uint16(26)] = &L32{}
    collection.data[uint16(27)] = &S8{}
    collection.data[uint16(28)] = &S16{}
    collection.data[uint16(29)] = &S32{}
    collection.data[uint16(42)] = &Bae{}
    collection.data[uint16(43)] = &Bat{}
    collection.data[uint16(44)] = &Bbe{}
    collection.data[uint16(45)] = &Bbt{}
    collection.data[uint16(46)] = &Beq{}
    collection.data[uint16(47)] = &Bge{}
    collection.data[uint16(48)] = &Bgt{}
    collection.data[uint16(49)] = &Biv{}
    collection.data[uint16(50)] = &Ble{}
    collection.data[uint16(51)] = &Blt{}
    collection.data[uint16(52)] = &Bne{}
    collection.data[uint16(53)] = &Bni{}
    collection.data[uint16(54)] = &Bnz{}
    collection.data[uint16(55)] = &Bun{}
    collection.data[uint16(56)] = &Bzd{}
    collection.data[uint16(63)] = &Int{}
}
// FIX: case when não existts instructiion
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

	// Return bits of 25 <-> 0 with size 16 bits
	func (instruction Instruction) I26() uint32 {
	    return (IR.data & I26)
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

		MS uint32 // MS - Most Partial Signifitive Operation Result
		LS uint32 // LS - Less Partial Signifitive Operation Result
		
	    }

	    func (instruction *InstructionFormatF) New() {
		instruction.RZ = R[instruction.Z()]
		instruction.RX = R[instruction.X()]
		instruction.MS = 0
		instruction.LS = 0
	    }

	    func (instruction InstructionFormatF) I16s() uint32 {
		return instruction.I16() | instruction.signal()
	    }

	    func (instruction *InstructionFormatF) signal() uint32 {
		if (instruction.I16() >> 15) == uint32(0x1) {
		   return uint32(0xFFFF0000)
		} else {
		   return uint32(0x00000000)
		}
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

		NAD uint32 // Address - New value for address
	    }

	    func (instruction *InstructionFormatS) New() {
		instruction.NAD = PC.data + 4
	    }

	    func (instruction *InstructionFormatS) Jump() {
		instruction.NAD += instruction.I26s() * 4
	    }

	    func (instruction *InstructionFormatS) PC() {
		PC.Set(instruction.NAD)
	    }

	    func (instruction *InstructionFormatS) Execute() {}

	    func (instruction *InstructionFormatS) Status() {}

	    func (instruction InstructionFormatS) I26s() uint32 {
		return instruction.I26() | instruction.signal()
	    }

	    func (instruction *InstructionFormatS) signal() uint32 {
		if (instruction.I26() >> 25) == uint32(0x1) {
		   return uint32(0xFF000000)
		} else {
		   return uint32(0x00000000)
		}
	    }
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
	    SR.ZN(add.LS == 0) 

	    SR.SN((add.LS >> 31) == 1)

	    SR.CY(add.MS != 0)

	    SR.OV((add.RX.Get() >> 31) == (add.RY.Get() >> 31) &&
		  (add.LS >> 31) != (add.RX.Get()))
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
	    x := uint64(sub.RX.Get())
	    y := uint64(sub.RY.Get())
	    R := x - y

	    sub.LS, sub.MS = uint32(R),  uint32(R >> 32)
	}

	func (sub *Sub) Status() {
	    SR.ZN(sub.LS == 0)

	    SR.SN((sub.LS >> 31) == 1)

	    SR.CY(sub.MS != 0)

	    SR.OV((sub.RX.Get() >> 31) != (sub.RY.Get() >> 31) &&
	   (sub.LS >> 31) != (sub.RX.Get()))
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
    SR.ZN(mul.MS == 0 && mul.LS == 0)

    SR.CY((mul.MS) != 0)
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

    tmp <<= uint64(sll.Desloc())

    return uint32(tmp >> 32), uint32(tmp)
}

func (sll *Sll) Status() {
    SR.ZN(sll.MS == 0 && sll.LS == 0)

    SR.CY((sll.MS) != 0)
}

func (sll *Sll) Store() {
    sll.RZ.Set(sll.MS)
    sll.RX.Set(sll.LS)
}

func(sll * Sll) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d<<%d=0x%08X%08X,SR=0x%08X",sll.Z(), sll.X(), sll.Z(), sll.Y(), sll.Desloc(), sll.RZ.Get(), sll.RX.Get(), SR.Get())
    code := fmt.Sprintf("sll r%d,r%d,r%d,%d", sll.Z(), sll.X(), sll.Y(), sll.I5())

    write(code, execution)
}

type Muls struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (muls *Muls) Execute() {
    muls.MS, muls.LS = muls.Mul32(muls.RX.Get(), muls.RY.Get())
}

func (muls *Muls) Mul32(x, y uint32) (uint32, uint32) {
    var tmp int64

    tmp = int64(x) * int64(y)

    return uint32(tmp>>32), uint32(tmp)
}

func (muls *Muls) Status() {
    SR.ZN(muls.LS == 0 && muls.MS == 0)

    SR.OV((muls.MS) != 0)
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
    tmp := int64(sla.RZ.Get()) << 32 | (int64(sla.RY.Get()) & int64(0x00000000FFFFFFFF))

    tmp <<= sla.Desloc()

    return uint32(tmp >> 32), uint32(tmp)
}

func (sla *Sla) Status() {
    SR.ZN(sla.MS == 0 && sla.LS == 0)

    SR.CY(sla.MS != 0)
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
    SR.ZD(div.RY.Get() == 0) 

    if div.RY.Get() == 0 { return } // When has divi per zero only set ZD

    SR.ZD(div.RY.Get() == 0)

    SR.CY(div.MS != 0)
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
    srl.MS, srl.LS = srl.slr32()
}

func (srl *Srl) slr32() (uint32, uint32) {
    tmp := uint64(srl.RZ.Get()) << 32 | uint64(srl.RY.Get())

    tmp >>= uint64(srl.Desloc())

    return uint32(tmp >> 32), uint32(tmp)
}

func (srl *Srl) Status() {
    SR.ZN(srl.MS == 0 && srl.LS == 0)

    SR.CY(srl.MS != 0)
}

func (srl *Srl) Store() {
    srl.RZ.Set(srl.MS)
    srl.RX.Set(srl.LS)
}

func(srl * Srl) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d>>%d=0x%08X%08X,SR=0x%08X",
    srl.Z(), srl.X(), srl.Z(), srl.Y(), srl.Desloc(), srl.RZ.Get(), srl.RX.Get(), SR.Get())
    code := fmt.Sprintf("srl r%d,r%d,r%d,%d", srl.Z(), srl.X(), srl.Y(), srl.I5())

    write(code, execution)
}

type Divs struct { InstructionFormatUforSubCode }

func (div *Divs)Execute() {
    var x, y, hi, lo int32

    x = int32(div.RX.Get())
    y = int32(div.RY.Get())

    if y != 0 {
	hi = x % y
	lo = x / y
    } else {
	hi = 0
	lo = 0
    }

    div.MS = uint32(hi) 
    div.LS = uint32(lo) 
}

func (div *Divs) Status() {

    SR.ZD(div.RY.Get() == 0) 

    if div.RY.Get() == 0 { return } // When has divi per zero only set ZD

    SR.ZN(div.LS == 0)

    SR.OV(div.MS != 0)
}

func (divs *Divs) Store() {
    if SR.Get() != uint32(0x00000060){
	divs.RI.Set(divs.MS)
	divs.RZ.Set(divs.LS)
    }
}

func(divs * Divs) Print() {
    var execution string
    var mod, quo uint32 

    if SR.Get() != uint32(0x00000060){
	mod = divs.RI.Get()
	quo = divs.RZ.Get()
    } else {
	mod = divs.MS
	quo = divs.LS
    }

    execution = fmt.Sprintf("R%d=R%d%%R%d=0x%08X,R%d=R%d/R%d=0x%08X,SR=0x%08X",
			      divs.I5(), divs.X(), divs.Y(), mod, divs.Z(), divs.X(), divs.Y(), quo, SR.Get())

    code := fmt.Sprintf("divs r%d,r%d,r%d,r%d", divs.I5(),  divs.Z(), divs.X(), divs.Y())

    write(code, execution)
}


type Sra struct { InstructionFormatUforSubCode }

// R[I] : R[z] = R[x] * R[y]
func (sra *Sra) Execute() {
    sra.MS, sra.LS = sra.slr32()
}

func (sra *Sra) slr32() (uint32, uint32) {
    tmp := int64(sra.RZ.Get()) << 32 | (int64(sra.RY.Get()) & int64(0x00000000FFFFFFFF)) // cast for 64 pode gerar extenção sinal indevida

    tmp >>=sra.Desloc()

    return uint32(tmp >> 32), uint32(tmp)
}

func (sra *Sra) Status() {
    SR.ZN(sra.MS == 0 && sra.LS == 0)

    SR.CY(sra.MS != 0)
}

func (sra *Sra) Store() {
    sra.RZ.Set(sra.MS)
    sra.RX.Set(sra.LS)
}

func(sra * Sra) Print() {
    execution := fmt.Sprintf("R%d:R%d=R%d:R%d>>%d=0x%08X%08X,SR=0x%08X",
    sra.Z(), sra.X(), sra.Z(), sra.Y(), sra.Desloc(), sra.RZ.Get(), sra.RX.Get(), SR.Get())
    code := fmt.Sprintf("sra r%d,r%d,r%d,%d", sra.Z(), sra.X(), sra.Y(), sra.I5())

    write(code, execution)
}

type Cmp struct { InstructionFormatU }

func (cmp *Cmp) Execute() {
    x := int64(cmp.RX.Get())
    y := int64(cmp.RY.Get())
    R := x - y

    cmp.LS, cmp.MS = uint32(R),  uint32(R >> 32)
}

func (cmp *Cmp) Status() {
    SR.ZN(cmp.LS == 0)

    SR.SN((cmp.LS >> 31) == 1)

    SR.CY(cmp.MS != 0)

    SR.OV((cmp.RX.Get() >> 31) != (cmp.RY.Get() >> 31) &&
   (cmp.LS >> 31) != (cmp.RX.Get()))
}

func (cmp *Cmp) Store() {}

func(cmp * Cmp) Print() {
    execution := fmt.Sprintf("SR=0x%08X", SR.Get())
    code := fmt.Sprintf("cmp r%d,r%d", cmp.X(), cmp.Y())

    write(code, execution)
}

type And struct { InstructionFormatU }

func (and *And) Execute() {
    and.LS = and.RX.Get() & and.RY.Get()
}

func (and *And) Status() {
    SR.ZN(and.LS == 0)

    SR.SN((and.LS >> 31) == 1)
}

func (and *And) Store() {
    and.RZ.Set(and.LS)
}

func(and * And) Print() {
    execution := fmt.Sprintf("R%d=R%d&R%d=0x%08X,SR=0x%08X", and.Z(), and.X(), and.Y(), and.RZ.Get(), SR.Get())
    code := fmt.Sprintf("and r%d,r%d,r%d", and.Z(), and.X(), and.Y())

    write(code, execution)
}

type Or struct { InstructionFormatU }

func (and *Or) Execute() {
    and.LS = and.RX.Get() | and.RY.Get()
}

func (and *Or) Status() {
    SR.ZN(and.LS == 0)

    SR.SN((and.LS >> 31) == 1)
}

func (and *Or) Store() {
    and.RZ.Set(and.LS)
}

func(and * Or) Print() {
    execution := fmt.Sprintf("R%d=R%d|R%d=0x%08X,SR=0x%08X", and.Z(), and.X(), and.Y(), and.RZ.Get(), SR.Get())
    code := fmt.Sprintf("or r%d,r%d,r%d", and.Z(), and.X(), and.Y())

    write(code, execution)
}

type Not struct { InstructionFormatU }

func (not *Not) Execute() {
    not.LS = ^not.RX.Get()
}

func (not *Not) Status() {
    SR.ZN(not.LS == 0)

    SR.SN((not.LS >> 31) == 1)
}

func (not *Not) Store() {
    not.RZ.Set(not.LS)
}

func(not * Not) Print() {
    execution := fmt.Sprintf("R%d=~R%d=0x%08X,SR=0x%08X", not.Z(), not.X(), not.RZ.Get(), SR.Get())
    code := fmt.Sprintf("not r%d,r%d", not.Z(), not.X())

    write(code, execution)
}

type Xor struct { InstructionFormatU }

func (xor *Xor) Execute() {
    xor.LS = xor.RX.Get() ^ xor.RY.Get()
}

func (xor *Xor) Status() {
    SR.ZN(xor.LS == 0)

    SR.SN((xor.LS >> 31) == 1)
}

func (xor *Xor) Store() {
    xor.RZ.Set(xor.LS)
}

func(xor * Xor) Print() {
    execution := fmt.Sprintf("R%d=R%d^R%d=0x%08X,SR=0x%08X", xor.Z(), xor.X(), xor.Y(), xor.RZ.Get(), SR.Get())
    code := fmt.Sprintf("xor r%d,r%d,r%d", xor.Z(), xor.X(), xor.Y())

    write(code, execution)
}

type Addi struct { InstructionFormatF }

//
func (addi *Addi) Execute() {
    addi.LS, addi.MS = bits.Add32(addi.RX.Get(), addi.I16s(), 0)
}

func (addi *Addi) Status() {
    SR.ZN(addi.LS == 0) 

    SR.SN((addi.LS >> 31) == 1)

    SR.CY(addi.MS != 0)

    SR.OV((addi.RX.Get() >> 31) == (addi.I16s() >> 31) &&
	  (addi.LS >> 31) != (addi.RX.Get()))
}

func (addi *Addi) Store() {
    addi.RZ.Set(addi.LS)
}

func(addi * Addi) Print() {
    execution := fmt.Sprintf("R%d=R%d+0x%08X=0x%08X,SR=0x%08X", addi.Z(), addi.X(), addi.I16s(), addi.RZ.Get(), SR.Get())
    code := fmt.Sprintf("addi r%d,r%d,%d", addi.Z(), addi.X(), int32(addi.I16s()))

    write(code, execution)
}

type Subi struct { InstructionFormatF }

func (subi *Subi) Execute() {
    comp_2 := ^subi.I16s() + 1
    subi.LS, subi.MS = bits.Add32(subi.RX.Get(), comp_2, 0)
}

func (subi *Subi) Status() {
    SR.ZN(subi.LS == 0)

    SR.SN((subi.LS >> 31) == 1)

    SR.CY(subi.MS == 1)

    SR.OV((subi.RX.Get() >> 31) != (subi.I16s() >> 31) &&
   (subi.LS >> 31) != (subi.RX.Get()))
}

func (subi *Subi) Store() {
    subi.RZ.Set(subi.LS)
}

func(subi * Subi) Print() {
    execution := fmt.Sprintf("R%d=R%d-0x%08X=0x%08X,SR=0x%08X", subi.Z(), subi.X(), subi.I16s(), subi.RZ.Get(), SR.Get())
    code := fmt.Sprintf("subi r%d,r%d,%d", subi.Z(), subi.X(), int32(subi.I16s()))

    write(code, execution)
}

type Muli struct { InstructionFormatF } 

// R[I] : R[z] = R[x] * R[y]
func (muli *Muli) Execute() {
    muli.MS, muli.LS = muli.Mul32(muli.RX.Get(), muli.I16s())
}

func (muli *Muli) Mul32(x, y uint32) (uint32, uint32) {
    tmp := int64(x) * int64(y)

    return uint32(tmp>>32), uint32(tmp)
}

func (muli *Muli) Status() {
    SR.ZN(muli.LS == 0 && muli.MS == 0)

    SR.OV((muli.MS) != 0)
}

func (muli *Muli) Store() {
    muli.RZ.Set(muli.LS)
}

func(muli * Muli) Print() {
    execution := fmt.Sprintf("R%d=R%d*0x%08X=0x%08X,SR=0x%08X", muli.Z(), muli.X(), muli.I16s(), muli.RZ.Get(), SR.Get())
    code := fmt.Sprintf("muli r%d,r%d,r%d,%d", muli.I5(),  muli.Z(), muli.X(), muli.I16s())

    write(code, execution)
}

type Divi struct { InstructionFormatF }

func (divi *Divi)Execute() {
    var x, y, lo int32

    x = int32(divi.RX.Get())
    y = int32(divi.I16s())

    if y != 0 {
	lo = x / y
    } else {
	lo = 0
    }

    divi.LS = uint32(lo) 
}

func (divi *Divi) Status() {
    SR.ZD(divi.I16s() == 0) 

    if divi.I16s() == 0 { return } // When has divi per zero only set ZD

    SR.ZN(divi.LS == 0)

    SR.OV(false)
}

func (divi *Divi) Store() {
    if SR.Get() != uint32(0x00000060){
	divi.RZ.Set(divi.LS)
    }
}

func(divi * Divi) Print() {
    var execution string
    var quo uint32 

    if SR.Get() != uint32(0x00000060){
	quo = divi.RZ.Get()
    } else {
	quo = divi.LS
    }

    execution = fmt.Sprintf("R%d=R%d/0x%08X=0x%08X,SR=0x%08X",
			      divi.Z(), divi.X(), divi.I16s(), quo, SR.Get())

    code := fmt.Sprintf("divi r%d,r%d,%d", divi.Z(), divi.X(), divi.I16s())

    write(code, execution)
}

type Modi struct { InstructionFormatF }

func (modi *Modi)Execute() {
    var x, y, lo int32

    x = int32(modi.RX.Get())
    y = int32(modi.I16s())

    if y != 0 {
	lo = x % y
    } else {
	lo = 0
    }

    modi.LS = uint32(lo) 
}

func (modi *Modi) Status() {
    SR.ZD(modi.I16s() == 0) 

    if modi.I16s() == 0 { return } // When has modi per zero only set ZD

    SR.ZN(modi.LS == 0)

    SR.OV(false)
}

func (modi *Modi) Store() {
    if SR.Get() != uint32(0x00000060){
	modi.RZ.Set(modi.LS)
    }
}

func(modi * Modi) Print() {
    var execution string
    var quo uint32 

    if SR.Get() != uint32(0x00000060){
	quo = modi.RZ.Get()
    } else {
	quo = modi.LS
    }

    execution = fmt.Sprintf("R%d=R%d%%0x%08X=0x%08X,SR=0x%08X",
			      modi.Z(), modi.X(), modi.I16s(), quo, SR.Get())

    code := fmt.Sprintf("modi r%d,r%d,%d", modi.Z(), modi.X(), modi.I16s())

    write(code, execution)
}

type Cmpi struct { InstructionFormatF }


func (cmpi *Cmpi) Execute() {
    x := int64(cmpi.RX.Get())
    y := int64(cmpi.I16s())
    R := x - y

    cmpi.LS, cmpi.MS = uint32(R),  uint32(R >> 32)
}

func (cmpi *Cmpi) Status() {
    SR.ZN(cmpi.LS == 0)

    SR.SN((cmpi.LS >> 31) == 1)

    SR.CY(cmpi.MS != 0)

    SR.OV((cmpi.RX.Get() >> 31) != (cmpi.I16s() >> 31) &&
   (cmpi.LS >> 31) != (cmpi.RX.Get()))
}

func (cmpi *Cmpi) Store() {}

func(cmpi * Cmpi) Print() {
    execution := fmt.Sprintf("SR=0x%08X", SR.Get())
    code := fmt.Sprintf("cmpi r%d,%d", cmpi.X(), cmpi.I16s())

    write(code, execution)
}

type L8 struct { InstructionFormatF }

func (l8 *L8) Execute() {
    l8.LS = l8.RX.Get() + l8.I16s()
}

func (l8 *L8) Status() {}

func (l8 *L8) Store() {
    l8.RZ.Set(uint32(Load8(l8.LS)))
}

func(l8 * L8) Print() {
    execution := fmt.Sprintf("R%d=MEM[0x%08X]=0x%02X",l8.Z(),l8.LS, l8.RZ.Get())
    code := fmt.Sprintf("l8 r%d,[r%d+%d]", l8.Z(), l8.X(), l8.I16s())

    write(code, execution)
}

type L16 struct { InstructionFormatF }

func (l16 *L16) Execute() {
    l16.LS = l16.RX.Get() + l16.I16s()
}

func (l16 *L16) Status() {}

func (l16 *L16) Store() {
    l16.RZ.Set(uint32(Load16(l16.LS)))
}

func(l16 * L16) Print() {
    execution := fmt.Sprintf("R%d=MEM[0x%08X]=0x%04X",l16.Z(),l16.LS, l16.RZ.Get())
    code := fmt.Sprintf("l16 r%d,[r%d+%d]", l16.Z(), l16.X(), l16.I16s())

    write(code, execution)
}

type L32 struct { InstructionFormatF }

func (l32 *L32) Execute() {
    l32.LS = l32.RX.Get() + l32.I16s()
}

func (l32 *L32) Status() {}

func (l32 *L32) Store() {
    l32.RZ.Set(Load32(l32.LS))
}

func(l32 * L32) Print() {
    execution := fmt.Sprintf("R%d=MEM[0x%08X]=0x%08X",l32.Z(),l32.LS, l32.RZ.Get())
    code := fmt.Sprintf("l32 r%d,[r%d+%d]", l32.Z(), l32.X(), l32.I16s())

    write(code, execution)
}

type S8 struct { InstructionFormatF }

func (s8 *S8) Execute() {
    s8.LS = s8.RX.Get() + s8.I16s()
}

func (s8 *S8) Status() {}

func (s8 *S8) Store() {
    Store8(s8.LS, uint8(s8.RZ.Get()))
}

func(s8 * S8) Print() {
    execution := fmt.Sprintf("MEM[0x%08X]=R%d=0x%02X",s8.LS, s8.Z(), uint8(s8.RZ.Get()))
    code := fmt.Sprintf("s8 [r%d+%d],r%d", s8.X(), s8.I16s(), s8.Z())

    write(code, execution)
}

type S16 struct { InstructionFormatF }

func (s16 *S16) Execute() {
    s16.LS = s16.RX.Get() + s16.I16s()
}

func (s16 *S16) Status() {}

func (s16 *S16) Store() {
    Store16(s16.LS, uint16(s16.RZ.Get()))
}

func(s16 * S16) Print() {
    execution := fmt.Sprintf("MEM[0x%08X]=R%d=0x%04X",s16.LS, s16.Z(), uint16(s16.RZ.Get()))
    code := fmt.Sprintf("s16 [r%d+%d],r%d", s16.X(), s16.I16s(), s16.Z())

    write(code, execution)
}

type S32 struct { InstructionFormatF }

func (s32 *S32) Execute() {
    s32.LS = s32.RX.Get() + s32.I16s()
}

func (s32 *S32) Status() {}

func (s32 *S32) Store() {
    Store32(s32.LS, uint32(s32.RZ.Get()))
}

func(s32 * S32) Print() {
    execution := fmt.Sprintf("MEM[0x%08X]=R%d=0x%08X",s32.LS, s32.Z(), uint32(s32.RZ.Get()))
    code := fmt.Sprintf("s32 [r%d+%d],r%d", s32.X(), s32.I16s(), s32.Z())

    write(code, execution)
}

type Bae struct { InstructionFormatS }

func (bae *Bae) Execute() {
    if !SR.Get_CY() {
	bae.Jump()
    }
}

func (bae *Bae) Status() {}

func (bae *Bae) Store() {}

func(bae *Bae) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bae.NAD)
    code := fmt.Sprintf("bae %d", bae.I26s())

    write(code, execution)
}

type Bat struct { InstructionFormatS }

func (bat *Bat) Execute() {
    if !(SR.Get_ZN() || SR.Get_CY()) {
	bat.Jump()
    }
}

func (bat *Bat) Status() {}

func (bat *Bat) Store() {}

func(bat *Bat) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bat.NAD)
    code := fmt.Sprintf("bat %d", bat.I26s())

    write(code, execution)
}

type Bbe struct { InstructionFormatS }

func (bbe *Bbe) Execute() {
    if SR.Get_ZN() || SR.Get_CY() {
	bbe.Jump()
    }
}

func (bbe *Bbe) Status() {}

func (bbe *Bbe) Store() {}

func(bbe *Bbe) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bbe.NAD)
    code := fmt.Sprintf("bbe %d", bbe.I26s())

    write(code, execution)
}

type Bbt struct { InstructionFormatS }

func (bbt *Bbt) Execute() {
    if SR.Get_CY() {
	bbt.Jump()
    }
}

func (bbt *Bbt) Status() {}

func (bbt *Bbt) Store() {}

func(bbt *Bbt) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bbt.NAD)
    code := fmt.Sprintf("bbt %d", bbt.I26s())

    write(code, execution)
}

type Beq struct { InstructionFormatS }

func (beq *Beq) Execute() {
    if SR.Get_ZN() {
	beq.Jump()
    }
}

func (beq *Beq) Status() {}

func (beq *Beq) Store() {}

func(beq *Beq) Print() {
    execution := fmt.Sprintf("PC=0x%08X", beq.NAD)
    code := fmt.Sprintf("beq %d", beq.I26s())

    write(code, execution)
}

type Bge struct { InstructionFormatS }

func (bge *Bge) Execute() {
    if SR.Get_SN() == SR.Get_OV() {
	bge.Jump()
    }
}

func (bge *Bge) Status() {}

func (bge *Bge) Store() {}

func(bge *Bge) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bge.NAD)
    code := fmt.Sprintf("bge %d", bge.I26s())

    write(code, execution)
}

type Bgt struct { InstructionFormatS }

func (bgt *Bgt) Execute() {
    if !SR.Get_ZN() && SR.Get_SN() == SR.Get_OV() {
	bgt.Jump()
    }
}

func (bgt *Bgt) Status() {}

func (bgt *Bgt) Store() {}

func(bgt *Bgt) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bgt.NAD)
    code := fmt.Sprintf("bgt %d", bgt.I26s())

    write(code, execution)
}

type Biv struct { InstructionFormatS }

func (biv *Biv) Execute() {
    biv.Jump()
}

func (biv *Biv) Status() {}

func (biv *Biv) Store() {}

func(biv *Biv) Print() {
    execution := fmt.Sprintf("PC=0x%08X", biv.NAD)
    code := fmt.Sprintf("biv %d", biv.I26s())

    write(code, execution)
}

type Ble struct { InstructionFormatS }

func (ble *Ble) Execute() {
    if SR.Get_ZN() && SR.Get_SN() != SR.Get_OV() {
	ble.Jump()
    }
}

func (ble *Ble) Status() {}

func (ble *Ble) Store() {}

func(ble *Ble) Print() {
    execution := fmt.Sprintf("PC=0x%08X", ble.NAD)
    code := fmt.Sprintf("ble %d", ble.I26s())

    write(code, execution)
}


type Blt struct { InstructionFormatS }

func (blt *Blt) Execute() {
    if SR.Get_SN() != SR.Get_OV() {
	blt.Jump()
    }
}

func (blt *Blt) Status() {}

func (blt *Blt) Store() {}

func(blt *Blt) Print() {
    execution := fmt.Sprintf("PC=0x%08X", blt.NAD)
    code := fmt.Sprintf("blt %d", blt.I26s())

    write(code, execution)
}

type Bne struct { InstructionFormatS }

func (bne *Bne) Execute() {
    if !SR.Get_ZN() {
	bne.Jump()
    }
}

func (bne *Bne) Status() {}

func (bne *Bne) Store() {}

func(bne *Bne) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bne.NAD)
    code := fmt.Sprintf("bne %d", bne.I26s())

    write(code, execution)
}

type Bni struct { InstructionFormatS }

func (bni *Bni) Execute() {
    if false { // FIX: Missing definition field IV
	bni.Jump()
    }
}

func (bni *Bni) Status() {}

func (bni *Bni) Store() {}

func(bni *Bni) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bni.NAD)
    code := fmt.Sprintf("bni %d", bni.I26s())

    write(code, execution)
}

type Bnz struct { InstructionFormatS }

func (bnz *Bnz) Execute() {
    if !SR.Get_ZD() { // FIX: Missing definition field IV
	bnz.Jump()
    }
}

func (bnz *Bnz) Status() {}

func (bnz *Bnz) Store() {}

func(bnz *Bnz) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bnz.NAD)
    code := fmt.Sprintf("bnz %d", bnz.I26s())

    write(code, execution)
}

type Bun struct { InstructionFormatS }

func (bun *Bun) Execute() {
    bun.Jump()
}

func (bun *Bun) Status() {}

func (bun *Bun) Store() {}

func(bun *Bun) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bun.NAD)
    code := fmt.Sprintf("bun %d", bun.I26s())

    write(code, execution)
}

type Bzd struct { InstructionFormatS }

func (bzd *Bzd) Execute() {
    if SR.Get_ZD() {
	bzd.Jump()
    }
}

func (bzd *Bzd) Status() {}

func (bzd *Bzd) Store() {}

func(bzd *Bzd) Print() {
    execution := fmt.Sprintf("PC=0x%08X", bzd.NAD)
    code := fmt.Sprintf("bzd %d", bzd.I26s())

    write(code, execution)
}

type Int struct { InstructionFormatS }

func (int *Int) Execute() {
    if int.I16() == 0 {
        int.NAD = 0
    }
}

func (int *Int) Shutdown() bool {
    return int.I16() == 0
}
func (int *Int) Status() {}

func (int *Int) Store() {}

func(int *Int) Print() {
    execution := fmt.Sprintf("CR=0x%08X,PC=0x%08X", 0, int.NAD) // FIX: missing definition for register CR
    code := fmt.Sprintf("int %d", int.I26())

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

    for {
	IR.Load()

	if IR.NOP() {
	    continue
	}

	exececutable := INSTRUCTION.Get()

	if exececutable == nil {
	    fmt.Printf("[INVALID INSTRUCTION @ 0x%08X]\n", IR.Get())
	    break
	}

	exececutable.New()

	exececutable.Execute()

	exececutable.Status()

	exececutable.Store()

	exececutable.Print()

	executableFormatS, ok := exececutable.(ExecutableFormatS)

	if ok {
	    executableFormatS.PC()
	} else {
	    PC.data += 4
	}

	exececutableInt, ok := exececutable.(*Int)

	if ok {
	    if exececutableInt.Shutdown() {
	        break
	    }
	} 
    }

    fmt.Println("[END OF SIMULATION]");
}
