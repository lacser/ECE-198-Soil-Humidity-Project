/* USER CODE BEGIN 0 */
uint16_t readValue;
uint16_t valueSum {0};
uint16_t count {0};
/* USER CODE END 0 */

  /* USER CODE BEGIN 2 */
  HAL_ADC_Start(&hadc1); //This is preconfigured in the CubeIDE
  /* USER CODE END 2 */

 /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    HAL_ADC_PollForConversion(&hadc1,1000); //Translates to decimal data from analog
    readValue = HAL_ADC_GetValue(&hadc1);
    count += 1; //Counts the number of data collected
    if (count < 3){
        valueSum += readValue;
        continue;
    }
    uint8_t parity = 0;
    uint16_t value_copy = valueSum; //Make an expendable copy of value
    while (value_copy){
        parity ^= (value_copy & 1); 
        value_copy >>= 1;
    } //This algorithm makes parity 1 if there is an odd number of 1 bits, and 0 if an even number of 1 bits
    valueSum <<= 1;
    valueSum += parity; //This appends the parity bit to the end of the data
    HAL_UART_Transmit(&huart1, valueSum, sizeof(valueSum) - 1, HAL_MAX_DELAY); //UART is preconfigured in the CubeIDE


  }  
    /* USER CODE END WHILE */



/* USER CODE BEGIN 0 */
uint16_t receiveValue;
uint16_t THRESHOLD {35};
/* USER CODE END 0 */
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start(&hadc1); //This is preconfigured in the CubeIDE
  /* USER CODE END 2 */
 /* USER CODE BEGIN WHILE */
  while (1)
  {
    u16int_t count {0};
    HAL_UART_Receive(&huart1, rxData, sizeof(rxData) - 1, HAL_MAX_DELAY);
    uint16_t dataCopy = receiveValue;
    while (dataCopy){
      if (value_copy & 1){
        count++;
      }
      value_copy >>= 1; //This checks for the number of 1s in the data
    }
    if (count % 2 == 0){
      receiveValue >>= 1; //Removes the last bit
      if (receiveValue > THRESHOLD){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); //Turn on LED
        HAL_Delay(1000)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
      }
      else{
      HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //This is preconfigured in IDE
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET); //Turn on LED
      HAL_Delay(1000);                          // Beep for 1 second
      HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
      }
    }
    else { //Parity check failed
      HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //This is preconfigured in IDE
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); //Turn on LED
      HAL_Delay(1000);                          // Beep for 1 second
      HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    }
  }  
    /* USER CODE END WHILE */
