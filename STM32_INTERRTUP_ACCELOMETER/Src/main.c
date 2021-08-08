#include "main.h"
#include "sensor.h"   // kutuphaneyi kullanabilmemiz icin ekledik, kutuphane eklemeler hep boyle yapiliyot.
											// onislemci komutu imis bu - include -
											// #define   AHMETSOYLU		1  ==> artik ahmetsoylu gorulen yerlerde 1 olacak demek, bunu on islemci yapar
/*
	Ilgili kod D12'ye bagli LED'i 250ms araliklarla yakip s?nd?ren bir kod projesi.
	Ders boyuncaki t?m projelerde ve ?devlerde buradaki taslak kullanilacaktir.
	Bu proje kasnagindan haric herhangi farkli bir kod kasnagi olusturanlar bu durumun t?m ayrintilarini mutlaka anlatmaktan sorumludur.
*/

// int8_t demek = 8 bitlik degisken  ama hem negatif hem pozitif olabilir
// uint8_t demek = yine 8 bitlik ama sadece pozitif deger alir, u = unsigned demek yani pozitif

// x,y,z bizim eksen degiskenlerimiz 
int8_t		a,b,c;												// Bunlar kütüphaneyi kullanabilmemiz icin gerekli degiskenlermis, bunu kütüphaneyi aldigim kisi soyledi
uint8_t		durum,	durum1;							// bunlar da ayni sekilde									
SPI_HandleTypeDef hspi1;								// kütüphane için gerekliymis,	sizin GPIO tanimlarken kullanfigimiz yapi
																				// bu degiskene gore ayar yapiyoruz, yani onun baslangiz ayarlarini sagliyoruz, detayini bilmioyrum
																				// kutuphane icin kullanmamiz gereken bir tanimlama

static void LED_GPIO_Init(void);				// fonksiyon prototipleme, sizin fonksiyonuz.

//=========================================================================================

/* Bir bitlik veriyi saklamak icin ulasan diye degisken olusturdum*/
uint8_t ulasan;


UART_HandleTypeDef UART_InitStruct;
//degisken türü--- degisken adi

/* UART2 Interrupt Service Routine */
void USART2_IRQHandler(void)                      // Interrupt fonksiyonu
{
  HAL_UART_IRQHandler(&UART_InitStruct);  // asagidaki fonksiyonu cagirdi isi bitti geri geldi
	//sonraki satir geldik
	//goev yok
}

/* Buradaki callback belirtilen adette byte ulastiginda HAL_UART_IRQHandler tarafindan cagrilir! */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)    
{
	/*
		Buradaki alt kontrol USART2 icin sadece aktiftir ve
		Her girilen karakteri echo yaparak, transmit > receive
		Ayni zamanda buyuk A girildiginde ekrana ilave olarak BASARILIDIR yazar!
	*/
  if (huart->Instance == USART2)
  {
    /* 100ms timeout ile 1 byte yolla */
    HAL_UART_Transmit(huart, &ulasan, 1, 100);
		
    /* Kesme modunda ulasan 1 byte alma */ 
    HAL_UART_Receive_IT(huart, &ulasan, 1);
		
		if((ulasan == 'A')||(ulasan == 'a'))
		{
			if( a >= 0 )
			{
				uint8_t koordinat[] = "+X Yonundesin";
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);  // uart mesaj gonderme fonksiyonu
			}
			else  
			{
				uint8_t koordinat[] = "-X Yonundesin";
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);
			}
		}
						
    else if((ulasan == 'B')||(ulasan == 'b'))
		{
			 
			if( b >= 0 )  /// y pozitif deger mi, sensor kutuphanesi okuyor bunu
			{
				uint8_t koordinat[] = "+Y Yonundesin";    // neden degiskenlere ayni isim erdin nasil calisti?? cevap
																									// Degiskenler tanimlandigi bloga gore sinirlidir
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);
			}
	
				
			else
			{
				uint8_t koordinat[] = "-Y Yonundesin";
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);
			}
			
			
		}
	
    else if((ulasan == 'C')||(ulasan == 'c'))
		{
			if( c >= 0 )
			{
				uint8_t koordinat[] = "+Z Yonundesin";
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);
			}
			else
			{
				uint8_t koordinat[] = "-Z Yonundesin";
				HAL_UART_Transmit(huart, koordinat, sizeof(koordinat), HAL_MAX_DELAY);
			}
		}
			
		else if((ulasan == 'R')||(ulasan == 'r'))
		{
    if( a>= 5)
			{
				uint8_t renk[] = "KIRMIZI  YANIYOR ";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			}
			
			
			else	if(( a>= 5) && (b>=5))
			{
				uint8_t renk[] = "KIRMIZI VE TURUNCU YANIYOR ";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			}
		
			
	else	if( b >= 5 )
			{
				uint8_t renk[] = "TURUNCU";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			}
			else if((b >= 5 )&& (a<=5))
			{
				uint8_t renk[] = "TURUNCU YESIL YANIYOR";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			}
			 else if (a<=5)
			 {
			 	uint8_t renk[] = " YESIL YANIYOR";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			 
			 
			 }
		
		 else if ((a<=5) && (b<=5 ))
			 {
			 	uint8_t renk[] = " YESIL MAVI YANIYOR";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
				 
			 
			 }
		
	  else if ( b<=5 )
			 {
			 	uint8_t renk[] = "MAVI YANIYOR";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			 
			 }
		  
		else if (( a>= 5 )&& (b<=5 ))
			 {
			 	uint8_t renk[] = "MAVI KIRMIZI YANIYOR";
				HAL_UART_Transmit(huart, renk, sizeof(renk), HAL_MAX_DELAY);
			 
			 }
		
		
		}
		
		
		
	}
	
	// program sonu , çagirdigi yere donecek
}

//=========================================================================================


int main(void)
{
  HAL_Init();											// hoca kodu, baslangic ayarlari
  SystemClock_Config();						// hoca kodui sistem baslangic ayari yapiyor
  LED_GPIO_Init();								// hoca kodu, INIT: baslangic ayari,GPIO baslangic ayari yapiyor 
	
	spi1_init();										// sensor kutuphanesinde tanimlanmis spi baslangic ayarlarini yapar
	LIS3DSH_Init();									// sensor kutuphanesinde, 					
	
	///////////////
		/*
		PD5 > USART2_TX
		PD6 > USART2_RX
		Interrupt ile
	*/
	
	/* UART gerceklemesi icin init etmemiz (oncul tanimlamamiz) gereken birimler */
	GPIO_InitTypeDef GPIO_InitStruct;		// uart pinlerini tanimlamissiniz hocam
	
	/* Birimlerin CLK sinyallerini enable (aktif) etmemiz gerekiyor! */
	/* GPIO A portunun clock'unu aktiflestir */
	__HAL_RCC_USART2_CLK_ENABLE();    /// hocam usart 2 nin clock unu aktif ettiniz
	/* USART 2 clock'unu aktiflestir */
	__HAL_RCC_GPIOD_CLK_ENABLE();  //
 
  GPIO_InitStruct.Pin 			= GPIO_PIN_5 | GPIO_PIN_6;    // datasheet de 5 ve 6 nolu pimleri se?tim 
  GPIO_InitStruct.Mode 			= GPIO_MODE_AF_PP;  // ttl kullandigimiz i?i pp se?tik (tx ve rx i?in),
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  //  usart 2 de AF7 ayarli
  GPIO_InitStruct.Pull 			= GPIO_PULLUP;
  GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);   // pil ayarlarini yapipyot isliyor registerlara kaydediyor

	/*
    se?ilen       >> USART 2 				 
    iletisim hizi >> 115200  					
    veri uzunlugu >> 8 bit   
    dur bit sayisi>> 1 tane  
    eslik biti    >> x
    modu          >> Full Duplex 
    Handshaking   >> x
    OverSampling  >> 16
	*/
  UART_InitStruct.Instance 						= USART2;   //usart2 modu secildi
  UART_InitStruct.Init.BaudRate 			= 115200;     	// baud rate , letisim hizi
  UART_InitStruct.Init.WordLength 		= UART_WORDLENGTH_8B; // VERI UZUNLUGU
  UART_InitStruct.Init.StopBits 			= UART_STOPBITS_1;  // stop bit bir tane olacak
  UART_InitStruct.Init.Parity 				= UART_PARITY_NONE;   // parity biti yok
  UART_InitStruct.Init.Mode 					= UART_MODE_TX_RX; // AYNI ANDA ALIS VERIS modu
  UART_InitStruct.Init.HwFlowCtl 			= UART_HWCONTROL_NONE;
  UART_InitStruct.Init.OverSampling 	= UART_OVERSAMPLING_16;

	HAL_UART_Init(&UART_InitStruct);  // uartla ilgili degisikleri kaydediyoruz

  /* USART2 icin kesme biriminin oncul tanimlanmasi */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);     // set = ayarlamak priority = oncelik demek, interrupt onceligini tanimla diyor
	//HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 1); 
  HAL_NVIC_EnableIRQ(USART2_IRQn);    // interrup aktiflestirme,,  ISR = Interrupt Servive Rotuine
	HAL_UART_Receive_IT(&UART_InitStruct, &ulasan, 1);  // ayarlari kaydediyoruz
	////////////////////

	//#define yesil_led_on HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, SET)
	//#define yesil_led_of HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, RESET)
	
	//#define mavi_led_on HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, SET)
	//#define mavi_led_of HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, RESET)
	
	//#define kirmizi_led_on HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, SET)
	//#define kirmizi_led_of  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, RESET)
	
	//#define turuncu_led_on HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, SET)
	//#define turuncu_led_of HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, RESET)
	
	
  while (1) // while bir oldugu icin surekli donecek burada program, ama interrupt olursa  interrupt fonksiyona gidecek
  {
																	
	if ( a < -5) 	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);                                                                        //yesil_led_on;
	else		          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);                                                                                                                 // yesil_led_of;              		

	if ( a > 5) 	     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);	       // kirmizi_led_on;	
	else						   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);           //kirmizi_led_of;
	
	if ( b < -5)       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);		  // turuncu_led_on;
	else						   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);        //turuncu_led_of; 
	
	if ( b > 5) 		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);		 //mavi_led_on; 
	else							  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);    // mavi_led_of; 
		
  }
	
	// hocanin attigi githundaki oglan led yakma derecelerini 200 yapmisti ben 5 yaptim 
}  

static void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin 	= GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);					// ozellikleri artik isliyorikaydediyor
	
}









