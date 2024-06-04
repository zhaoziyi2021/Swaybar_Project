//****************************************************************************
// @Project Includes
//****************************************************************************
#include "MAIN.h"

can_mailbox_descriptor_struct g_transmit_message;
can_mailbox_descriptor_struct g_receive_message;
uint8_t rx_data[8];

void CAN_vInit(void);
void CAN_CAN0Init(void);
void CanDebugSend(ulong ulID,ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7);

//=================================================================================================
// [Function Name]void CAN_CAN0Init(void)
// [Parameters] None
// [Introduction]
// [Return value] None
// [Creator] 20240415 zzy
//=================================================================================================
void CAN_CAN0Init(void)
{
    can_parameter_struct can_parameter;

    /* enable CAN clock */
    rcu_can_clock_config(CAN0, RCU_CANSRC_PCLK2_DIV_2);
    rcu_periph_clock_enable(RCU_CAN0);

    /* enable CAN port clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure CAN0 GPIO */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_13);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_14);

    /* initialize CAN register */
    can_deinit(CAN0);

    /* initialize CAN */
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    /* initialize CAN parameters */
    can_parameter.internal_counter_source = CAN_TIMER_SOURCE_BIT_CLOCK;
    can_parameter.self_reception = DISABLE;
    can_parameter.mb_tx_order = CAN_TX_HIGH_PRIORITY_MB_FIRST;
    can_parameter.mb_tx_abort_enable = ENABLE;
    can_parameter.local_priority_enable = DISABLE;
    can_parameter.mb_rx_ide_rtr_type = CAN_IDE_RTR_FILTERED;
    can_parameter.mb_remote_frame = CAN_STORE_REMOTE_REQUEST_FRAME;
    can_parameter.rx_private_filter_queue_enable = ENABLE;
    can_parameter.rx_filter_order = CAN_RX_FILTER_ORDER_FIFO_FIRST;
    can_parameter.memory_size = CAN_MEMSIZE_32_UNIT;
    /* filter configuration */
    can_parameter.mb_public_filter = 0xFFFFFFFF;
    //SJW,Resynchronization jump width
    can_parameter.resync_jump_width = 1;
    //SS,Synchronization Segment,SYNC_SEG,1Tq
    //PTS,Propagation Time Segment,PROP_SEG
    can_parameter.prop_time_segment = 2;
    //PBS1,Phase Buffer Segment 1,PHASE_SEG1 10
    can_parameter.time_segment_1 = 10;
    //PBS2,Phase Buffer Segment 2,PHASE_SEG2 3
    can_parameter.time_segment_2 = 3;
    can_parameter.prescaler = 5;
    /*
    tsp = (SYNC_SEG + PROP_SEG + PHASE_SEG1) / (SYNC_SEG + PROP_SEG + PHASE_SEG1 + PHASE_SEG2)
       = (1 + 2 + 10) / (1 + 2 + 10 + 3)
       = 81.25%
    */
    /*
    BitRate = ClockFrequency / (1 + PHASE_SEG1 + PHASE_SEG2) / prescaler
            = 40MHz / 16 / 5
            = 500Kbps
    */
    can_auto_busoff_recovery_enable(CAN0);
    /* initialize CAN */
    can_init(CAN0, &can_parameter);
    
    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_Message_IRQn, 1, 0);

    /* enable CAN MB0 interrupt */
    can_interrupt_enable(CAN0, CAN_INT_MB0);
    can_interrupt_enable(CAN0, CAN_INT_MB1);
    can_interrupt_enable(CAN0, CAN_INT_MB2);
    can_interrupt_enable(CAN0, CAN_INT_MB3);
    can_interrupt_enable(CAN0, CAN_INT_MB6);
    can_interrupt_enable(CAN0, CAN_INT_MB7);
    can_interrupt_enable(CAN0, CAN_INT_MB8);
    can_interrupt_enable(CAN0, CAN_INT_MB9);
    can_interrupt_enable(CAN0, CAN_INT_MB31);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x770;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 0, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 0, 0x1ffc0000);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x7DF;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 1, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 1, 0x1ffc0000);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x112;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 3, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 3, 0x1ffc0000);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x202;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 4, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 4, 0x1ffc0000);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x240;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 5, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 5, 0x1ffc0000);

    /* initialize receive message */
    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0xD0;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 31, &g_receive_message);//g_receive_message
    can_private_filter_config(CAN0, 31, 0x1ffc0000);

    can_operation_mode_enter(CAN0, CAN_NORMAL_MODE);
}


//=================================================================================================
// [Function Name]void CAN0_Message_IRQHandler(void)
// [Parameters] None
// [Introduction] CAN0 receive interrupt function
// [Return value] None
// [Creator] 20240320 zzy
//=================================================================================================
void CAN0_Message_IRQHandler(void)
{
    // 0x770 Swaybar_Physical_Diagnostic_Rx
    if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB0))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB0);
        CAN_RxFrame(0);
        /*add by BWF for when receive consecutive request frame only response the first frame*/
        EB_TpReqManage();
        SoftTimerStart(Tester_Absent_Timer, TESTER_ABSENT_TIMEROUT);
        Tester_Absent_Flg = FALSE;

    }
    // 0x7DF Tester_FunctionAddressingReq
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB1)) 
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB1);
        CAN_RxFrame(1);
        /*add by BWF for when receive consecutive request frame only response the first frame*/
        EB_TpReqManage();
        SoftTimerStart(Tester_Absent_Timer, TESTER_ABSENT_TIMEROUT);
        Tester_Absent_Flg = FALSE;
    }
    // 0x778 Swaybar_Physical_Diagnostic_Tx
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB2)) 
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB2);
        CAN_TxConf(2);
    }
    // 0x112 TCU_3
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB3))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB3);

        // Diag 0x28
        if(Normal_Message_Receive_Flg == TRUE)
        {
            can_mailbox_receive_data_read(CAN0, 3, &g_receive_message);
            if(g_receive_message.dlc == 8)
            {
                CAN_Rcv.TCU_3.TCU_Drivemode = (ubyte)(rx_data[4] & 0x3F);
                CAN_Rcv.TCU_3.TCU_3_RollingCounter = (ubyte)(rx_data[6] & 0x0F);
                CAN_Rcv.TCU_3.TCU_Drivemode = rx_data[7];
            }
            CAN_Rcv.TCU_3.Timeout.Rcv_Flag = 1;
            DTC_Judge.TCU_3_RxFrame_Flag = 1;
        }
    }
    // 0x202 ABS_3
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB4))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB4);

        // Diag 0x28
        if(Normal_Message_Receive_Flg == TRUE)
        {
            can_mailbox_receive_data_read(CAN0, 4, &g_receive_message);
            if(g_receive_message.dlc == 8)
            {
                CAN_Rcv.ABS_3.ABS_VehicleSpeedValid = (ubyte)((rx_data[0] >> 5) & 0x01);
                CAN_Rcv.ABS_3.ABS_VehicleSpeed = (uword)(((uword)rx_data[0] & 0x1F) << 8);
                CAN_Rcv.ABS_3.ABS_VehicleSpeed += (uword)rx_data[1];
                CAN_Rcv.ABS_3.ABS_3_RollingCounter = (ubyte)(rx_data[6] & 0x0F);
                CAN_Rcv.ABS_3.ABS_3_Checksum = rx_data[7];
            }
            CAN_Rcv.ABS_3.Timeout.Rcv_Flag = 1;
            DTC_Judge.ABS_3_RxFrame_Flag = 1;
        }
    }
    // 0x240 AWD_1
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB5))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB5);

        // Diag 0x28
        if(Normal_Message_Receive_Flg == TRUE)
        {
            can_mailbox_receive_data_read(CAN0, 5, &g_receive_message);
            if(g_receive_message.dlc == 8)
            {
                CAN_Rcv.AWD_1.AWD_SystemOperatingMode = (ubyte)((rx_data[0] >> 3) & 0x0F);
                CAN_Rcv.AWD_1.AWD_1_RollingCounter = (ubyte)(rx_data[6] & 0x0F);
                CAN_Rcv.AWD_1.AWD_1_Checksum = rx_data[7];
            }
            CAN_Rcv.AWD_1.Timeout.Rcv_Flag = 1;
            DTC_Judge.AWD_1_RxFrame_Flag = 1;
        }
    }
    // 0x388 TxConfermation
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB6))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB6);
        CAN0BusOffSts = 0;
    }
    // 0xD0 DebugMessage
    else if(RESET != can_interrupt_flag_get(CAN0, CAN_INT_FLAG_MB31))
    {
        can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_MB31);
        can_mailbox_receive_data_read(CAN0, 31, &g_receive_message);

        if((rx_data[4]==0x12)&&(rx_data[5]==0x21)&&
            (rx_data[6]==0xDB)&&(rx_data[7]==0xBD))
        {
            CAN_Rcv.DebugMessage[0] = rx_data[0];
            CAN_Rcv.DebugMessage[1] = rx_data[1];
            CAN_Rcv.DebugMessage[2] = rx_data[2];
            CAN_Rcv.DebugMessage[3] = rx_data[3];
        }
    }
    else
    {

    }

}


//=================================================================================================
// [Function Name]void CAN_vInit(void)
// [Parameters] None
// [Introduction]
// [Return value] None
// [Creator]
//=================================================================================================
void CAN_vInit(void)
{
    CAN_CAN0Init();
    // CAN_CAN1Init();

    g_transmit_message.rtr = 0;
    g_transmit_message.ide = 0;
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.brs = 0;
    g_transmit_message.fdf = 0;
    g_transmit_message.prio = 0;
    g_transmit_message.data_bytes = 8;
}


//=================================================================================================
// [Function Name] CanDebugSend
// [Parameters]
// [Introduction] Send debugging message
// [Return value] none
// [Creator] 20240517 zzy
//=================================================================================================
void CanDebugSend(ulong ulID,ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7)
{
    stCAN_SWObj  stCAN_SWObj_Tx;

    stCAN_SWObj_Tx.ubData[0] = byte0;
    stCAN_SWObj_Tx.ubData[1] = byte1;
    stCAN_SWObj_Tx.ubData[2] = byte2;
    stCAN_SWObj_Tx.ubData[3] = byte3;
    stCAN_SWObj_Tx.ubData[4] = byte4;
    stCAN_SWObj_Tx.ubData[5] = byte5;
    stCAN_SWObj_Tx.ubData[6] = byte6;
    stCAN_SWObj_Tx.ubData[7] = byte7;

    g_transmit_message.data = (uint32_t *)(stCAN_SWObj_Tx.ubData);
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.id = ulID;
    g_transmit_message.rtr = 0;
    g_transmit_message.ide = 0;
    g_transmit_message.brs = 0;
    g_transmit_message.fdf = 0;
    g_transmit_message.prio = 0;
    g_transmit_message.data_bytes = 8;

    can_mailbox_config(CAN0, 30, &g_transmit_message);
}


