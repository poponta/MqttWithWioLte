//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef ENERGY_MANAGER_H_
#define ENERGY_MANAGER_H_

#include <RTClock.h>
#include "PowerCtrl.h"

class EnergyManager {
 public:
  static EnergyManager& GetInstance(PowerCtrl *p_pwr_pctrl, RTClock *p_rtc);
  void SetCurrentTime(tm *p_current_time);
  void GetCurrentTime(tm *p_current_time);
  void EnterStandbyMode(time_t wakeup_time);
  
  
 private:
  EnergyManager(PowerCtrl *p_pwr_pctrl, RTClock *p_rtc);  // singleton
  virtual ~EnergyManager() = default;

 private:
  PowerCtrl *pwr_ctrl_;
  RTClock   *rtc_;
};

#endif  // ENERGY_MANAGER_H_
