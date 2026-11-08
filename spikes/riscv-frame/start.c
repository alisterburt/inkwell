static unsigned char frame[1872*1404/2];
extern unsigned inkwell_fill(unsigned char*, int, int);
static long sc(long n,long a,long b,long c){register long a7 __asm__("a7")=n,a0 __asm__("a0")=a,a1 __asm__("a1")=b,a2 __asm__("a2")=c;__asm__ volatile("ecall":"+r"(a0):"r"(a7),"r"(a1),"r"(a2));return a0;}
static void out(unsigned v){char s[16];int i=15;s[i]='\n';do{s[--i]='0'+v%10;v/=10;}while(v);sc(64,1,(long)(s+i),16-i);}
void _start(void){out(inkwell_fill(frame,1872,1404));out(frame[1]);out(frame[1314143]);sc(93,0,0,0);}
