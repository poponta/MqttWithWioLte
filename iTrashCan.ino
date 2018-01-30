#include <RTClock.h>
#include "MainTask.h"
#include "NetworkManager.h"
#include "PowerCtrl.h"
#include "EnergyManager.h"

MainTask main_task;

void setup() {

  // 各クラスのSingletonなインスタンスを生成し、メインタスクに渡す
  static RTClock rtc(RTCSEL_LSI);
  NetworkManager &mqtt_mgr = NetworkManager::GetInstance();
  PowerCtrl &pwr_ctrl = PowerCtrl::GetInstance();
  EnergyManager &eng_mgr = EnergyManager::GetInstance(&pwr_ctrl, &rtc);

  main_task.Init(&mqtt_mgr, &eng_mgr);
  main_task.setup();
}

void loop()
{
  main_task.loop();
}
