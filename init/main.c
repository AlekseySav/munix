const char * msg = "Munix loaded";

#define WIDTH 80

unsigned x, y;
unsigned pos;
char z = 'z';

#define CS 0x8

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct idt_entry {
    uint16_t low_offset;
    uint16_t segment;
    uint8_t reserved;
    uint8_t flags;
    uint16_t high_offset;
} idt_entry;

#define low16(x) (uint16_t)(x & 0xffff)
#define high16(x) (uint16_t)((x >> 16) & 0xffff)

typedef struct {
    uint16_t size;
    uint32_t ptr;
} idt_descriptor;

#define IDT_SIZE 256

idt_entry idt[IDT_SIZE];
idt_descriptor idt_d;

extern void ignore_int(void);

void print(void)
{
    char * c = (char *)0xb8000;

    pos = *(unsigned *)0x901fe;             // loading cursor pos from saved place...
    x = pos & 0x00ff;                       // lower byte is x pos
    y = (pos & 0xff00) >> 8;                // higher byte is y pos
    pos = (x + y * WIDTH) << 1;

    c += pos;

    while(*msg != 0) {                      // write msg
        *c = *msg;                          // write letter
        *(c + 1) = 2;                       // set green color
        c += 2;
        msg++;
    }
}

void main(void)
{
    uint32_t ignore = (uint32_t)ignore_int;
    
    for(int i = 0; i < IDT_SIZE; i++) {
        idt[i].low_offset = low16(ignore);
        idt[i].segment = CS;
        idt[i].reserved = 0;
        idt[i].flags = 0x8e;
        idt[i].high_offset = high16(ignore);
    }

    uint32_t prt = (uint32_t)print;
    idt[1].low_offset = low16(prt);
    idt[1].high_offset = high16(prt);

    idt_d.size = sizeof(idt_entry) * IDT_SIZE - 1;
    idt_d.ptr = (uint32_t)&idt[0];

    asm volatile("lidt (%0)":: "r" (&idt_d));
    asm("sti");

    asm volatile("int ");
}
