/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������KV58F24���ܳ�VDĸ��
����    д���ܽŸ��ò�����CHIUSIR�޸�ȷ�ϣ�Դ������SDK��Ұ����մ��������ֲ
��E-mail  ��chiusir@163.com
�������汾��V1.0
�������¡�2017��12��15��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR7.80.4
��Target  ��MKV58F1M0VLQ24
��Crystal �� 50.000Mhz
��busclock��137.500MHz
��pllclock��275.000MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

//-------------------------------------------------------------------------*
//������: pit_init
//��  ��: ��ʼ��PIT
//��  ��: pitn:ģ����PIT0��PIT1��PIT2��PIT3
//        cnt �ж�ʱ�䣬��λ1ms
//��  ��: ��
//��  ��: pit_init(PIT0,1000); PIT0�жϣ�1000ms����1s����PIT0_interrupt()һ��
//-------------------------------------------------------------------------*
void PIT_Init(PITn pitn, u32 cnt)
{
    //PIT �õ��� Bus Clock ����Ƶ��

    /* ����ʱ��*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //ʹ��PITʱ��

    /* PITģ����� PIT Module Control Register (PIT_MCR) */
    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������

    /* ��ʱ������ֵ���� Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt*40*1000;                                   //��������ж�ʱ��

    //��ʱʱ�䵽�˺�TIF �� 1 ��д1��ʱ��ͻ���0
    PIT_Flag_Clear(pitn);                                             //���жϱ�־λ

    /* ��ʱ�����ƼĴ��� Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //ʹ�� PITn��ʱ��,����PITn�ж�

    NVIC_EnableIRQ((IRQn_Type)(pitn + 48));			      //���������ŵ�IRQ�ж�
}



//-------------------------------------------------------------------------*
//������: PIT0_interrupt
//��  ��: PIT�жϺ���
//��  ��: ��
//��  ��: ��
//��  ��: �ɳ�ʼ���������೤ʱ�����һ��
//-------------------------------------------------------------------------*
//short speed=0;
int speedLeftGet, speedRightGet;

void PIT0_IRQHandler(){
// �û������������
  PIT_Flag_Clear(PIT0);
  LED_Ctrl(LED0, RVS);        //�жϷ�����LED��˸
  speedLeftGet = FTM_AB_Get(FTM1);
  speedRightGet = -FTM_AB_Get(FTM2);
  Motor_Duty(MotL, PositionalPID(speedLeftGet,  &PIDMotorLeft));
  Motor_Duty(MotR, PositionalPID(speedRightGet, &PIDMotorRight));
//  speed=FTM_AB_Get(FTM2);     //������������󣬿��Ի�ȡ�ٶȣ�������ʾ����
}

void PIT1_IRQHandler(){
  PIT_Flag_Clear(PIT1);       //���жϱ�־λ
  LED_Ctrl(LED1, RVS);        //�жϷ�����LED��˸
  /*�û������������*/
  DataFusion();
  Servo_Duty(LimitingAmplitude(servo, 4700, 5800));
}

void PIT2_IRQHandler(){
  PIT_Flag_Clear(PIT2);       //���жϱ�־λ
  LED_Ctrl(LED3, RVS);
  GyroAngleProcessing();


  /*�û������������*/
}

void PIT3_IRQHandler()
{
  PIT_Flag_Clear(PIT3);       //���жϱ�־λ
  Menu();
  /*�û������������*/
}

// test function
void Test_PIT(void)
{
    //UART������ʾ
    UART_Init(UART_4,115200);
    UART_Put_Str(UART_4,"LongQiu UART4 ADC\n");
    PIT_Init(PIT0, 200);          //PIT��ʱ�жϳ�ʼ��

    while (1)
    {
      //LED��˸
      LED_Ctrl(LED1, RVS);
      UART_Put_Str(UART_4,"LongQiu PIT0 INTERRUPT!\n");
      //systick�ж���ʱ
      time_delay_ms(500);
    }
}
