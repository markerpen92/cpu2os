riscv64-unknown-elf-gcc -march=rv64g -mabi=lp64 -static -mcmodel=medany \
  -fvisibility=hidden -nostdlib -nostartfiles -Thello.ld hello.s -o hello.elf
file hello.elf
qemu-system-riscv64 -nographic -machine sifive_u -bios none -kernel hello.elf