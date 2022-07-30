package main_test

import (
	"io/ioutil"
	"os"

	. "github.com/onsi/ginkgo/v2"
	. "github.com/onsi/gomega"

	"github.com/DaniloJNS/main"
)

var _ = Describe("InstructionsF", func() {
	r, w, _ := os.Pipe()

	var stdout *os.File

	BeforeEach(func() {
		stdout = os.Stdout
		os.Stdout = w
	})

	AfterEach(func() {
		r, w, _ = os.Pipe() // Reset PIPE
		os.Stdout = stdout
	})

	Describe("Mov", func() {
		var mov main.Mov

		JustBeforeEach(func() {
			main.PC = 0x00000008
			main.IR.Data = uint32(0x336AC9D)
			mov.New()
			mov.Execute()
		})

		Context("#Execute", func() {
			It("should write value in RZ", func() {
				Expect(main.R[mov.Z()]).To(Equal(uint32(0x0016AC9D)))
			})
		})
		
		Context("#Print", func() {
			It("Should view return instruction", func() {
				mov.Print()
				w.Close()
				message, _ := ioutil.ReadAll(r)

				Expect(string(message)).To(Equal("0x00000008:\tmov r25, 1485981         \tR25=0x0016AC9D\n"))
			})
		})
	})

	Describe("Movs", func() {
		var mov main.Movs

		When("number is negative", func() {
			JustBeforeEach(func() {
				main.PC = 0x00000008
				main.IR.Data = uint32(0x336AC9D)
				mov.New()
				mov.Execute()
			})

			Context("#Execute", func() {
				It("should write value in RZ wtih signal", func() {
					Expect(main.R[mov.Z()]).To(Equal(uint32(0xFFF6AC9D)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction", func() {
					mov.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmovs r25, -611171        \tR25=0xFFF6AC9D\n"))
				})
			})
		})

		When("number is positive", func() {
			JustBeforeEach(func() {
				main.PC = 0x00000008
				main.IR.Data = uint32(0x326AC9D)
				mov.New()
				mov.Execute()
			})

			Context("#Execute", func() {
				It("should write value in RZ wtih signal", func() {
					Expect(main.R[mov.Z()]).To(Equal(uint32(0x0006AC9D)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction", func() {
					mov.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmovs r25, 437405         \tR25=0x0006AC9D\n"))
				})
			})
		})
	})

	Describe("Add", func() {
		var add main.Add

		JustBeforeEach(func() {
			main.PC = 0x00000008
			main.IR.Data = uint32(0x08611000)
			add.New()
			add.Execute()
		})

		AfterEach(func() {
			main.SR.Data = uint32(0)
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1] = 0
				main.R[2] = 0
			})
			Context("#Execute", func() {
				It("Should be equal zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000040)))
					Expect(main.R[add.Z()]).To(Equal(uint32(0x0)))
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
				main.R[1] = 0xFFFF0000
				main.R[2] = 0x00012345
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000001)))
					Expect(main.R[add.Z()]).To(Equal(uint32(0x00002345)))
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
		})

		When("Result has signal", func() {
			BeforeEach(func() {
				main.R[1] = 0x80000000
				main.R[2] = 0x00012345
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000010)))
					Expect(main.R[add.Z()]).To(Equal(uint32(0x80012345)))
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
		})

		When("Result has overflow with signal", func() {
			BeforeEach(func() {
				main.R[1] = 0x7FFFFFFF
				main.R[2] = 0x7FFFFFFF
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000018)))
					Expect(main.R[add.Z()]).To(Equal(uint32(0xFFFFFFFE)))
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
		})

		When("Result has overflow with carry and signal", func() {
			BeforeEach(func() {
				main.R[1] = 0xFFFFFFFF
				main.R[2] = 0xFFFFFFFF
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000019)))
					Expect(main.R[add.Z()]).To(Equal(uint32(0xFFFFFFFE)))
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
		})
	})

	Describe("Sub", func() {
		var sub main.Sub

		JustBeforeEach(func() {
			main.PC = 0x00000008
			main.IR.Data = uint32(0x08611000)
			sub.New()
			sub.Execute()
		})

		AfterEach(func() {
			main.SR.Data = uint32(0)
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1] = 0
				main.R[2] = 0
			})
			Context("#Execute", func() {
				It("Should be equal zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000040)))
					Expect(main.R[sub.Z()]).To(Equal(uint32(0x0)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)
					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1+R2=0x00000000,SR=0x00000040\n"))
				})
			})
		})

		When("Result has overflow", func() {
			BeforeEach(func() {
					main.R[1] = 0x80000000
					main.R[2] = 0x7FFFFFFF
			})

			Context("#Execute", func() {
				// 0x00012345 − 0xFFFF0000 = −FFFDDCBB
				// !(-FFFDDCBB) + 1 = 22345
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000008)))
					Expect(main.R[sub.Z()]).To(Equal(uint32(0x00000001)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1+R2=0x00000001,SR=0x00000008\n"))
				})
			})
		})

		When("Result has signal and overflow", func() {
			BeforeEach(func() {
				main.R[1] = 0xFFFFFFFE
				main.R[2] = 0x00000001
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000018)))
					Expect(main.R[sub.Z()]).To(Equal(uint32(0xFFFFFFFD)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with SN in SR toggle", func() {
					sub.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tsub r3,r1,r2             \tR3=R1+R2=0xFFFFFFFD,SR=0x00000018\n"))
				})
			})
		})
	})

	Describe("Mul", func() {
		var mul main.Mul

		JustBeforeEach(func() {
			main.PC = 0x00000008
			// OP = 000100
			// Z = 00111
			// I = 00011
			// X = 00001
			// X = 00010
			main.IR.Data = uint32(0x10E11003)
			mul.New()
			mul.Execute()
		})

		AfterEach(func() {
			main.SR.Data = uint32(0)
		})

		When("Result is zero", func() {
			BeforeEach(func() {
				main.R[1] = 0
				main.R[2] = 0x5
			})
			Context("#Execute", func() {
				It("Should be equal zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000040)))
					Expect(main.R[mul.Z()]).To(Equal(uint32(0x0)))
					Expect(main.R[mul.I5()]).To(Equal(uint32(0x0)))
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
					main.R[1] = 0x00000010
					main.R[2] = 0x00000010
			})

			Context("#Execute", func() {
				// 0x00012345 − 0xFFFF0000 = −FFFDDCBB
				// !(-FFFDDCBB) + 1 = 22345
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000000)))
					Expect(main.R[mul.Z()]).To(Equal(uint32(0x00000100)))
					Expect(main.R[mul.I5()]).To(Equal(uint32(0x00000000)))
				})
			})
			
			Context("#Print", func() {
				It("Should view return instruction with ZN in SR toggle", func() {
					mul.Print()
					w.Close()
					message, _ := ioutil.ReadAll(r)

					Expect(string(message)).To(Equal("0x00000008:\tmul r3,r7,r1,r2          \tR3:R7=R1*R2=0x0000000000000100,SR=0x00000000\n"))
				})
			})
		})

		When("Result has carry", func() {
			BeforeEach(func() {
				main.R[1] = 0xFFFFFFFE
				main.R[2] = 0x00000011
			})
			Context("#Execute", func() {
				It("Should be diferent zero", func() {
					Expect(main.SR.Data).To(Equal(uint32(0x00000001)))
					Expect(main.R[mul.Z()]).To(Equal(uint32(0xFFFFFFDE)))
					Expect(main.R[mul.I5()]).To(Equal(uint32(0x000000010)))
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
		})
	})
})
