extern void con_init(void);
extern void con_write(const char * ptr);

const char * msg = "System loaded";

void main(void)
{
    con_init();
    con_write(msg);
}
