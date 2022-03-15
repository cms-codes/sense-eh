#include <uart.h>

void _uart_init(void) 
{

	usart_set_baudrate( USART2, 115200 );
	usart_set_databits( USART2, 8 );
	usart_set_stopbits( USART2, USART_CR2_STOPBITS_1 );
	usart_set_mode( USART2, USART_MODE_TX );

	usart_set_parity( USART2, USART_PARITY_NONE );
	usart_set_flow_control( USART2, USART_FLOWCONTROL_NONE );

	usart_enable( USART2 );
	
}

void Uart_send( volatile void* data, int length ) 
{

	_uart_init();
	usart_disable_tx_dma( USART2 );
	dmamux_reset_dma_channel( DMAMUX1, DMA_CHANNEL3 );
	dma_channel_reset( DMA1, DMA_CHANNEL3 );

	dma_set_peripheral_address( DMA1, DMA_CHANNEL3, (uint32_t) &USART2_TDR );
	dma_set_memory_address( DMA1, DMA_CHANNEL3, (uint32_t) data );
	dma_set_number_of_data( DMA1, DMA_CHANNEL3, length );
	dma_set_read_from_memory( DMA1, DMA_CHANNEL3 );
	dma_enable_memory_increment_mode( DMA1, DMA_CHANNEL3 );
	dma_set_peripheral_size( DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_8BIT );
	dma_set_memory_size( DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_8BIT );
	dma_set_priority( DMA1, DMA_CHANNEL3, DMA_CCR_PL_VERY_HIGH );
	dma_enable_transfer_complete_interrupt( DMA1, DMA_CHANNEL3 );

	dmamux_set_dma_channel_request( DMAMUX1, DMA_CHANNEL3, DMAMUX_CxCR_DMAREQ_ID_USART2_TX );
	
	dma_enable_channel( DMA1, DMA_CHANNEL3 );

	USART2_ICR |= USART_ICR_TCCF;
	usart_enable_tx_dma( USART2 );

	// Wait for DMA transfer
	while ( !dma_get_interrupt_flag( DMA1, DMA_CHANNEL3, DMA_TCIF ) );
	// Wait for USART transfer
	while ( !( USART2_ISR & USART_ISR_TC ) );
	usart_disable( USART2 );

}
