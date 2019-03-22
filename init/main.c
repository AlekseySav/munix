const char * msg = "Munix loaded";

#define WIDTH 80

unsigned pos;
unsigned x, y;

void main(void)
{
    char * c = (char *)0xb8000;

    pos = *(unsigned *)0x901fe;             // loaded cursor pos from saved place...
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
