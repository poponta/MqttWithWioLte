//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef COMMON_RESOURCE_H_
#define COMMON_RESOURCE_H_

#include <Arduino.h>
#include <time.h>

namespace ConstantValue {
  const char APN[] = "soracom.io";
  const char USERNAME[] = "sora";
  const char PASSWORD[] = "sora";
  const char MQTT_SERVER_HOST[] = "cloudrock.tech";
  const char NTP_SERVER[] = "ntp.nict.jp";
  const char TOPIC_NAME[] = "iTrashCan/test";
  const uint16 MQTT_SERVER_PORT = 1883;
  const int CLIENT_ID_SIZE = 32;
  const time_t WAKE_UP_INTERVAL = 15 * 60;  // 15(min) * 60(sec)
  const int DATA_SOURCE_NUM_MAX = 8;
  const int JSON_MESSAGE_SIZE = 512;
  const time_t JAPAN_TIME_DIFF = 9 * 60 * 60; // UTC + 9h
}
#endif  // COMMON_RESOURCE_H_
