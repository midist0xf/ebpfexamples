# ebpfexamples
This repository goal is to introduce eBPF basic usage in a easy way.

The following steps are tested on Ubuntu 19.04 (disco), 5.0.0-32-generic #34-Ubuntu SMP
with libbpf 0.0.5 and clang 8.0.0-3 and on Ubuntu 18.04 (bionic), 5.0.0-36-generic 39~18.04.1-Ubuntu SMP with clang 6.0.0-1 and libbpf 0.0.4.
## SETUP 
### Clone the repository
`git clone https://github.com/midist0xf/ebpfexamples.git`
### Install clang
`sudo apt install clang`
### Download linux source code
`sudo apt install linux-source`
### Copy bpf_helpers.h
```
cd ebpfexamples
cp /usr/src/llinux-source-5.0.0/tools/testing/selftests/bpf/bpf_helpers.h ./examples
```


### Install libelf-dev
`sudo apt install libelf-dev`
### Install libbpf 
```
git clone https://github.com/libbpf/libbpf.git
cd src
make
```
### Directory tree
```
ebpfexamples
├── examples
│   ├── bpf_helpers.h
│   ├── return_kern.c
│   └── ...
└── libbpf
```
  


## COMPILE AN EXAMPLE
Each example consists of two files `examplename_user.c` and `examplename_kern.c`. The `_kern.c` file contains the eBPF program. The `_user.c` file contains the userspace program which loads the eBPF program in to the kernel through the `libbpf` library. 
### 1 - Compile eBPF program
#### Define the alias
Use this alias to compile `_kern.c` files.
```
__kcc()
{  
     clang -S -I. -O2 -emit-llvm -c $1 -o - | llc -march=bpf -filetype=obj -o "$(basename  $1 .c).o";
 }
alias kcc=__kcc
```
#### example
`kcc examplename_kern.c`
### 2 - Compile userspace program
```
__ucc () 
{ 
    gcc -o "$(basename $1 _user.c)" $1 -I../libbpf/src/ ../libbpf/src/libbpf.a -lelf
}
alias ucc=__ucc
```
#### example
`ucc examplename_user.c`
### 3 - Execute the userspace program
`./examplename`

Programs which use a packet socket or tracing facilities require `sudo`.

## INSPECTION
### read elf sections
`llvm-readelf-8 -sections examplename_kern.o`
```
Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .strtab           STRTAB          0000000000000000 000248 00007b 00      0   0  1
  [ 2] .text             PROGBITS        0000000000000000 000040 000000 00  AX  0   0  4
  [ 3] socket            PROGBITS        0000000000000000 000040 000098 00  AX  0   0  8
  [ 4] .relsocket        REL             0000000000000000 000228 000010 10     11   3  8
  [ 5] maps              PROGBITS        0000000000000000 0000d8 00001c 00  WA  0   0  4
  [ 6] license           PROGBITS        0000000000000000 0000f4 000004 00  WA  0   0  1
  ...
```
### show the bytecode of the eBPF program 
`llvm-objdump -no-show-raw-insn -section=socket -S examplename_kern.o`
```
Disassembly of section socket:
0000000000000000 bpfrog1:
       0:	r6 = r1
       1:	r1 = 0
       2:	*(u32 *)(r10 - 4) = r1
       3:	r2 = r10
       4:	r2 += -4
       5:	r1 = 0 ll
       7:	call 1
       8:	if r0 == 0 goto +7 <LBB0_2>
       ...
 ```

## EXAMPLE EXPLANATION
### return 
Discard all packets received on the socket or accept all packets (if you change `return 0` to `return -1`).

### icmpreply
Accept only icmp echo reply packets. Execute `sudo ./icmpreply` and ping your machine from a second one while monitoring the traffic. 

### udpport
Accept only udp packets from a fixed port (1030). 
On a first terminal execute `./udpport`. On a second terminal execute `nc -u -p 1031 localhost 55555` and then `nc -u -p 1031 localhost 55555` to see the difference.

### udportmap
The eBPF program dynamically modifies the udp src allowed port accordingly to what the userspace program writes on a shared hash map. 

On a first terminal execute `./udportmap`
```
./udportmap
port=1031 allowed
size of the message=2
port=1032 allowed
size of the message=3
port=1033 allowed
size of the message=4
port=1034 allowed
...
```
On a second terminal use netcat to send udp messages to the local interface modifying the port everytime.
```
$ echo "a" | nc -u -p 1031 localhost 55555
^C
$ echo "bc" | nc -u -p 1032 localhost 55555
^C
$ echo "def" | nc -u -p 1033 localhost 55555
```
### traceopenat
Writes a message in `/sys/kernel/debug/tracing/trace` every time `openat` is called with `O_RDONLY` parameter. First execute `sudo ./traceopneat` and then `sudo cat /sys/kernel/debug/tracing/trace` to see the message.

```
# tracer: nop
#
# entries-in-buffer/entries-written: 8/8   #P:4
#
#                              _-----=> irqs-off
#                             / _----=> need-resched
#                            | / _---=> hardirq/softirq
#                            || / _--=> preempt-depth
#                            ||| /     delay
#           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
#              | |       |   ||||       |         |
     traceopenat-6736  [003] ....  4387.099495: 0: openat read only
```

