//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#include <Arduino.h>
#include <RTClock.h>
#include <time.h>
#include "EnergyManager.h"
#include "PowerCtrl.h"
#include <libmaple/bkp.h>

EnergyManager::EnergyManager(PowerCtrl *p_pwr_ctrl, RTClock *p_rtc) :
  pwr_ctrl_(p_pwr_ctrl),
  rtc_(p_rtc) {
}

EnergyManager& EnergyManager::GetInstance(PowerCtrl *p_pwr_ctrl, RTClock *p_rtc) {
  static EnergyManager instance(p_pwr_ctrl, p_rtc);
  return instance;
}

void EnergyManager::SetCurrentTime(tm *p_current_time) {
  rtc_->setTime(p_current_time);
}

void EnergyManager::GetCurrentTime(tm *p_current_time) {
  rtc_->getTime(p_current_time);
}

void EnergyManager::EnterStandbyMode(time_t wakeup_time) {
  struct tm dbg = *gmtime(&wakeup_time);
  
  // see STM32F405xx Reference manual

  rtc_->turnOffAlarmA();

  rtc_enter_config_mode();
  RTC_BASE->ISR &= ~(1 << RTC_ISR_ALRAF_BIT);
  rtc_exit_config_mode();
  *bb_perip(&EXTI_BASE->PR, EXTI_RTC_ALARM_BIT) = 1;

  pwr_ctrl_->ClearWUF();

  rtc_->setAlarmATime(wakeup_time);

  SerialUSB.print("next wake up time: ");  // for debug
  SerialUSB.println(asctime(&dbg));  // for debug
  delay(100);  // for debug
  
  pwr_ctrl_->EnterStandbyMode();  // enter standby mode
}
