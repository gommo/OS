;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This code was made for the tutorial:
;; "Making a Simple C kernel with Basic printf and clearscreen Functions"
;;
;; This code comes with absolutly
;; NO WARRANTY
;; you can not hold me(KJ), nor
;; anyone else responsible for what
;; this code does.
;;
;; This code is in the public domain,
;; you may use it however you want
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; this is for building a multiboot header
;; a multiboot header must exist for GRUB
;; to load the kernel

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_AOUT_KLUDGE	equ 1<<16
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)