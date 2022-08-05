package main_test

import (
	"io/ioutil"
	"os"

	. "github.com/onsi/ginkgo/v2"
	. "github.com/onsi/gomega"

	"github.com/DaniloJNS/main"
)

var _ = Describe("InstructionsF", func() {
	var stdout *os.File
	var instruction main.Executable
	var PC, IR, SR uint32
	r, w, _ := os.Pipe()

	BeforeEach(func() {
		stdout = os.Stdout
		os.Stdout = w
		main.Setup_registers()
	})

	JustBeforeEach(func() {
		main.PC.Set(PC) 
		main.IR.Set(IR)

		instruction.New()
		instruction.Execute()
		instruction.Status()
		instruction.Store()

		SR = main.SR.Get()
	})

	AfterEach(func() {
		main.SR.Set(uint32(0)) 
		r, w, _ = os.Pipe() // Reset PIPE
		os.Stdout = stdout
	})

	Describe("Mov", func() {
		var mov main.Mov

		BeforeEach(func() {
			instruction = &mov
			PC = 0x00000008
			IR = 0x336AC9D
		})

		Context("#Execute", func() {
			It("result operation must be equal LS", func() {
				Expect(mov.LS).To(Equal(uint32(0x0016AC9D)))
			})
		})
		
		Context("#Status", func() {
			It("must not change SR", func() {
				Expect(SR).To(Equal(uint32(0)))
			})
		})

		Context("#Store", func() {
			It("should write LS in RZ", func() {
				Expect(main.R[mov.Z()].Get()).To(Equal(uint32(0x0016AC9D)))
			})
		})

		Context("#Print", func() {
			It("Should view return instruction", func() {
				instruction.Print()
				w.Close()
				message, _ := ioutil.ReadAll(r)

				Expect(string(message)).To(Equal("0x00000008:\tmov r25, 1485981         \tR25=0x0016AC9D\n"))
			})
		})

		When("RZ is Read Only", func() {
			BeforeEach(func() {
				IR = 0x0016AC9D
			})

			Context("#Execute", func() {
				It("result operation must be write in LS", func() {
					Expect(mov.LS).To(Equal(uint32(0x0016AC9D)))
				})
			})
			
			Context("#Status", func() {
				It("must not change SR", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Store", func() {
				It("should not write LS in RZ", func() {
					Expect(main.R[mov.Z()].Get()).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction", func() {
					instruction.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					// DUVIDA: RZ ou LS
					Expect(string(message)).To(Equal("0x00000008:\tmov r0, 0                \tR0=0x00000000\n"))
				})
			})
		})
	})

	Describe("Movs", func() {
		var movs main.Movs

		When("number is negative", func() {
			BeforeEach(func() {
				instruction = &movs
				PC = 0x0000008
				IR = 0x336AC9D
			})

			Context("#Execute", func() {
				It("result operation must be write in LS", func() {
					Expect(movs.LS).To(Equal(uint32(0xFFF6AC9D)))
				})
			})
			
			Context("#Status", func() {
				It("must not change SR", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Store", func() {
				It("should write value LS in RZ", func() {
					Expect(main.R[movs.Z()].Get()).To(Equal(uint32(0xFFF6AC9D)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction", func() {
					movs.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmovs r25, -611171        \tR25=0xFFF6AC9D\n"))
				})
			})
		})

		When("number is positive", func() {
			BeforeEach(func() {
				instruction = &movs
				PC = 0x00000008
				IR = 0x0326AC9D
			})

			Context("#Execute", func() {
				It("result operation must be equal LS without extend signal", func() {
					Expect(movs.LS).To(Equal(uint32(0x0006AC9D)))
				})
			})

			Context("#Status", func() {
				It("must not change SR", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[movs.Z()].Get()).To(Equal(uint32(0x0006AC9D)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction", func() {
					movs.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmovs r25, 437405         \tR25=0x0006AC9D\n"))
				})
			})
		})

		When("RZ is Read Only", func() {
			BeforeEach(func() {
				IR = 0x0406AC9D
			})

			Context("#Execute", func() {
				It("result operation must be equal LS", func() {
					Expect(movs.LS).To(Equal(uint32(0x0006AC9D)))
				})
			})
			
			Context("#Status", func() {
				It("must not change SR", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Store", func() {
				It("should not write LS in RZ", func() {
					Expect(main.R[movs.Z()].Get()).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction", func() {
					instruction.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					// DUVIDA: RZ ou LS
					Expect(string(message)).To(Equal("0x00000008:\tmovs r0, 0               \tR0=0x00000000\n"))
				})
			})
		})
	})

	Describe("Add", func() {
		var add main.Add

		BeforeEach(func() {
			instruction = &add
			PC = 0x00000008
			IR = 0x08611000
		})


		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0)
				main.R[2].Set(0)
			})

			Context("#Execute", func() {
				It("LS must be zero", func() {
					Expect(add.LS).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					add.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tadd r3,r1,r2             \tR3=R1+R2=0x00000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has carry", func() {
			BeforeEach(func() {
				main.R[1].Set(0xFFFF0000)
				main.R[2].Set(0x00012345)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(add.LS).To(Equal(uint32(0x00002345)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00002345)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					add.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tadd r3,r1,r2             \tR3=R1+R2=0x00002345,SR=0x00000001\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})

				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(add.LS).To(Equal(uint32(0x00002345)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})

				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						add.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tadd r0,r1,r2             \tR0=R1+R2=0x00000000,SR=0x00000001\n"))
					})
				})
			})
		})

		When("Result has signal", func() {
			BeforeEach(func() {
				main.R[1].Set(0x80000000)
				main.R[2].Set(0x00012345)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(add.LS).To(Equal(uint32(0x80012345)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000010)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x80012345)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					add.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tadd r3,r1,r2             \tR3=R1+R2=0x80012345,SR=0x00000010\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})

				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(add.LS).To(Equal(uint32(0x80012345)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000010)))
					})
				})

				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						add.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tadd r0,r1,r2             \tR0=R1+R2=0x00000000,SR=0x00000010\n"))
					})
				})
			})
		})

		When("Result has overflow with signal", func() {
			BeforeEach(func() {
				main.R[1].Set(0x7FFFFFFF)
				main.R[2].Set(0x7FFFFFFF)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(add.LS).To(Equal(uint32(0xFFFFFFFE)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY and OV", func() {
					Expect(SR).To(Equal(uint32(0x00000018)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[add.Z()].Get()).To(Equal(uint32(0xFFFFFFFE)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					add.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tadd r3,r1,r2             \tR3=R1+R2=0xFFFFFFFE,SR=0x00000018\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})

				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(add.LS).To(Equal(uint32(0xFFFFFFFE)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY and OV", func() {
						Expect(SR).To(Equal(uint32(0x00000018)))
					})
				})

				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						add.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tadd r0,r1,r2             \tR0=R1+R2=0x00000000,SR=0x00000018\n"))
					})
				})
			})
		})

		When("Result has overflow with carry and signal", func() {
			BeforeEach(func() {
				main.R[1].Set(0xFFFFFFFF)
				main.R[2].Set(0xFFFFFFFF)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(add.LS).To(Equal(uint32(0xFFFFFFFE)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000019)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[add.Z()].Get()).To(Equal(uint32(0xFFFFFFFE)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					add.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tadd r3,r1,r2             \tR3=R1+R2=0xFFFFFFFE,SR=0x00000019\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})

				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(add.LS).To(Equal(uint32(0xFFFFFFFE)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000019)))
					})
				})

				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[add.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						add.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tadd r0,r1,r2             \tR0=R1+R2=0x00000000,SR=0x00000019\n"))
					})
				})
			})
		})
	})

	Describe("Sub", func() {
		var sub main.Sub

		JustBeforeEach(func() {
			instruction = &sub
			PC = 0x00000008
			IR = 0x08611000
		})

		AfterEach(func() {
			main.SR.Set(uint32(0) )
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0)
				main.R[2].Set(0)
			})

			Context("#Execute", func() {
				It("LS must be diferent equal zero", func() {
					Expect(sub.LS).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})

			Context("#Store", func() {
				It("should write LS in RZ", func() {
					Expect(main.R[sub.Z()].Get()).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1-R2=0x00000000,SR=0x00000040\n"))
				})
			})

		})

		When("Result has overflow", func() {
			BeforeEach(func() {
					main.R[1].Set(0x80000000)
					main.R[2].Set(0x7FFFFFFF)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(sub.LS).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate OV", func() {
					Expect(SR).To(Equal(uint32(0x00000008)))
				})
			})

			Context("#Store", func() {
				It("should not write LS in RZ", func() {
					Expect(main.R[sub.Z()].Get()).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with OV in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1-R2=0x00000001,SR=0x00000008\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})
			
				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(sub.LS).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate OV", func() {
						Expect(SR).To(Equal(uint32(0x00000008)))
					})
				})
			
				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[sub.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sub.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsub r0,r1,r2             \tR0=R1-R2=0x00000000,SR=0x00000008\n"))
					})
				})
			})
		})

		When("Result has signal and overflow", func() {
			BeforeEach(func() {
				main.R[1].Set(0xFFFFFFFE)
				main.R[2].Set(0x00000001)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(sub.LS).To(Equal(uint32(0xFFFFFFFD)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate OV and SN", func() {
					Expect(SR).To(Equal(uint32(0x000000018)))
				})
			})

			Context("#Store", func() {
				It("should not write LS in RZ", func() {
					Expect(main.R[sub.Z()].Get()).To(Equal(uint32(0xFFFFFFFD)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1-R2=0xFFFFFFFD,SR=0x00000018\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})
			
				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(sub.LS).To(Equal(uint32(0xFFFFFFFD)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate OV", func() {
						Expect(SR).To(Equal(uint32(0x00000018)))
					})
				})
			
				Context("#Store", func() {
					It("should not write LS in RZ", func() {
						Expect(main.R[sub.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sub.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsub r0,r1,r2             \tR0=R1-R2=0x00000000,SR=0x00000018\n"))
					})
				})
			})
		})
	})

	Describe("Mul", func() {
		var mul main.Mul

		JustBeforeEach(func() {
			instruction = &mul
			PC = 0x00000008
			// OP = 00010
			// Z = 00111
			// I = 00011
			// X = 00001
			// X = 00010
			IR = 0x10E11003
		})

		AfterEach(func() {
			main.SR.Set(uint32(0))
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0)
				main.R[2].Set(0x5)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(mul.LS).To(Equal(uint32(0x00000000)))
					Expect(mul.MS).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[mul.Z()].Get()).To(Equal(uint32(0x00000000)))
					Expect(main.R[mul.I5()].Get()).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					mul.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tmul r3,r7,r1,r2          \tR3:R7=R1*R2=0x0000000000000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has not status", func() {
			BeforeEach(func() {
					main.R[1].Set(0x00000010)
					main.R[2].Set(0x00000010)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(mul.LS).To(Equal(uint32(0x00000100)))
					Expect(mul.MS).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate nothing", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[mul.Z()].Get()).To(Equal(uint32(0x00000100)))
					Expect(main.R[mul.I5()].Get()).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction without toggle", func() {
					mul.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmul r3,r7,r1,r2          \tR3:R7=R1*R2=0x0000000000000100,SR=0x00000000\n"))
				})
			})
		})

		When("Result has carry", func() {
			BeforeEach(func() {
				main.R[1].Set(0xFFFFFFFE)
				main.R[2].Set(0x00000011)
			})

			Context("#Execute", func() {
				It("LS must be diferent of zero", func() {
					Expect(mul.LS).To(Equal(uint32(0xFFFFFFDE)))
					Expect(mul.MS).To(Equal(uint32(0x000000010)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[mul.Z()].Get()).To(Equal(uint32((0xFFFFFFDE))))
					Expect(main.R[mul.I5()].Get()).To(Equal(uint32(0x000000010)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					mul.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmul r3,r7,r1,r2          \tR3:R7=R1*R2=0x00000010FFFFFFDE,SR=0x00000001\n"))
				})
			})

			When("And RZ and RI is Read Only", func() {
				BeforeEach(func() {
					IR = 0x08011000
				})
			
				Context("#Execute", func() {
					It("LS must be diferent of zero", func() {
						Expect(mul.LS).To(Equal(uint32(0xFFFFFFDE)))
						Expect(mul.MS).To(Equal(uint32(0x000000010)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write LS in RZ and MS in RI", func() {
						Expect(main.R[mul.Z()].Get()).To(Equal(uint32((0x00000000))))
						Expect(main.R[mul.I5()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						mul.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tmul r0,r0,r1,r2          \tR0:R0=R1*R2=0x0000000000000000,SR=0x00000001\n"))
					})
				})
			})

		})
	})

	Describe("Sll", func() {
		var sll main.Sll

		JustBeforeEach(func() {
			instruction = &sll
			PC = 0x00000008
			// OP.Set(000100)
			// Z.Set(00111)
			// I.Set(00011)
			// X.Set(00001)
			// Y.Set(00010)
			IR = 0x10E11103
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0x00000000)
				main.R[2].Set(0x00000000)
				main.R[7].Set(0x00000000)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent equal zero", func() {
					Expect(sll.LS).To(Equal(uint32(0x000000000)))
					Expect(sll.MS).To(Equal(uint32(0x000000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x000000000))))
					Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x000000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sll.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tsll r7,r1,r2,3           \tR7:R1=R7:R2<<3=0x0000000000000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has not status", func() {
			BeforeEach(func() {
					main.R[2].Set(0x00000001) // Y
					main.R[7].Set(0x00000000) // Z
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(sll.MS).To(Equal(uint32(0x000000000)))
					Expect(sll.LS).To(Equal(uint32(0x000000008)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate nothing", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x000000000))))
					Expect(main.R[sll.I5()].Get()).To(Equal(uint32(0x000000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction without SR toggle", func() {
					sll.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsll r7,r1,r2,3           \tR7:R1=R7:R2<<3=0x0000000000000008,SR=0x00000000\n"))
				})
			})
		})

		When("Result has carry because Rz is greater than 0", func() {
			BeforeEach(func() {
				main.R[2].Set(0x00000001)
				main.R[7].Set(0x00000001)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(sll.MS).To(Equal(uint32(0x000000008)))
					Expect(sll.LS).To(Equal(uint32(0x000000008)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x000000008))))
					Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x000000008)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with CY in SR toggle", func() {
					sll.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsll r7,r1,r2,3           \tR7:R1=R7:R2<<3=0x0000000800000008,SR=0x00000001\n"))
				})
			})

			When("And RX is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10E01103
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS diferent a zero", func() {
						Expect(sll.MS).To(Equal(uint32(0x000000008)))
						Expect(sll.LS).To(Equal(uint32(0x000000008)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x00000008))))
						Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sll.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsll r7,r0,r2,3           \tR7:R0=R7:R2<<3=0x0000000800000000,SR=0x00000001\n"))
					})
				})
			})
		})

		When("Result has carry because RY overflow", func() {
			BeforeEach(func() {
				main.R[2].Set(0x80000001)
				main.R[7].Set(0x1)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(sll.MS).To(Equal(uint32(0x00000000C)))
					Expect(sll.LS).To(Equal(uint32(0x000000008)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should write LS in RZ and MS in RI", func() {
					Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x00000000C))))
					Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x000000008)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sll.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsll r7,r1,r2,3           \tR7:R1=R7:R2<<3=0x0000000C00000008,SR=0x00000001\n"))
				})
			})

			When("And RX is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10E01103
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS equal a zero", func() {
						Expect(sll.MS).To(Equal(uint32(0x00000000C)))
						Expect(sll.LS).To(Equal(uint32(0x000000008)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x0000000C))))
						Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sll.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsll r7,r0,r2,3           \tR7:R0=R7:R2<<3=0x0000000C00000000,SR=0x00000001\n"))
					})
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10011103
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS too", func() {
						Expect(sll.MS).To(Equal(uint32(0x000000004)))
						Expect(sll.LS).To(Equal(uint32(0x000000008)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[sll.Z()].Get()).To(Equal(uint32((0x00000000))))
						Expect(main.R[sll.X()].Get()).To(Equal(uint32(0x00000008)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sll.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsll r0,r1,r2,3           \tR0:R1=R0:R2<<3=0x0000000000000008,SR=0x00000001\n"))
					})
				})
			})
		})
	})

	Describe("Muls", func() {
		var muls main.Muls

		JustBeforeEach(func() {
			instruction = &muls
			PC = 0x00000008
			// OP.Set(000100    )
			// Z.Set(00111     )
			// I.Set(00011     )
			// X.Set(00001     )
			// X.Set(00010     )
			IR = 0x10E11003
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0x00000000)
				main.R[2].Set(0x00000005)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(muls.MS).To(Equal(uint32(0x000000000)))
					Expect(muls.LS).To(Equal(uint32(0x000000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[muls.Z()].Get()).To(Equal(uint32((0x00000000))))
					Expect(main.R[muls.X()].Get()).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					muls.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tmuls r3,r7,r1,r2         \tR3:R7=R1*R2=0x0000000000000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has not status", func() {
			BeforeEach(func() {
					main.R[1].Set(0x00000010)
					main.R[2].Set(0x00000010)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(muls.MS).To(Equal(uint32(0x000000000)))
					Expect(muls.LS).To(Equal(uint32(0x000000100)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate ZN", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[muls.I5()].Get()).To(Equal(uint32(0x00000000)))
					Expect(main.R[muls.Z()].Get()).To(Equal(uint32((0x00000100))))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					muls.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmuls r3,r7,r1,r2         \tR3:R7=R1*R2=0x0000000000000100,SR=0x00000000\n"))
				})
			})
		})

		When("Result has carry", func() {
			BeforeEach(func() {
				main.R[1].Set(0xFFFFFFFE)
				main.R[2].Set(0x00000011)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(muls.MS).To(Equal(uint32(0x000000010)))
					Expect(muls.LS).To(Equal(uint32(0xFFFFFFDE)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[muls.I5()].Get()).To(Equal(uint32((0x00000010))))
					Expect(main.R[muls.Z()].Get()).To(Equal(uint32(0xFFFFFFDE)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					muls.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmuls r3,r7,r1,r2         \tR3:R7=R1*R2=0x00000010FFFFFFDE,SR=0x00000001\n"))
				})
			})

			When("And RI is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10E11000
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS equal a zero", func() {
						Expect(muls.MS).To(Equal(uint32(0x000000010)))
						Expect(muls.LS).To(Equal(uint32(0xFFFFFFDE)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[muls.I5()].Get()).To(Equal(uint32(0x00000000)))
						Expect(main.R[muls.Z()].Get()).To(Equal(uint32(0xFFFFFFDE)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						muls.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tmuls r0,r7,r1,r2         \tR0:R7=R1*R2=0x00000000FFFFFFDE,SR=0x00000001\n"))
					})
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10011003
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS equal a zero", func() {
						Expect(muls.MS).To(Equal(uint32(0x000000010)))
						Expect(muls.LS).To(Equal(uint32(0xFFFFFFDE)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[muls.I5()].Get()).To(Equal(uint32(0x00000010)))
						Expect(main.R[muls.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						muls.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tmuls r3,r0,r1,r2         \tR3:R0=R1*R2=0x0000001000000000,SR=0x00000001\n"))
					})
				})
			})
		})
	})
	
	Describe("Sla", func() {
		var sla main.Sla

		JustBeforeEach(func() {
			instruction = &sla
			PC = 0x00000008
			// OP = 0001
			// Z = 0011
			// I = 0001
			// X = 0000
			// Y = 0001
			IR = 0x10E11103
		})

		AfterEach(func() {
			main.SR.Set(uint32(0) )
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1].Set(0)
				main.R[2].Set(0)
				main.R[7].Set(0)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(sla.MS).To(Equal(uint32(0x000000000)))
					Expect(sla.LS).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000040)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[sla.I5()].Get()).To(Equal(uint32(0000000000)))
					Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sla.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tsla r7,r1,r2,3           \tR7:R1=R7:R2<<4=0x0000000000000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has not status", func() {
			BeforeEach(func() {
					main.R[2].Set(0x00000001) // Y
					main.R[7].Set(0x00000000)  // Z  
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS equal a zero", func() {
					Expect(sla.MS).To(Equal(uint32(0x00000000)))
					Expect(sla.LS).To(Equal(uint32(0x00000010)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate nothing", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0000000000)))
					Expect(main.R[sla.X()].Get()).To(Equal(uint32(0x00000010)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sla.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsla r7,r1,r2,3           \tR7:R1=R7:R2<<4=0x0000000000000010,SR=0x00000000\n"))
				})
			})
		})
		
		When("Result has carry because Desc with overflow", func() {
			BeforeEach(func() {
				IR = 0x1001110A
				main.R[0].Set(0x00000000)
				main.R[2].Set(0xFFF00000)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS diferent of zero", func() {
					Expect(sla.MS).To(Equal(uint32(0x000007FF)))
					Expect(sla.LS).To(Equal(uint32(0x80000000)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0000000000)))
					Expect(main.R[sla.X()].Get()).To(Equal(uint32(0x80000000)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sla.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsla r0,r1,r2,10          \tR0:R1=R0:R2<<11=0x0000000080000000,SR=0x00000001\n"))
				})
			})

		})

		When("Result has carry because Rz is greater than 0", func() {
			BeforeEach(func() {
				main.R[2].Set(0x1)
				main.R[7].Set(0x1)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS diferent of zero", func() {
					Expect(sla.MS).To(Equal(uint32(0x00000010)))
					Expect(sla.LS).To(Equal(uint32(0x00000010)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0x00000010)))
					Expect(main.R[sla.X()].Get()).To(Equal(uint32(0x00000010)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sla.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsla r7,r1,r2,3           \tR7:R1=R7:R2<<4=0x0000001000000010,SR=0x00000001\n"))
				})
			})
		})

		When("Result has carry because RY overflow", func() {
			BeforeEach(func() {
				main.R[2].Set(0x80000001)
				main.R[7].Set(0x1)
			})
			
			Context("#Execute", func() {
				It("LS must be diferent zero and MS diferent of zero", func() {
					Expect(sla.MS).To(Equal(uint32(0x00000018)))
					Expect(sla.LS).To(Equal(uint32(0x00000010)))
				})
			})
			
			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0x00000018)))
					Expect(main.R[sla.X()].Get()).To(Equal(uint32(0x00000010)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sla.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsla r7,r1,r2,3           \tR7:R1=R7:R2<<4=0x0000001800000010,SR=0x00000001\n"))
				})
			})

			When("And RZ is Read Only", func() {
				BeforeEach(func() {
					IR = 0x10011003
					main.R[2].Set(0x80000001)
				})
			
				Context("#Execute", func() {
					It("LS must be diferent zero and MS equal a zero", func() {
						Expect(sla.MS).To(Equal(uint32(0x000000008)))
						Expect(sla.LS).To(Equal(uint32(0x00000010)))
					})
				})
				
				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})
				
				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[sla.X()].Get()).To(Equal(uint32(0x00000010)))
						Expect(main.R[sla.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})
			
				Context("#Print", func() {
					It("Should view return instruction with SN in SR toggle", func() {
						sla.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
			
						Expect(string(message)).To(Equal("0x00000008:\tsla r0,r1,r2,3           \tR0:R1=R0:R2<<4=0x0000000000000010,SR=0x00000001\n"))
					})
				})
			})
		})
	})

	Describe("Div", func() {
		var div main.Div

		JustBeforeEach(func() {
			instruction = &div
			PC = 0x00000008
			// OP.Set(000100    )
			// Z.Set(00111     )
			// I.Set(00011     )
			// X.Set(00001     )
			// Y.Set(00010     )
			IR = 0x10E11003
		})

		Context("Result is zero", func() {
			When("RX is zero", func() {
				BeforeEach(func() {
					main.R[1].Set(0x00000000)
					main.R[2].Set(0x00000005)
				})

				Context("#Execute", func() {
					It("LS must be diferent zero and MS diferent of zero", func() {
						Expect(div.MS).To(Equal(uint32(0x0000000)))
						Expect(div.LS).To(Equal(uint32(0x0000000)))
					})
				})

				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000040)))
					})
				})

				Context("#Store", func() {
					It("should write MS in RI and LS in RZ", func() {
						Expect(main.R[div.Z()].Get()).To(Equal(uint32(0x00000000)))
						Expect(main.R[div.I5()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with ZN in SR toggle", func() {
						div.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
						Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r7,r1,r2          \tR3=R1%R2=0x00000000,R7=R1/R2=0x00000000,SR=0x00000040\n"))
					})
				})
			})

			When("RY is zero", func() {
				BeforeEach(func() {
					main.R[1].Set(0x00000005)
					main.R[2].Set(0x00000000)
					main.R[7].Set(0xF0000000)
					main.R[3].Set(0xF0000000)
				})

				Context("#Execute", func() {
					It("LS must be diferent zero and MS diferent of zero", func() {
						Expect(div.MS).To(Equal(uint32(0x00000000)))
						Expect(div.LS).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000060)))
					})
				})

				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[div.Z()].Get()).To(Equal(uint32(0xF0000000)))
						Expect(main.R[div.I5()].Get()).To(Equal(uint32(0xF0000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with ZN in SR toggle", func() {
						div.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)
						Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r7,r1,r2          \tR3=R1%R2=0x00000000,R7=R1/R2=0x00000000,SR=0x00000060\n"))
					})
				})
			})
		})

		When("Result has not status", func() {
			BeforeEach(func() {
					main.R[1].Set(0x00000010)
					main.R[2].Set(0x00000010)
			})

			Context("#Execute", func() {
				It("LS must be diferent zero and MS diferent of zero", func() {
					Expect(div.MS).To(Equal(uint32(0x00000000)))
					Expect(div.LS).To(Equal(uint32(0x00000001)))
				})
			})

			Context("#Status", func() {
				It("must indicate CY", func() {
					Expect(SR).To(Equal(uint32(0x00000000)))
				})
			})

			Context("#Store", func() {
				It("should not write MS in RZ and LS in RX", func() {
					Expect(main.R[div.I5()].Get()).To(Equal(uint32(0x00000000)))
					Expect(main.R[div.Z()].Get()).To(Equal(uint32(0x00000001)))
				})
			})

			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					div.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r7,r1,r2          \tR3=R1%R2=0x00000000,R7=R1/R2=0x00000001,SR=0x00000000\n"))
				})
			})
		})

		Context("Result has carry", func() {
			When("RY is greater than RX", func() {
				BeforeEach(func() {
					main.R[1].Set(1)
					main.R[2].Set(5)
				})

				Context("#Execute", func() {
					It("LS must be diferent zero and MS diferent of zero", func() {
						Expect(div.MS).To(Equal(uint32(0x00000001)))
						Expect(div.LS).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000041)))
					})
				})

				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[div.I5()].Get()).To(Equal(uint32(0x00000001)))
						Expect(main.R[div.Z()].Get()).To(Equal(uint32(0x00000000)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with ZN in SR toggle", func() {
						div.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r7,r1,r2          \tR3=R1%R2=0x00000001,R7=R1/R2=0x00000000,SR=0x00000041\n"))
					})
				})
			})

			When("RX is not a multiple of RY", func() {
				BeforeEach(func() {
					main.R[1].Set(9)
					main.R[2].Set(5)
				})

				Context("#Execute", func() {
					It("LS must be diferent zero and MS diferent of zero", func() {
						Expect(div.MS).To(Equal(uint32(0x00000004)))
						Expect(div.LS).To(Equal(uint32(0x00000001)))
					})
				})

				Context("#Status", func() {
					It("must indicate CY", func() {
						Expect(SR).To(Equal(uint32(0x00000001)))
					})
				})

				Context("#Store", func() {
					It("should not write MS in RZ and LS in RX", func() {
						Expect(main.R[div.I5()].Get()).To(Equal(uint32(0x00000004)))
						Expect(main.R[div.Z()].Get()).To(Equal(uint32(0x00000001)))
					})
				})

				Context("#Print", func() {
					It("Should view return instruction with ZN in SR toggle", func() {
						div.Print()
						w.Close()
						message, _ := ioutil.ReadAll(r)

						Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r7,r1,r2          \tR3=R1%R2=0x00000004,R7=R1/R2=0x00000001,SR=0x00000001\n"))
					})
				})

				When("And RZ is Read Only", func() {
					BeforeEach(func() {
						IR = 0x10011003
					})
				
					Context("#Execute", func() {
						It("LS must be diferent zero and MS diferent of zero", func() {
							Expect(div.MS).To(Equal(uint32(0x00000004)))
							Expect(div.LS).To(Equal(uint32(0x00000001)))
						})
					})

					Context("#Status", func() {
						It("must indicate CY", func() {
							Expect(SR).To(Equal(uint32(0x00000001)))
						})
					})
					
					Context("#Store", func() {
						It("should not write MS in RZ and LS in RX", func() {
							Expect(main.R[div.I5()].Get()).To(Equal(uint32(0x00000004)))
							Expect(main.R[div.Z()].Get()).To(Equal(uint32(0x00000000)))
						})
					})
				
					Context("#Print", func() {
						It("Should view return instruction with SN in SR toggle", func() {
							div.Print()
							w.Close()
							message, _ := ioutil.ReadAll(r)
				
							Expect(string(message)).To(Equal("0x00000008:\tdiv r3,r0,r1,r2          \tR3=R1%R2=0x00000004,R0=R1/R2=0x00000000,SR=0x00000001\n"))
						})
					})
				})
			})
		})
	})
})
