#include <Arduino_FreeRTOS.h>
#include <stdio.h>

long int task_run[]={100,450,200};
int task_stop[]= {18,25,30};


TaskHandle_t taskZeroHandle;
TaskHandle_t taskOneHandle;
TaskHandle_t taskTwoHandle;

void task(int parameter)
{   
    int n = parameter;
    long int x;
    
    Serial.begin(9600);
    delay(100);
    Serial.print("Task "); 
    Serial.print(n);
    Serial.println(" has been started");


    while(1){
      Serial.print("task "); 
      Serial.print(n);
      Serial.println(": running");

      x = 1000000 * task_run[n];
      while(x>0) x--;
      Serial.print("task "); 
      Serial.print(n);
      Serial.println(": stopped");
      vTaskDelay(task_stop[n]);
    }
}

void CreateTasks(void)
{
  int id_zero = 0;
  int id_one = 1;
  int id_two = 2;
//  xTaskCreate(task,"Task0",128,id_zero,210,&taskZeroHandle);
  xTaskCreate(task,"Task1",128,id_one,210,&taskOneHandle);
  xTaskCreate(task,"Task2",128,id_two,210,&taskTwoHandle);
}

void setup()
{

  CreateTasks();
  
}

void loop()
{
}
