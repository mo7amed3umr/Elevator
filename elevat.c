#include <C8051F020.h>
#include <compiler_defs.h>
#include <intrins.h>
#define stepper P1


void go_up(unsigned int floor);
void go_down(unsigned int floor);
void msdelay(unsigned int time);
void Delay_servo(unsigned int us);
void open_close_door();
void interrupts_Init();


int i =0;
int current=0;
char floor[5]= {0,0,0,0,0};
char up_flags[] = {0,0,0,0,0};
char down_flags[] = {0,0,0,0,0};
unsigned int ch[]={0x00,0x01,0x02,0x03,0x04};
int door = 0;
// keypad
sbit row1=P2^0;
sbit row2=P2^1;
sbit row3=P2^2;
sbit row4=P2^3;
sbit col1=P2^4;
sbit col2=P2^5;
sbit col3=P2^6;
// motorPin
sbit door_motor= P1^6;
// door interrupt pin
sbit open_door= P3^2;
// call buttons
sbit UP_Btn_0 = P2^7;
sbit UP_Btn_1 = P3^0;
sbit DOWN_Btn_1 = P3^1;
sbit UP_Btn_2 = P3^3;
sbit DOWN_Btn_2 = P3^4;
sbit UP_Btn_3 = P3^5;
sbit DOWN_Btn_3 = P3^6;
sbit DOWN_Btn_4 = P3^7;
sbit over_load = P1^7;


void timer0_isr() interrupt 1
{
    TH0  = 0XFE;        
    TL0  = 0X0B;
	col1=1;
	col2=1;
	col3=1;
	row1=row2=row3=row4=0;
	// check on column 1
	if(col1==0)
	{
	row1=row2=row3=row4=1;

	// check on row 1
	row1=0;
	if(col1==0)
	{
	floor[1] =1;
	}
	// check on row 2
	row1=1;
	row2=0;
	if(col1==0)
	{
	floor[4] = 1;
	}
	}

	// check on column 2
	else if(col2==0)
	{
	row1=row2=row3=row4=1;
	row1=0;
	if(col2==0)
	{
	floor[2] = 1;
	}

	row1=1;
	row4 = 0;
	if(col2==0)
	{
	floor[0] = 1;
	}
	}
	// check on column 3
	else if(col3==0) 
	{
	row1=row2=row3=row4=1;
	row1=0;
	if(col3==0)	
	{
	floor[3] = 1;
	}
	}
    if (!UP_Btn_0 ){
        up_flags[0] = 1;   
    }
    else if (!UP_Btn_1 ){
        up_flags[1] = 1;
    }
    else if (!UP_Btn_2){
        up_flags[2] = 1;
    }
    else if (!UP_Btn_3){
        up_flags[3] = 1;
    }
    else if (!DOWN_Btn_1 ){
        down_flags[1] = 1;   
    }
    else if (!DOWN_Btn_2 ){
        down_flags[2] = 1;
    }
    else if (!DOWN_Btn_3){
        down_flags[3] = 1;
    }
    else if (!DOWN_Btn_4){
        down_flags[4] = 1;
    }	      
}


void ex0_isr (void) interrupt 0
{
door= 180; 

}



void main ()
{
P1 = 0x00; // initially all buttons are high 
P2 = 0xf0; //lower bits as outputs, upper bits as inputs
P0 = 0x00; // initialize the 7-segment to be in the ground floor
door_motor = 1;
over_load = 0;


open_door = 1;

interrupts_Init();

while(1)
{


if (floor[0] ==  1)
{
	if(0 < current)
	{
	go_down(0);
	open_close_door();
	}
	floor[0] = 0;

}

if (floor[1] ==  1)
{

	if (1 > current)
	{
	go_up(1 );
	open_close_door();

	}
	else if(1 < current)
	{
	go_down(1);
	open_close_door();

	}

	floor[1] = 0;

}


if (floor[2] ==  1)
{

	if (2 > current)
	{
	go_up(2 );
	open_close_door();

	}
	else if(2 < current)
	{
	go_down( 2);
	open_close_door();

	}

	floor[2] = 0;

}


if (floor[3] ==  1)
{

	if (3 > current)
	{
	go_up(3 );
	open_close_door();

	}
	else if(3 < current)
	{
	go_down(3);
	open_close_door();

	}

	floor[3] = 0;

}


if (floor[4] ==  1)
{

	if (4 > current)
	{
	go_up(4 );
	open_close_door();

	}
	floor[4]= 0;
}

if (up_flags[0])
{
	if(0 < current)
	{
	go_down(0);
	open_close_door();
	}
	up_flags[0] = 0;	
}

if (up_flags[1])
{
	if (1 > current)
	{
	go_up(1 );
	open_close_door();
	}
	else if(1 < current)
	{
	go_down(1);
	}
	up_flags[1] = 0;	
}

if (up_flags[2])
{
	if (2 > current)
	{
	go_up(2 );
	open_close_door();

	}
	else if(2 < current)
	{
	go_down( 2);
	open_close_door();
	}
	up_flags[2] = 0;
}

if (up_flags[3])
{
	if (3 > current)
	{
	go_up(3 );
	open_close_door();

	}
	else if(3 < current)
	{
	go_down( 3);
	open_close_door();
	}
	up_flags[3] = 0;
}


if (down_flags[1])
{
	if (1 > current)
	{
	go_up(1);
	open_close_door();

	}
	else if(1 < current)
	{
	go_down( 1);
	open_close_door();
	}
	down_flags[1] = 0;
}

if (down_flags[2])
{
	if (2 > current)
	{
	go_up(2);
	open_close_door();

	}
	else if(2 < current)
	{
	go_down( 2);
	open_close_door();
	}
	down_flags[2] = 0;
}

if (down_flags[3])
{
	if (3 > current)
	{
	go_up(3);
	open_close_door();

	}
	else if(3 < current)
	{
	go_down( 3);
	open_close_door();
	}
	down_flags[3] = 0;
}


if (down_flags[4])
{
	if (4 > current)
	{
	go_up(4);
	open_close_door();

	}
	else if(4 < current)
	{
	go_down( 4);
	open_close_door();
	}
	down_flags[4] = 0;
}
}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void go_up(unsigned int floor_no)
{
   for(current;current<=(floor_no-1);current++) {
	if (floor[current] == 1 || up_flags[current] == 1)
		{
		msdelay(400);
		floor[current]=0;
		up_flags[current]=0;
		open_close_door();
		}
		stepper =(stepper&0xf0)|0x09;
		msdelay(100);
		stepper =(stepper&0xf0)|0x01;
		msdelay(100);
		stepper =(stepper&0xf0)|0x03;
		msdelay(100);
		stepper =(stepper&0xf0)|0x02;
		msdelay(100);
		stepper =(stepper&0xf0)|0x06;
		msdelay(100);
		stepper =(stepper&0xf0)|0x04;
		msdelay(100);
		stepper =(stepper&0xf0)|0x0C;
		msdelay(100);
		stepper =(stepper&0xf0)|0x08;
		msdelay(100);
		stepper =(stepper&0xf0)|0x09;
		msdelay(100);
		P0 = ch[current+1];
		
		}
	
		
		
		 
}


void go_down(unsigned int floor_no)
{
   for(current; current>=(floor_no+1); current--){
	if (floor[current] == 1 || down_flags[current] == 1)
		{
		msdelay(400);
		floor[current]=0;
		down_flags[current]=0;
		open_close_door();
		}
 	 stepper =(stepper&0xf0)|0x09;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x08;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x0C;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x04;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x06;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x02;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x03;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x01;
	 msdelay(100);
	 stepper =(stepper&0xf0)|0x09;
	 msdelay(100); 
	 P0 = ch[current-1];
	 }
	 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void open_close_door()
{
    for(door = 50;door< 180;door+=10)
		{
		door_motor=1;
	   Delay_servo(door);
	   door_motor = 0;
	   msdelay(50);
	   }
	msdelay(300);
	while(over_load == 1);
	for(door = 180;door> 50;door-=10)
		{
		if(door == 180)
		{
		msdelay(300);
		}

		door_motor=1;
	   Delay_servo(door);
	   door_motor = 0;
	      msdelay(50);
	   
	   }
}

///////////////////////////////////////////////////////////DELAYS FUNCTIONS///////////////////////////////////////////////////////////////////////
void msdelay(unsigned int time)
    {
        unsigned i,j ;
        for(i=0;i<time;i++)    
        for(j=0;j<1275;j++);
    }

void Delay_servo(unsigned int us)
{
	while(us--)
	{
    _nop_();
    }		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void interrupts_Init()				
{
	EA  = 1;	/* Enable global interrupt */
	EX0 = 1;    /* Enable Ext. interrupt0 */
	IT0 = 1;    /* Select Ext. interrupt0 on falling edge */
	TMOD = 0x01;       //Timer0 mode 1 
    TH0 = 0XFE;        //Load the timer value
    TL0 = 0X0B;
    TR0 = 1;           //turn ON Timer zero
    ET0 = 1;           //Enable TImer0 Interrupt
    EA = 1;            //Enable Global Interrupt bit
}

