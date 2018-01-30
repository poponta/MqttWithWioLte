//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef DISTANCE_SENSOR_SOURCE_H_
#define DISTANCE_SENSOR_SOURCE_H_

#include "AbstractDataSource.h"

class DistanceSensor : public AbstractDataSource {
 public:
  DistanceSensor() = default;
  virtual ~DistanceSensor() = default;
  
  virtual void Init() final;
  virtual void GetData(st_CollectedData *data) final;
};

#endif  // DISTANCE_SENSOR_SOURCE_H_
