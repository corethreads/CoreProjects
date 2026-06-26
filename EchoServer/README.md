USER SPACE (Your Echo Server Engine)
 ┌────────────────────────────────────────────────────────┐
 │  [Passive Socket fd] ──> [accept()] blocks              │
 │                               │                        │
 │                               ▼ (returns Client fd)    │
 │       ┌───────────────────────┴────────────────┐       │
 │       │  Loop: read(fd, buf)  ──> write(fd, buf)│       │
 └───────┼──────────────────────────▲─────────────┼───────┘
         │ Syscall Boundary         │             │ Syscall Boundary
 ────────┼──────────────────────────┼─────────────┼─────────────────
 KERNEL SPACE (Linux OS)            │             │
 ┌───────▼──────────────────────────┴─┐           │
 │  TCP ESTABLISHED ACCEPT QUEUE      │           │
 │  [Client Conn 1][Client Conn 2]    │           │
 └───────────────▲────────────────────┘           ▼
                 │                         ┌──────────────┐
                 │ (Handshake Complete)    │ Socket Ring  │
         ┌───────┴──────┐                  │ TX/RX Buffers│
         │  SYN QUEUE   │                  └──────┬───────┘
         └───────▲──────┘                         │
                 │                                ▼
 ────────────────┼────────────────────────────────┼─────────────────
 HARDWARE        │                                │
         [Incoming TCP SYN Packet]        [Raw Byte Streams]
                 │                                ▲
         ┌───────┴────────────────────────────────┴───────┐
         │             Network Interface Card (NIC)       │
         └────────────────────────────────────────────────┘
