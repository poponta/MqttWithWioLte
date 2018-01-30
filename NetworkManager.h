//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#ifndef MQTT_CLIENT_H_
#define MQTT_CLIENT_H_

#include <WioLTEforArduino.h>
#include <WioLTEClient.h>
#include <PubSubClient.h>

class NetworkManager {
 public:
  NetworkManager(const NetworkManager&) = delete;
  NetworkManager& operator=(const NetworkManager&) = delete;

  static NetworkManager& GetInstance();  // Singleton
  bool Init();
  bool GetNtpTime(tm *p_current_time);
  bool Connect();
  bool Exit();
  bool Activate();
  bool MqttConnect();
  bool MqttPublish(const char *json);
  
 private:
  NetworkManager(WioLTE *wio, WioLTEClient *wio_client, PubSubClient *mqtt_client);
  virtual ~NetworkManager() = default;
  WioLTE *wio_;
  WioLTEClient *wio_client_;
  PubSubClient *mqtt_client_;
};
#endif  // MQTT_CLIENT_H_
