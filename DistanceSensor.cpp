//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#include "DistanceSensor.h"

void DistanceSensor::Init() {

}

void DistanceSensor::GetData(st_CollectedData *data) {
  data->distance = 100.00;  // for test
}
