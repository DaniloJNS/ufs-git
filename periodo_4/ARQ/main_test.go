package main_test

import (
	. "github.com/onsi/ginkgo/v2"
	. "github.com/onsi/gomega"

	"github.com/DaniloJNS/main"
)

var _ = Describe("Main", func() {

	Describe("Instructions", func() {
		Context("Store", func() {
			It("store 8 bits in memory", func() {
				main.Store8(0x00000001, 0x43)
				Expect(main.MEM[1]).To(Equal(uint8(0x43)))
			})

			It("store 16 bits in memory", func() {
				main.Store16(0x00000001, 0x43FC)

				Expect(main.MEM[2]).To(Equal(uint8(0x43)))
				Expect(main.MEM[3]).To(Equal(uint8(0xFC)))
			})

			It("store 32 bits in memory", func() {
				main.Store32(0x00000002, 0x10E343FC)

				Expect(main.MEM[8]).To(Equal(uint8(0x10)))
				Expect(main.MEM[9]).To(Equal(uint8(0xE3)))
				Expect(main.MEM[10]).To(Equal(uint8(0x43)))
				Expect(main.MEM[11]).To(Equal(uint8(0xFC)))
			})
		})

		Context("Load", func() {
			It("load 8 bits of memory", func() {
				address := uint32(0x00000001)
				data := uint8(0x43)
				main.Store8(uint32(address), data)

				Expect(main.Load8(address)).To(Equal(data))
			})

			It("load 16 bits of memory", func() {
				address := uint32(0x00000002)
				data := uint16(0xFC43)
				main.Store16(uint32(address), data)

				Expect(main.Load16(address)).To(Equal(data))
			})

			It("load 32 bits of memory", func() {
				address := uint32(0x00000002)
				data := uint32(0xAD36FC43)
				main.Store32(uint32(address), data)

				Expect(main.Load32(address)).To(Equal(data))
			})
		})
	})

	Context("Runtime", func() {
		Context("Methods", func() {
		})
	})

	Context("Strutcs", func() {
		Context("InstructionRegister", func() {
			Context("Methods", func() {
				var data uint32

				BeforeEach(func() {
					address := uint32(0x00000004)
					data = uint32(0xB536FC43)
					main.Store32(uint32(address), data)
					main.PC = address
					main.IR.Load()
				})

				It("load", func() {
					Expect(main.IR.Data).To(Equal(data))
				})

				It("opcode", func() {
					Expect(main.IR.Opcode()).To(Equal(uint8(0x2D)))
				})
			})
		
		})
		Context("Instruction", func() {
			Context("Methods", func() {
				var instruction *main.Instruction

				BeforeEach(func() {
					instruction = &main.Instruction{}
				})

				It("#Z", func() {
					main.IR.Data = uint32(0xF6ADA254)

					Expect(instruction.Z()).To(Equal(uint32(0x15)))
				})

				It("#X", func() {
					main.IR.Data = uint32(0xF635A254)

					Expect(instruction.X()).To(Equal(uint32(0x15)))
				})

				It("#Y", func() {
					main.IR.Data = uint32(0xF635AA54)

					Expect(instruction.Y()).To(Equal(uint32(0x15)))
				})

				It("#L", func() {
					main.IR.Data = uint32(0xFFFFF)

					Expect(instruction.L()).To(Equal(uint32(0x7FF)))
				})

				It("#I11", func() {
					main.IR.Data = uint32(0x2AFFFFF)

					Expect(instruction.I11()).To(Equal(uint32(0x7FF)))
				})

				It("#I16", func() {
					main.IR.Data = uint32(0xA7FFFFFF)

					Expect(instruction.I16()).To(Equal(uint32(0x07FFFFFF)))
				})
			})
		})

		Context("InstructionFormatU", func() {
			Context("Fields", func() {
				var instruction *main.InstructionFormatU

				BeforeEach(func() {
					instruction = &main.InstructionFormatU{}
					main.R[0x15] = 0xAD36FC43
				})

				It("#Z", func() {
					main.IR.Data = uint32(0xF6ADA254)
					instruction.New()

					Expect(*instruction.RZ).To(Equal(uint32(0xAD36FC43)))
				})

				It("#X", func() {
					main.IR.Data = uint32(0xF635A254)
					instruction.New()

					Expect(*instruction.RX).To(Equal(uint32(0xAD36FC43)))
				})

				It("#Y", func() {
					main.IR.Data = uint32(0xF635AA54)
					instruction.New()

					Expect(*instruction.RY).To(Equal(uint32(0xAD36FC43)))
				})
			})
		})

		Context("InstructionFormatF", func() {
			Context("Fields", func() {
				var instruction *main.InstructionFormatF

				BeforeEach(func() {
					instruction = &main.InstructionFormatF{}
					main.R[0x15] = 0xAD36FC43
				})

				It("#Z", func() {
					main.IR.Data = uint32(0xF6ADA254)
					instruction.New()

					Expect(*instruction.RZ).To(Equal(uint32(0xAD36FC43)))
				})

				It("#X", func() {
					main.IR.Data = uint32(0xF635A254)
					instruction.New()

					Expect(*instruction.RX).To(Equal(uint32(0xAD36FC43)))
				})
			})
		})
	})
})
