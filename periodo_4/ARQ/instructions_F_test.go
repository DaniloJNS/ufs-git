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
})
