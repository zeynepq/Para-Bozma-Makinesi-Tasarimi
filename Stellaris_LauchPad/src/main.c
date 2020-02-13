
#include <stdlib.h>

#include "inc/lm4f120h5qr.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"


volatile unsigned long ulLoop;


void portB_aktif()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // Port B’nin saati aktifleştir
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7);
	GPIO_PORTB_AFSEL_R &= ~0xFF; // Alternatif fonksiyonlar kapat
	GPIO_PORTB_DEN_R |= 0xFF;
}

void setup()
{
//clock setup
	SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
//lcd connections init
	ulLoop = SYSCTL_RCGC2_R;
	GPIO_PORTD_DIR_R = 0xff;
	GPIO_PORTD_DEN_R = 0xff;
	GPIO_PORTF_DIR_R = 0xff;
	GPIO_PORTF_DEN_R = 0xff;
}


#define RS(X)     GPIO_PORTF_DATA_R = ((GPIO_PORTF_DATA_R & ~(1<<1)) | (X<<1))
#define EN(X)     GPIO_PORTF_DATA_R = ((GPIO_PORTF_DATA_R & ~(1<<2)) | (X<<2))
#define LCD_STROBE do{EN(1);EN(0);}while(0)
#define databits GPIO_PORTD_DATA_R  // P1.7 - D7, ....., P1.4 - D4
#define LINE1 cmd(0x80)
#define LINE2 cmd(0xc0)


void data(unsigned char c)
{

    RS(1); //Rs(1) -> veri saklayacisi
    SysCtlDelay(400); //bekleme (geçme) işlemi
    databits = c >>4;
    LCD_STROBE;
    databits = c;
    LCD_STROBE;
}

void cmd(unsigned char c)
{

    RS(0); //Rs(0) -> data(komut) saklayicisi
    SysCtlDelay(400);
    databits = c >>4; //4 bit saga kaydır.
    LCD_STROBE;
    databits = c;
    LCD_STROBE;
}

void pseudo_8bit_cmd(unsigned char c)
{
    RS(0);
    SysCtlDelay(15000);
    databits = (c);
    LCD_STROBE;
}
void clear(void)
{
    cmd(0x01);
    SysCtlDelay(30000);
}

void lcd_init()
{
    pseudo_8bit_cmd(0x30); //this command is like 8 bit mode command
    pseudo_8bit_cmd(0x30); //lcd expect 8bit mode commands at first
    pseudo_8bit_cmd(0x30); //for more details, check any 16x2 lcd spec
    pseudo_8bit_cmd(0x20);
    cmd(0x28);             //4 bit mode command started, set two line
    cmd(0x0c);             // Make cursorinvisible
    clear();               // Clear screen
    cmd(0x6);              // Set entry Mode(auto increment of cursor)
}

void lcd_yazdir(char *p)
{
    while(*p) data(*p++);
}

float yirmilik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / 20;
	kalanpara = para -(adet * 20);

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
    itoa(adet, adeti, 10);

    if(adet == 0 )
    	return kalanpara;
    else{
    	lcd_init();
    	LINE1;
    	lcd_yazdir("           ");
    	lcd_yazdir(tam);
    	lcd_yazdir(".");
    	lcd_yazdir(yarim);
    	LINE2;
    	lcd_yazdir(adeti);
    	lcd_yazdir("yirmilik");
    	SysCtlDelay(16000000);
    	RS(0);

    	return kalanpara;
    }
}

float onluk(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / 10;
	kalanpara = para -(adet * 10);

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	   	return kalanpara;
	else{
	   	lcd_init();
	   	LINE1;
	   	lcd_yazdir("           ");
	   	lcd_yazdir(tam);
	   	lcd_yazdir(".");
	   	lcd_yazdir(yarim);
	   	LINE2;
	   	lcd_yazdir(adeti);
	   	lcd_yazdir("onluk");
	   	SysCtlDelay(16000000);
	   	RS(0);
	   	return kalanpara;
   }
}

float beslik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / 5;
	kalanpara = para -(adet * 5);

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	   	return kalanpara;
	else{
	   	lcd_init();
	   	LINE1;
	   	lcd_yazdir("           ");
	   	lcd_yazdir(tam);
	   	lcd_yazdir(".");
	   	lcd_yazdir(yarim);
	  	LINE2;
	  	lcd_yazdir(adeti);
	  	lcd_yazdir("beslik");
	   	SysCtlDelay(16000000);
	   	RS(0);
	   	return kalanpara;
	  }
}

float birlik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / 1;
	kalanpara = para -(adet * 1);

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	    	return kalanpara;
	    else{
	    	lcd_init();
	    	LINE1;
	    	lcd_yazdir("           ");
	    	lcd_yazdir(tam);
	    	lcd_yazdir(".");
	    	lcd_yazdir(yarim);
	   	   	LINE2;
	   	   	lcd_yazdir(adeti);
	   	   	lcd_yazdir("birlik");
	    	SysCtlDelay(16000000);
	    	RS(0);

	    	return kalanpara;
	    }
}

float ellikurusluk(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / (0.5); //0.5 =1/2
	kalanpara = para -(adet * (0.5));

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	    	return kalanpara;
	    else{
	    	lcd_init();
	    	LINE1;
	    	lcd_yazdir("           ");
	    	lcd_yazdir(tam);
	    	lcd_yazdir(".");
	    	lcd_yazdir(yarim);
	   	   	LINE2;
	   	   	lcd_yazdir(adeti);
	   	   	lcd_yazdir("yarimlik");
	    	SysCtlDelay(16000000);
	    	RS(0);

	    	return kalanpara;
	    }
}

float ceyreklik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / (0.25);
	kalanpara = para -(adet * (0.25));

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	    	return kalanpara;
	    else{
	    	lcd_init();
	    	LINE1;
	    	lcd_yazdir("           ");
	    	lcd_yazdir(tam);
	    	lcd_yazdir(".");
	    	lcd_yazdir(yarim);
	    	LINE2;
	    	lcd_yazdir(adeti);
	    	lcd_yazdir("ceyreklik");
	    	SysCtlDelay(16000000);
	    	RS(0);

	    	return kalanpara;
	    }
}

float metelik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / (0.1);
	kalanpara = para -(adet * (0.1));

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);
	if(adet == 0 )
	    	return kalanpara;
	    else{
	    	lcd_init();
	    	LINE1;
	    	lcd_yazdir("           ");
	    	lcd_yazdir(tam);
	    	lcd_yazdir(".");
	    	lcd_yazdir(yarim);
	    	LINE2;
	    	lcd_yazdir(adeti);
	    	lcd_yazdir("metelik");
	    	SysCtlDelay(16000000);
	    	RS(0);

	    	return kalanpara;
	    }
}

float delik(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / (0.05);
	kalanpara = para -(adet * (0.05));

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10);
	itoa(adet, adeti, 10);

	if(adet == 0 )
	    	return kalanpara;
	    else{
	    	lcd_init();
	    	LINE1;
	    	lcd_yazdir("           ");
	    	lcd_yazdir(tam);
	    	lcd_yazdir(".");
	    	lcd_yazdir(yarim);
	    	LINE2;
	    	lcd_yazdir(adeti);
	    	lcd_yazdir("delik");
	    	SysCtlDelay(16000000);
	    	RS(0);

	    	return kalanpara;
	    }
}

void kurusluk(float para)
{
	int adet ;
	float kalanpara;
	char tam[4],yarim[4], adeti[4];
	adet = para / (0.01);
	kalanpara = para -(adet * (0.01));

	int tamkisim = kalanpara;
	int yarimkisim = (kalanpara - tamkisim) * 100 ;

	itoa(tamkisim, tam, 10);
	itoa(yarimkisim, yarim, 10); //onluk tabanda olduğunu ifade eder
	itoa(adet, adeti, 10);

	if(adet != 0)
	{
		lcd_init();
		LINE1;
		lcd_yazdir("          ");
		lcd_yazdir(tam);
		lcd_yazdir(".");
		lcd_yazdir(yarim);
		LINE2;
		lcd_yazdir(adeti);
		lcd_yazdir("kurusluk");
		SysCtlDelay(16000000);
		RS(0);
	}

}

int main()
{
	setup();
    clear();
	portB_aktif();
	double para = 0.0;
	int onlarButonu,birlerButonu,noktaButonu,ondabirButonu,yuzdebirButonu,hesaplaButonu,resetleButonu;
	int onlarBasamagiSayac = 0 ,birlerBasamagiSayac= 0,ondabirlerBasamagiSayac=0,yuzdebirlerBasamagiSayac=0;
	while(1)
	{

		int tamkisim,yarimkisim;
		char paraTam[2],paraYarim[2];

		onlarButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0);
		birlerButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1); //B1 de bağlı butonun degeri
		noktaButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2); //B2	de bağlı olan buton degeri
		ondabirButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3);   //B3 e bağlı olan buton degeri
		yuzdebirButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4); //B4 e bağlı olan buton degeri
		hesaplaButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5); //B5 e baglı olan buton degeri
		resetleButonu = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_7); //B6 e baglı olan buton degeri

		if(onlarButonu == 1)
		{
			SysCtlDelay(16000000/3);
			onlarBasamagiSayac++;
			if(onlarBasamagiSayac>9)
				onlarBasamagiSayac=0;
			para =onlarBasamagiSayac * 10 + birlerBasamagiSayac * 1 + ondabirlerBasamagiSayac * 0.1 + yuzdebirlerBasamagiSayac * 0.01;


		}

		else if(birlerButonu == 2)
		{
			SysCtlDelay(16000000/3);
			birlerBasamagiSayac++;
			if(birlerBasamagiSayac>9)
				birlerBasamagiSayac=0;
			para =onlarBasamagiSayac * 10 + birlerBasamagiSayac * 1 + ondabirlerBasamagiSayac * 0.1 + yuzdebirlerBasamagiSayac * 0.01;


		}
		else if(noktaButonu == 4)
		{
			char nokta;
			nokta = '.';
			SysCtlDelay(16000000/3);
		}
		else if(ondabirButonu == 8)
		{
			SysCtlDelay(16000000 /3);
			ondabirlerBasamagiSayac++;
			if(ondabirlerBasamagiSayac>9)
				ondabirlerBasamagiSayac=0;
			para =onlarBasamagiSayac * 10 + birlerBasamagiSayac * 1 + ondabirlerBasamagiSayac * 0.1 + yuzdebirlerBasamagiSayac * 0.01;


		}
		else if(yuzdebirButonu == 16)
		{
			SysCtlDelay(16000000 /3);
			yuzdebirlerBasamagiSayac++;
			if(yuzdebirlerBasamagiSayac>9)
				yuzdebirlerBasamagiSayac=0;
			para =onlarBasamagiSayac * 10 + birlerBasamagiSayac * 1 + ondabirlerBasamagiSayac * 0.1 + yuzdebirlerBasamagiSayac * 0.01;

		}
		else if(hesaplaButonu == 32)
		{
			SysCtlDelay(16000000 /3);
			para = (onlarBasamagiSayac * 10) + (birlerBasamagiSayac * 1) + (ondabirlerBasamagiSayac * 0.1) + (yuzdebirlerBasamagiSayac * 0.01);
			float yirmilikkalan = yirmilik(para);
			float onlukkalan = onluk(yirmilikkalan);
			float beslikkalan = beslik(onlukkalan);
			float birlikkalan = birlik(beslikkalan);
			float yarimlikkalan = ellikurusluk(birlikkalan);
		    float ceyreklikkalan = ceyreklik(yarimlikkalan);
			float metelikkalan = metelik(ceyreklikkalan);
			float delikkalan = delik(metelikkalan);
			kurusluk(delikkalan);
			clear();
		}
		else if(resetleButonu == 128)
		{
			SysCtlDelay(16000000 /3);
			onlarBasamagiSayac=0;
			birlerBasamagiSayac=0;
			ondabirlerBasamagiSayac=0;
			yuzdebirlerBasamagiSayac =0;
			para = (onlarBasamagiSayac * 10) + (birlerBasamagiSayac * 1) + (ondabirlerBasamagiSayac * 0.1) + (yuzdebirlerBasamagiSayac * 0.01);

		}

		tamkisim = para;
		yarimkisim = (para - tamkisim) * 100 ;

		itoa(tamkisim, paraTam, 10);
		itoa(yarimkisim,paraYarim,10);

		lcd_init();
		LINE1;
		lcd_yazdir("          ");
		lcd_yazdir(paraTam);
		lcd_yazdir(".");
		lcd_yazdir(paraYarim);
	}
}
