//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef MAIN_TASK_H_
#define MAIN_TASK_H_

#include "AbstractDataSource.h"
#include "CommonResource.h"
#include "EnergyManager.h"
#include "NetworkManager.h"
#include "PowerCtrl.h"

typedef enum MainState {
  kMeasurement = 1,
  kPublish,
  kExit
}MainState;

class MainTask {
 public:
  MainTask() = default;
  virtual ~MainTask() = default;
  
  void setup();
  void loop();
  void Init(NetworkManager *mqtt_mgr, EnergyManager *eng_mgr);

 private:
  MainState state_;
  NetworkManager *net_mgr_;
  EnergyManager *eng_mgr_;
  AbstractDataSource *data_source_[ConstantValue::DATA_SOURCE_NUM_MAX];
};
#endif  // MAIN_TASK_H_
