//
//  Copyright (c) 2018 Hirotaka Yuno <create.future.technology@gmail.com>.  All right reserved.
//

#include "CommonResource.h"
#include "NetworkManager.h"

namespace {
  void GenerateClientId(char *id) {
    long num = random(100000);
    char tmp_id[ConstantValue::CLIENT_ID_SIZE] = {0};
    String client = String(num);
    
    SerialUSB.print("client ID: ");
    SerialUSB.println(client);
    client.toCharArray(tmp_id, ConstantValue::CLIENT_ID_SIZE);
    memcpy(id, tmp_id, ConstantValue::CLIENT_ID_SIZE);
  }
}

NetworkManager::NetworkManager(WioLTE *wio, WioLTEClient *wio_client, PubSubClient *mqtt_client) :
  wio_(wio), wio_client_(wio_client), mqtt_client_(mqtt_client){
}

NetworkManager& NetworkManager::GetInstance() {
  static WioLTE wio;
  static WioLTEClient wio_client(&wio);
  static PubSubClient mqtt_client;
  static NetworkManager instance(&wio, &wio_client, &mqtt_client);
  return instance;
}

// LTEモジュールを初期化する
bool NetworkManager::Init() {  
  wio_->Init();
  wio_->PowerSupplyLTE(true);
  delay(5000);  // Wait for wake up LTE module.
  
  if (!wio_->TurnOnOrReset()) {
    // init error;
    SerialUSB.println("TurnOnOrReset() error");
    return false;
  }
  SerialUSB.println("TurnOnOrReset() success");
  return true;
}

// LTE通信を切断する
bool NetworkManager::Exit() {
  //  if (mqtt_client_->connected() == true) {
  //  mqtt_client_->disconnect();
  //}
  wio_->Deactivate();  // Deactivate a PDP context. Added at v1.1.9
  wio_->TurnOff(); // Shutdown the LTE module. Added at v1.1.6
  wio_->PowerSupplyLTE(false); // Turn the power supply to LTE module off
}

// NTPサーバから時刻(UTC)を取得する
bool NetworkManager::GetNtpTime(tm *p_current_time) {
  if(!wio_->SyncTime(ConstantValue::NTP_SERVER)) {
    SerialUSB.println("SyncTime() error");
    return false;
  }
  return wio_->GetTime(p_current_time);
}

// PDPコンテクストをアクティベートする
bool NetworkManager::Activate() {
  if (!wio_->Activate(ConstantValue::APN,
                      ConstantValue::USERNAME,
                      ConstantValue::PASSWORD)) {
    // Activate error.
    SerialUSB.println("Activate() error");
    return false;
  }
  SerialUSB.println("Activate() success");
  return true;
}

// MQTTサーバへ接続する
bool NetworkManager::MqttConnect() {
  char client_id[ConstantValue::CLIENT_ID_SIZE] = {0};

  mqtt_client_->setServer(ConstantValue::MQTT_SERVER_HOST,
                          ConstantValue::MQTT_SERVER_PORT);
  
  mqtt_client_->setClient(*wio_client_);
  
  GenerateClientId(client_id);
  if (!mqtt_client_->connect(client_id)) {
    SerialUSB.println("connect() error");
    return false;
  }
  SerialUSB.println("connect() success");
  return true;
}

// データをpublishする
bool NetworkManager::MqttPublish(const char *json) {
  return mqtt_client_->publish(ConstantValue::TOPIC_NAME, json);
}
