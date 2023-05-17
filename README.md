# 问题合集

### 串口23.5.16

- [x] ~~为什么没加packed会变成60ms有数据~~



> 已解决

打波形的时候会发现30ms刷新一次，我们一般是14ms刷新一次，有可能存了两次的数据

于是矛头理论上会指向中断函数部分，现在处理的数据结构体都是按cortex最喜欢的全是原生int处理，没有做__packed做多两次向下转型，但理论上不会这么夸张的，60ms一次刷新。



> 有可能是因为之前设置的SBUS的接受长度要36，我们此举是为了防止上溢
>
> 但是进入中断的时间差都是14，人傻了



现在有个疑惑，如果你设置了DMA传输大小为36，他一定是被传输到两次才会从CR搬数据到内存吗？

那我现在传了18给你，因为开了接受中断，所以在中断里突然间把你关掉，怎么我的内存里就能读到18个的数据流？

```c
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	
	__HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_IDLEF);
    __HAL_DMA_DISABLE(huart1.hdmarx);
    RC_UART_Handler();
  /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
    __HAL_DMA_ENABLE(huart1.hdmarx);
  /* USER CODE END USART1_IRQn 1 */
}
```

秉承着能用就行原则，打完比赛再来解析