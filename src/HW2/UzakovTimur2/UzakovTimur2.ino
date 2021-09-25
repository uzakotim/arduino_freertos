#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>
#include <queue.h>
#include <stdio.h>

#define QUEUE_LENGTH 3
#define ITEM_SIZE_QUEUE sizeof(SemaphoreHandle_t)

#define WORK_TIME 50
#define BREAK_TIME 50

TaskHandle_t taskHandle;

TaskHandle_t taskPrintHandle;

QueueHandle_t semShovels;
SemaphoreHandle_t mutex;
SemaphoreHandle_t semSoilHeap;


int i_lower = 0;
int i_upper = 0;

void setup() {
 


  semShovels = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE_QUEUE);
  configASSERT(semShovels);

  mutex = xSemaphoreCreateMutex();
  semSoilHeap = xSemaphoreCreateMutex();

  xQueueSendToBack(semShovels,&mutex,(TickType_t) 10);
  xQueueSendToBack(semShovels,&mutex,(TickType_t) 10);
  xQueueSendToBack(semShovels,&mutex,(TickType_t) 10);
  
  xTaskCreate(print_something,"PrintTask",128,NULL,210,&taskPrintHandle);

  CreateTasks();
  
  Serial.begin(9600);
  delay(100);
  Serial.println();
  
}
void print_something(void* parameter)
{
  while(1){
    Serial.println("hello world\n");
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
void CreateTasks()
{
    char taskName[10];
    sprintf(taskName, "tWorkerL%d",i_lower);
    xTaskCreate(digger_in_hole,taskName,128,i_lower,0,&taskHandle);
    sprintf(taskName,"tWorkerU%d",i_upper);
    xTaskCreate(digger_above,taskName,128,i_upper,0,&taskHandle);
}
void digger_above(int parameter)
{
  SemaphoreHandle_t xShovel;
  while(1)
  {
    if( xSemaphoreTake(semSoilHeap,( TickType_t ) 10) == pdPASS )
      {
          xQueueReceive( semShovels,&( xShovel ),( TickType_t ) 10 );
          xSemaphoreTake( xShovel, ( TickType_t ) 10 );
          Serial.print("upper digger ");
          Serial.print(parameter);
          Serial.print(": working\n");
          vTaskDelay(WORK_TIME/ portTICK_PERIOD_MS);
          xSemaphoreGive(semSoilHeap);
          xSemaphoreGive(xShovel);
          xQueueSendToBack(semShovels,&xShovel,(TickType_t) 10);
          Serial.print("upper digger ");
          Serial.print(parameter);
          Serial.print(": resting\n");    
          vTaskDelay(BREAK_TIME/ portTICK_PERIOD_MS);
          Serial.println();
      }
    
  }
}
void digger_in_hole(int parameter)
{
  SemaphoreHandle_t xShovel;
  while(1)
  {
    xQueueReceive( semShovels,&( xShovel ),( TickType_t ) 10 );
    xSemaphoreTake( xShovel, ( TickType_t ) 10 );
    Serial.print("lower digger ");
    Serial.print(parameter);
    Serial.print(": working\n");
    vTaskDelay(WORK_TIME / portTICK_PERIOD_MS);
    xSemaphoreGive(semSoilHeap);
    xSemaphoreGive(xShovel);
    xQueueSendToBack(semShovels,&xShovel,(TickType_t) 10);
    Serial.print("lower digger ");
    Serial.print(parameter);
    Serial.print(": resting\n");    
    vTaskDelay(BREAK_TIME / portTICK_PERIOD_MS);
    Serial.println();
  }
}
void loop() {
}
